#include "../include/Watchable.h"

Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags) :
    Watchable(id,length, tags), name(name), mov(true) {

}



std::string Movie::toString() const {
    return name;
}

void Movie::operator= (const Movie &other) {

}

bool Movie::isEpisode() {
    return !(mov);
}




//Watchable *Movie::getNextWatchable(Session &) const {
//    return nullptr;
//}



