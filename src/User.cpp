#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Action.h"

using namespace std;

// User Constructor
// Initialize name string
User::User(const std::string &name):name(name) {

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
Watchable* User::getRecommendation(Session &s) {}

std::string User::getName() const {
    return name;
}

std::vector<Watchable *> User::getHistory() {
    return history;
}

std::string User::getAlgo() {
    return userAlgo;
}

void User::setAlgo(std::string algo) {
    userAlgo=algo;
}

// TODO change this function to copy history
void User::setHistory(std::vector<Watchable *> historyToCopy) {
    for (auto x: historyToCopy){
        history.push_back(x);
    }
}

void User::addToHistory(Watchable *watchable) {
    history.push_back(watchable);
}

void User::setNotSeen(std::vector<Watchable *> notSeenToCopy) {
    for (auto x: notSeenToCopy){
        notSeen.push_back(x);
    }
}

std::vector<Watchable *> User::get_notSeen() {
    return notSeen;
}

void User::updateRec(Watchable *watchable) {}

void User::removeFromNotSeen(Watchable* watchToDelete, int id) {
    notSeen.erase(notSeen.begin() + id);
}

//==========================================Length Recommender User=====================================================

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
    while (i<get_notSeen().size()){
        Watchable* temp = get_notSeen().at(i);
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

void LengthRecommenderUser::updateRec(Watchable *watchable) {
    totalTime += watchable->getLength();
    howManyMovies++;
    averageTime = totalTime / howManyMovies; // TODO check double, int, float...
}

//===========================================Rerun Recommender User=====================================================

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) {
    setAlgo("rer");
    histLength = getHistory().size();
    watchIndex = 0;
}

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    Watchable* rec;
    if (watchIndex == 0){
        rec = getHistory().at(0);
    }
    else {
        rec = getHistory().at((watchIndex)%histLength);
    }
    watchIndex++;
    return rec;
}

std::string RerunRecommenderUser::getAlgo() {
    return User::getAlgo();
}

void RerunRecommenderUser::updateRec(Watchable *watchable) {
    addToHistory(watchable);
    histLength++;
}

//==========================================Genre Recommender User=====================================================
// TODO possible to make it faster by deleting and inserting back to the multimap
// https://stackoverflow.com/questions/11343822/how-to-replace-key-value-in-a-std-multimap
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) {
    setAlgo("gen");
}

Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    // Sorting the tags by appearance
    for (auto x: counterTag) {
        std::pair<int, std::string> temp_pair(x.second, x.first);
        tagsMap.insert(temp_pair);
    }
    // Sorting the tags by lex
    for (auto x: tagsMap) {
        std::pair<std::string, int> temp_pair(x.second, x.first);
        sortedTagsMap.insert(temp_pair);
    }
    // Searching the relevant tag in the not seen vector
    for (auto x: sortedTagsMap) {
        std::string tag = x.first;
        for (int i = 0; i < get_notSeen().size(); i++) {
            std::vector<std::string> temp_v = get_notSeen().at(i)->getTags();
            for (auto str: temp_v) {
                if (str == tag) {
                    return get_notSeen().at(i);
                }
            }
        }
    }
    return nullptr;
}

std::string GenreRecommenderUser::getAlgo() {
    return User::getAlgo();
}

void GenreRecommenderUser::updateRec(Watchable *watchable) {
    //TODO make it prettier and better

    // Updating counter for each tag in the unordered map
    for (int i=0;i<watchable->getTags().size();i++) {
        std::string temp_tag = watchable->getTags().at(i);
        if (counterTag.find(temp_tag) != counterTag.end()) {
            counterTag.at(temp_tag)++;
        }
        else {
            std::pair<std::string, int> temp_pair(temp_tag, 1);
            counterTag.insert(temp_pair);
        }
   }
}
