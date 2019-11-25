#include "../include/Watchable.h"

#include <utility>
#include <iostream>

Watchable::Watchable(long id, int length, std::vector<std::string> tags) :
    id(id), length(length), tags(std::move(tags)) {}

Watchable::~Watchable() {

}

int Watchable::getLength() {
    return length;
}

int Watchable::getId() {
    return  id;
}

std::string Watchable::tagsToString() {
    int i = 0;
    std::string tagString;
    while (i < tags.size()) {
        tagString += tags.at(i);
        i++;
        if (i< tags.size())
            tagString+=", ";
    }
    return tagString;
}

std::vector<std::string> Watchable::getTags() {
    return tags;
}

std::string Watchable::getSeriesName() {}


