#pragma once

#include <vector>
#include <memory>
#include "event.h"

class Link;

class Linkable{
    protected:
        std::vector<Link*> incomingLinks;
        std::vector<Link*> outgoingLinks;
    public:
        Linkable(){}
        inline void addOutgoingLink(Link* link){
            outgoingLinks.push_back(link);
        }
        inline void addIncomingLink(Link* link){
            incomingLinks.push_back(link);
        }
        virtual ListOfEvents receive(std::shared_ptr<RequestUnloadEvent> event) = 0;
};


class Link{
    const Linkable& from;
    Linkable& to;

    public:
        Link(const Linkable& src, Linkable& dst): from(src), to(dst){}
        ListOfEvents receive(std::shared_ptr<RequestLoadEvent> event) const;
};
