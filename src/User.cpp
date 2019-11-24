#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/Session.h"

#include <algorithm>


using namespace std;

// User Constructor
// Initialize name string
User::User(const std::string &name):name(name) {}


//copy assi
//User& User::operator= (const User& other)  {
//    if ( this != &other )
//    {
//        history = other.history;
//    }
//}

// copy cons
User::User(const User& other) : name(other.name)  {
    std::vector<Watchable*> history(other.history.begin(), other.history.end());
}

User::~User() {
    for (Watchable* w: history){
        delete w;
    }
    history.clear();
}
//Watchable* User::getRecommendation(Session &s) {}

std::string User::getName() const {
    return name;
}

std::vector<Watchable *> User::getHistory() {
    return history;
}

//std::string User::getAlgo() {
//    return userAlgo;
//}

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

void User::updateRec(Watchable *watchable) {}

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
    while (i<s.getContent().size()){
        if (std::find(history.begin(), history.end(), s.getContent().at(i)) == history.end()) {
            Watchable* temp = s.getContent().at(i);
            int len = temp->getLength();
            if (std::abs(len-averageTime) < min){
                min = std::abs(len-averageTime);
                rec = temp;
            }
        }
        i++;
    }
    return rec;
}

std::string LengthRecommenderUser::getAlgo() const {
//    return User::getAlgo();
    return "len";
}

void LengthRecommenderUser::setHistory(std::vector<Watchable *> historyToCopy) {
    User::setHistory(historyToCopy);
}

void LengthRecommenderUser::updateRec(Watchable *watchable) {
    totalTime += watchable->getLength();
    howManyMovies++;
    averageTime = totalTime / howManyMovies; // TODO check double, int, float...
}

User *LengthRecommenderUser::duplicateUser(const std::string &name) {
    LengthRecommenderUser* newUser = new LengthRecommenderUser(name);
    for (auto x: history) {
        newUser->addToHistory(x);
    }
    return newUser;
}

//===========================================Rerun Recommender User=====================================================

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) {
    setAlgo("rer");
    histLength=0;
}

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    Watchable* rec;
    watchIndex = history.at(history.size()-1)->getId();
    rec = getHistory().at((watchIndex+1)%histLength);
    return rec;
}

std::string RerunRecommenderUser::getAlgo() const {
//    return User::getAlgo();
    return "rer";
}

void RerunRecommenderUser::updateRec(Watchable *watchable) {
    histLength++;
}

User *RerunRecommenderUser::duplicateUser(const std::string &name) {
    RerunRecommenderUser *newUser = new RerunRecommenderUser(name);
    for (auto x: history) {
        newUser->addToHistory(x);
    }
    return newUser;
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
    // Searching the relevant tag
    for (auto x: sortedTagsMap) {
        std::string tag = x.first;

        for (int i = 0; i < s.getContent().size(); i++) {
            if (std::find(history.begin(), history.end(), s.getContent().at(i)) == history.end()) {
                std::vector<std::string> temp_v = s.getContent().at(i)->getTags();
                for (auto str: temp_v) {
                    if (str == tag) {
                        return s.getContent().at(i);
                    }
                }
            }
        }
    }
    return nullptr;
}

std::string GenreRecommenderUser::getAlgo() const {
//    return User::getAlgo();
    return "gen";
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

User *GenreRecommenderUser::duplicateUser(const std::string &name) {
    GenreRecommenderUser *newUser = new GenreRecommenderUser(name);
    for (auto x: history) {
        newUser->addToHistory(x);
    }
    return newUser;
}
