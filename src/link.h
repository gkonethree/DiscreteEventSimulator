#pragma once

#include <unordered_map>
#include <memory>
#include <iostream>
#include "event.h"


class Link;
class Server;
class User;

// Since the server system nodes behave differently from the user nodes in choosing the next node 
// probabilistically, the only common property all linkable share is the ability to handle 
// requestunloadevents.

class Linkable{
    public:
        Linkable(){}
        virtual ListOfEvents receive(std::shared_ptr<RequestUnloadEvent> event) = 0;
};


class Link{
    private:
        Linkable& from;
        Linkable& to;
        const Time propDelay;
    public:
        Link(Linkable& from, Linkable& to, const Time propDelay): from(from), to(to), propDelay(propDelay){}
        ListOfEvents receive(std::shared_ptr<RequestLoadEvent> event) const;
};

class ServerServerLink: public Link{
    const double weight;
    public:
        ServerServerLink(Server& from, Server& to, double weight, const Time propDelay);
};

class ServerUserLink: public Link{
    public:
        ServerUserLink(Server& from, User& to, const Time propDelay);
};


class UserServerLink: public Link{
    public:
        UserServerLink(User& from, Server& to, const Time propDelay);
};
