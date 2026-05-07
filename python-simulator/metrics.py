import numpy as np
from request import Request

class Metrics:

    def __init__(self):
        self.response_times = []
        self.goodput = 0
        self.badput = 0
        self.dropped = 0
        self.total_requests = 0
        self.core_busy_time = 0.0

    def record_completion(self, request:Request, sim_time):
        rt = sim_time - request.arrival_time
        self.response_times.append(rt)

        if request.timed_out:
            self.badput += 1
        else:
            self.goodput += 1

    def summary(self, sim_time):
        return {
            "avg_response_time": np.mean(self.response_times)
            if self.response_times else 0,
            "throughput": (self.goodput + self.badput) / sim_time ,
            "goodput": self.goodput / sim_time,
            "badput": self.badput / sim_time,
            "drop_rate": self.dropped / max(1, self.total_requests),
        }
