#include "../include/User.h"
#include "../include/Watchable.h"

// User Constructor
// Initialize name string
User::User(const std::string &name):name(name){}

//copy assi
User& User::operator= (const User& other)  {
    if ( this != &other )
    {
        history = other.history;
        notSeen = other.notSeen;
    }
}

// copy cons
User::User(const User& other) : name(other.name)  {
    std::vector<Watchable*> history(other.history.begin(), other.history.end());
    std::vector<Watchable*> notSeen(other.notSeen.begin(), other.notSeen.end());

}

std::string User::getName() const {
    return name;
}

std::vector<Watchable *> User::get_history() const {
    return history;
}
std::vector<Watchable *>* User::get_notSeen()  {
    std::vector<Watchable*> *notSeen = new std::vector<Watchable *>;
    return notSeen ;
}

LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {
    averageTime = 0;
}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    int i = 0;
    Watchable* rec;
    long min = INTMAX_MAX;
    while (i<get_notSeen()->size()){
        Watchable* temp = get_notSeen()->at(i);
        int len = temp->getLength();
        if (std::abs(len-averageTime) < min){
            min = std::abs(len-averageTime);
            rec = temp;
        }
        i++;
    }
    return rec;
}
