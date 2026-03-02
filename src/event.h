#pragma once

#include <queue>
#include <vector>
#include <memory>

class Link;
class Linkable;
class Server;
class Request;
class Event;
class EventComparator;

typedef unsigned long long Time;
typedef std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparator> EventQueue;
typedef std::vector<std::shared_ptr<Event>> ListOfEvents;


class Event{
    public:
        const Time time;
        Event(Time time) : time(time){}
        virtual ListOfEvents execute() = 0;
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


class EventComparator{
    public:
        bool operator()(const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b) const {
            return a->time > b->time;
        }
};
