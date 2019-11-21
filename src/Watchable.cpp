#include "../include/Watchable.h"

#include <utility>

Watchable::Watchable(long id, int length, std::vector<std::string> tags) :
    id(id), length(length), tags(std::move(tags)) {}

//Watchable& Watchable::(const Watchable& other){
//
//}

//Watchable::~Watchable() {
//
//}

int Watchable::getLength()  {
    return length;

}

