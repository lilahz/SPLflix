#include "../include/Watchable.h"
#include "../include/Session.h"

Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags) :
    Watchable(id,length, tags), name(name), mov(true) {

}

std::string Movie::toString() const {
    return name;
}

void Movie::operator= (const Movie &other) {

}

Watchable *Movie::getNextWatchable(Session &s) const {
    return s.getActiveUser()->getRecommendation(s);
}

Watchable *Movie::clone() const {
    return new Movie(*this);
}
