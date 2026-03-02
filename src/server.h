#pragma once

#include <vector>
#include <queue>
#include <memory>
#include "event.h"
#include "link.h"

class Request;

class Server : public Linkable{
    private:
        std::queue<std::shared_ptr<Request>> requestQueue;
    public:
        Server() {}
        ListOfEvents receive(std::shared_ptr<RequestUnloadEvent>);
        ListOfEvents receive(std::shared_ptr<RequestCompletionEvent>);
};