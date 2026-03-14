#pragma once

#include <queue>
#include <vector>
#include <memory>
#include <iostream>

class Link;
class Linkable;
class Server;
class Request;
class Event;
class EventComparator;
class Core;

typedef double Time;
typedef std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparator> EventQueue;
typedef std::vector<std::shared_ptr<Event>> ListOfEvents;


class Event{
    public:
        const Time time;
        bool outdated;
        Event(Time time) : time(time), outdated(false){}
        virtual ListOfEvents execute() = 0;
        Event(const Event&) = delete;
        Event(Event &&) = delete;
        Event operator=(const Event&) = delete;
        Event operator=(Event&&) = delete;
};


class RequestLoadEvent: public Event, public std::enable_shared_from_this<RequestLoadEvent>{
        const Link& toLoadOn;
    public:
        const std::shared_ptr<Request> request;

        RequestLoadEvent(Time time, const Link& toLoadOn, std::shared_ptr<Request> request) : Event(time), toLoadOn(toLoadOn), request(request){}
        ListOfEvents execute() override;
};


class RequestUnloadEvent: public Event, public std::enable_shared_from_this<RequestUnloadEvent>{
        Linkable& unloadTo;
    public:
        const std::shared_ptr<Request> request;
        RequestUnloadEvent(Time time, Linkable& unloadTo, std::shared_ptr<Request> request) :
                            Event (time), unloadTo(unloadTo), request(request){}
        
        ListOfEvents execute() override;
};


class RequestCompletionEvent: public Event, public std::enable_shared_from_this<RequestCompletionEvent>{
        Server& completeAt;
    public:
        const std::shared_ptr<Request> request;
        RequestCompletionEvent(Time time, Server& completeAt, std::shared_ptr<Request> request) :
                            Event (time), completeAt(completeAt), request(request){}
        
        ListOfEvents execute() override;
};

class TimerInterruptEvent: public Event, public std::enable_shared_from_this<TimerInterruptEvent>{
    Core& core;
    public:
        TimerInterruptEvent(Time time, Core& core): Event(time), core(core){}
        ListOfEvents execute() override;
};


class EventComparator{
    public:
        bool operator()(const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b) const {
            return a->time > b->time;
        }
};
