from dataclasses import dataclass, field
from typing import Any

ARRIVAL = 1
THREAD_ASSIGNED = 2
SLICE_COMPLETE = 3
DEPARTURE = 4
TIMEOUT = 5
CONTEXT_SWITCH = 6


@dataclass(order=True)
class Event:
    time: float
    priority: int
    event_type: int = field(compare=False)
    data: Any = field(compare=False, default=None)