
from typing import List
import heapq
from config import Config
from event import CONTEXT_SWITCH, Event, ARRIVAL, SLICE_COMPLETE
import numpy as np
import random
from request import Request
from webserver import WebServer
from metrics import Metrics

class Simulation:

    def __init__(self, num_users):
        self.sim_time = 0.0
        self.event_list: List[Event] = []
        self.metrics = Metrics()
        self.server = WebServer(self)
        self.num_users = num_users
        self.req_id = 0

    def schedule_event(self, time, event_type, data=None):
        event = Event(time, 0, event_type, data)
        heapq.heappush(self.event_list, event)

    def initialize(self):
        random.seed(Config.RANDOM_SEED)
        np.random.seed(Config.RANDOM_SEED)

        # Schedule first request for each user
        for user_id in range(self.num_users):
            self.schedule_event(0.0, ARRIVAL, user_id)

    def run(self):
        self.initialize()

        while self.event_list and self.sim_time < Config.SIM_TIME:
            event = heapq.heappop(self.event_list)
            self.sim_time = event.time
            self.process_event(event)

        return self.metrics.summary(self.sim_time)

    def process_event(self, event:Event):

        if event.event_type == ARRIVAL:
            self.handle_arrival_event(event.data)

        elif event.event_type == SLICE_COMPLETE:
            core_id, request,next_time = event.data
            self.server.handle_slice_complete(core_id, request,next_time)
            
        elif event.event_type == CONTEXT_SWITCH:
            core_id = event.data
            self.server.start_next_on_core(core_id)
            
    def next_request(self, request:Request):
        think = max(0, Config.THINK_TIME_MEAN)
        self.schedule_event(self.sim_time + think, ARRIVAL, request.user_id)

    def handle_arrival_event(self, user_id):
        self.req_id += 1
        request = Request(self.req_id, user_id, self.sim_time)
        self.server.handle_arrival(request)
        