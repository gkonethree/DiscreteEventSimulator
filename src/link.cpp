#include "link.h"
#include "server.h"
#include "user.h"

ListOfEvents Link::receive(std::shared_ptr<RequestLoadEvent> event) const {
    ListOfEvents consequences;
    std::shared_ptr<Event> new_event = std::make_shared<RequestUnloadEvent>(
        event->time + propDelay, to, event->request
    );
    consequences.push_back(new_event);
    return std::move(consequences);
}

ServerUserLink::ServerUserLink(Server& from, User& to, const Time propDelay): Link(from, to, propDelay){
    from.addUserLink(&to, this);
}

ServerServerLink::ServerServerLink(Server& from, Server& to, double weight, const Time propDelay): Link(from, to, propDelay), weight(weight){
    from.addServerLink(&to, this, weight);
}

UserServerLink::UserServerLink(User& from, Server& to, const Time propDelay): Link(from, to, propDelay){
    from.addServerLink(this);
}