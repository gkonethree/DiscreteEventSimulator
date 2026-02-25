from config import Config
import numpy as np

class Request:
    def __init__(self, req_id, user_id, arrival_time):
        self.req_id = req_id
        self.user_id = user_id
        self.arrival_time = arrival_time

        self.service_time = self.generate_service_time()
        self.remaining_time = self.service_time

        self.timeout = self.generate_timeout()
        self.completed = False
        self.timed_out = False

    #currently exp
    def generate_service_time(self):
        return Config.MIN_SERVICE+np.random.exponential(Config.THINK_TIME_MEAN)

    def generate_timeout(self):
        return Config.MIN_TIMEOUT+np.random.exponential(Config.TIMEOUT_MEAN)