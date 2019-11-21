#include "../include/Watchable.h"

Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags) :
    Watchable(id,length, tags), name(name) {

}

std::string Movie::toString() const {
    return name;
}

//Watchable *Movie::getNextWatchable(Session &) const {
//    return nullptr;
//}

