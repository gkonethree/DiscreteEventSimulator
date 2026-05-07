from collections import deque
from config import Config
from event import SLICE_COMPLETE, CONTEXT_SWITCH
from request import Request

class WebServer:

    def __init__(self, sim):
        self.sim = sim
        self.thread_count = 0
        self.core_current = [None] * Config.NUM_CORES
        self.ready_queue = deque()      #request not given a thread  
        self.core_queues = [deque() for _ in range(Config.NUM_CORES)] #requests assigned to cores but not completed

        

    def handle_arrival(self, request):

        self.sim.metrics.total_requests += 1

        if self.thread_count < Config.MAX_THREADS:
            self.thread_count += 1
            self.assign_to_core(request)
        else:
            self.ready_queue.append(request)


    #assigning a request to a core with shortest queue (i.e. appending in core queue)
    def assign_to_core(self, request):

        core_id = min(range(Config.NUM_CORES),
                      key=lambda i: len(self.core_queues[i]))

        self.core_queues[core_id].append(request)

        if self.core_current[core_id] is None:
            self.start_next_on_core(core_id)

    def start_next_on_core(self, core_id):
        #print("Scheduling slice at time:", self.sim.sim_time)
        if self.core_current[core_id] is not None:
            return
        if not self.core_queues[core_id]:
            self.core_current[core_id] = None
            return
       
        request = self.core_queues[core_id].popleft()
        self.core_current[core_id] = request

        next_time = min(Config.TIME_SLICE, request.remaining_time)

        self.sim.schedule_event(
            self.sim.sim_time + next_time,
            SLICE_COMPLETE,
            (core_id, request,next_time)
        )

    def handle_slice_complete(self, core_id, request:Request, next_time):

        request.remaining_time -= next_time
        self.core_current[core_id] = None
        
        if request.remaining_time <= 0:
            self.finish_request(core_id, request)
        else:
            # since core affinityv
            self.core_queues[core_id].append(request)

            self.sim.schedule_event(
                self.sim.sim_time + Config.CONTEXT_SWITCH,
                CONTEXT_SWITCH,
                core_id
            )

    def finish_request(self, core_id, request:Request):

        if self.sim.sim_time - request.arrival_time > request.timeout:
            request.timed_out = True

        self.sim.metrics.record_completion(request, self.sim.sim_time)

        self.thread_count -= 1
        self.core_current[core_id] = None

        # Assign next waiting request if exists
        if self.ready_queue:
            next_req = self.ready_queue.popleft()
            self.thread_count += 1
            self.assign_to_core(next_req)

        self.start_next_on_core(core_id)
        
        #assuming after timeout user still sends req
        self.sim.next_request(request)