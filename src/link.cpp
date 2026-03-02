#include "link.h"

ListOfEvents Link::receive(std::shared_ptr<RequestLoadEvent> event) const {
    ListOfEvents consequences;
    std::shared_ptr<Event> new_event = std::make_shared<RequestUnloadEvent>(
        event->time + 1, to, event->request
    );
    consequences.push_back(new_event);
    return consequences;
}