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
    public:
        Link(Linkable& from, Linkable& to): from(from), to(to){}
        ListOfEvents receive(std::shared_ptr<RequestLoadEvent> event) const;
};

class ServerServerLink: public Link{
    const double weight;
    public:
        ServerServerLink(Server& from, Server& to, double weight);
};

class ServerUserLink: public Link{
    public:
        ServerUserLink(Server& from, User& to);
};


class UserServerLink: public Link{
    public:
        UserServerLink(User& from, Server& to);
};
