#include "event.h"
#include "server.h"
#include "core.h"

ListOfEvents RequestCompletionEvent::execute(){
    return this->completeAt.receive(std::shared_ptr<RequestCompletionEvent>(shared_from_this()));
}

ListOfEvents RequestUnloadEvent::execute(){
    return this->unloadTo.receive(std::shared_ptr<RequestUnloadEvent>(shared_from_this()));
}

ListOfEvents RequestLoadEvent::execute(){
    return this->toLoadOn.receive(std::shared_ptr<RequestLoadEvent>(shared_from_this()));
}