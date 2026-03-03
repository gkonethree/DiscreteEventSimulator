#pragma once

#include <unordered_map>
#include <memory>
#include <iostream>
#include "event.h"

class Link;

class Linkable{
    protected:
        std::unordered_map<const Linkable*, Link*> incomingLinks;
        std::unordered_map<const Linkable*, Link*> outgoingLinks;
    public:
        Linkable(){}
        inline void addOutgoingLink(Linkable* linkable, Link* link){
            outgoingLinks[linkable] = link;
        }
        inline void addIncomingLink(Linkable* linkable, Link* link){
            incomingLinks[linkable] = link;
        }
        virtual ListOfEvents receive(std::shared_ptr<RequestUnloadEvent> event) = 0;
};


class Link{
        Linkable& from;
        Linkable& to;
    public:
        Link(Linkable& src, Linkable& dst): from(src), to(dst){
            from.addOutgoingLink(&to, this);
            to.addIncomingLink(&from, this);
        }
        ListOfEvents receive(std::shared_ptr<RequestLoadEvent> event) const;
};
