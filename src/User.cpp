#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Action.h"
// User Constructor
// Initialize name string
User::User(const std::string &name):name(name){

}

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

User::~User() {
    for (Watchable* w: history){
        delete w;
    }
    history.clear();
    for (Watchable* w: notSeen){
        delete w;
    }
    notSeen.clear();
}

std::string User::getName() const {
    return name;
}

std::vector<Watchable *> User:: get_history() {
    return history;
}
std::vector<Watchable *> User :: newget_notSeen()  {
    return notSeen;
}

std::string User::getAlgo() {
    return userAlgo;
}

void User::setAlgo(std::string algo) {
    userAlgo=algo;

}



void User::setHistory(std::vector<Watchable *> historyToCopy) {
    std::vector<Watchable*> history(historyToCopy.begin(), historyToCopy.end());
}

void User::setNotSeen(std::vector<Watchable *> notSeenToCopy) {
    std::vector<Watchable*> notSeen(notSeenToCopy.begin(), notSeenToCopy.end());
}

std::vector<Watchable *> *User::get_notSeen() {
    return &notSeen;
}


LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {
    setAlgo("len");
    totalTime=0;
    averageTime=0;
    howManyMovies=0;
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

std::string LengthRecommenderUser::getAlgo() {
    return User::getAlgo();
}

void LengthRecommenderUser::setHistory(std::vector<Watchable *> historyToCopy) {
    User::setHistory(historyToCopy);
}


RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) {
    setAlgo("rer");
}

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    // TODO algo
    return nullptr;
}

std::string RerunRecommenderUser::getAlgo() {
    return User::getAlgo();
}

GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) {
    setAlgo("gen");
}

Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    // TODO algo
    return nullptr;
}

std::string GenreRecommenderUser::getAlgo() {
    return User::getAlgo();
}
