#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/Session.h"

#include <algorithm>

using namespace std;

// User Constructor
User::User(const std::string &name): history(), name(name) {}

//===============================================Rule of Three==========================================================

 //Copy Constructor
User::User(const User &other) : history(), name(other.name) {
     for (auto *x: other.history){
         history.push_back(x->clone());
     }
}

//copy Assignment
User &User::operator=(const User &other) {
    if (this != &other)
    {
        name = other.name;
        // Delete and create new history
        for (Watchable* w: history){
            delete w;
            w = nullptr;
        }
        history.clear();
        for (int unsigned i = 0; i < other.history.size(); i++){
            history.push_back(other.history[i] -> clone());
        }
    }
    return *this;
}

User::~User() {

    history.clear();
}

std::string User::getName() const {
    return name;
}

std::vector<Watchable *> User::getHistory() {
    return history;
}

// TODO change this function to copy history
void User::setHistory(std::vector<Watchable *> historyToCopy) {
    for (auto x: historyToCopy){
        Watchable* temp = x;
        history.push_back(temp);
    }
}

void User::addToHistory(Watchable* watchable) {
        history.push_back(watchable);
}

void User::updateRec(Watchable *watchable) {}


//==========================================Length Recommender User=====================================================


LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name), totalTime(0),
    howManyMovies(0), averageTime(0) {}

//Copy constructor
LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser& other) : User(other),
    totalTime(other.totalTime), howManyMovies(other.howManyMovies), averageTime(other.averageTime) {}

//Copy Assignment
LengthRecommenderUser &LengthRecommenderUser::operator=(const LengthRecommenderUser &other) {
    User::operator=(other);
    totalTime = other.totalTime;
    howManyMovies = other.howManyMovies;
    averageTime = other.averageTime;
    return *this;
}

LengthRecommenderUser::~LengthRecommenderUser() {}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    int unsigned i = 0;
    Watchable* rec;
    long unsigned min = INTMAX_MAX;
    while (i<s.getContent().size()){
        if (std::find(history.begin(), history.end(), s.getContent().at(i)) == history.end()) {
            Watchable* temp = s.getContent().at(i);
            int unsigned len = temp->getLength();
            if (unsigned(std::abs((long)len-averageTime)) < min){
                min = std::abs((long)len-averageTime);
                rec = temp;
            }
        }
        i++;
    }
    return rec;
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

User* LengthRecommenderUser::clone(Session &s) {
    LengthRecommenderUser* temp_user = new LengthRecommenderUser(getName());
    for (auto x: history) {
        int id = x->getId();
        temp_user->history.push_back(s.getContent().at(id));
    }
    temp_user->totalTime = totalTime;
    temp_user->howManyMovies = howManyMovies;
    temp_user->averageTime = averageTime;


    return temp_user;
}



//===========================================Rerun Recommender User=====================================================

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name), recIndex(0), histLength(0) {}

//Copy constructor
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser& other) : User(other), recIndex(0), histLength(0){
    recIndex = other.recIndex;
    histLength = other.histLength;
}
//Copy Assignment
RerunRecommenderUser &RerunRecommenderUser::operator=(const RerunRecommenderUser &other) {
    User::operator=(other);
    recIndex = other.recIndex;
    histLength = other.histLength;
    return *this;
}

RerunRecommenderUser::~RerunRecommenderUser()  {}

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    Watchable* rec;
    if (recIndex == 0){
        rec = history.at(0);
        recIndex++;
    }
    else {
        rec = history.at((recIndex+1)%histLength);
        recIndex = (recIndex+1)%histLength;
    }
    return rec;
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

User *RerunRecommenderUser::clone(Session &s) {
    RerunRecommenderUser* temp_user = new RerunRecommenderUser(getName());
    for (auto x: history) {
        int unsigned id = x->getId();
        temp_user->history.push_back(s.getContent().at(id));
    }
    temp_user->recIndex = recIndex;
    temp_user->histLength = histLength;

    return temp_user;
}



//==========================================Genre Recommender User=====================================================
// TODO possible to make it faster by deleting and inserting back to the multimap
// https://stackoverflow.com/questions/11343822/how-to-replace-key-value-in-a-std-multimap
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name), counterTag(), tagsMap(),
    sortedTagsMap() {}

//Copy constructor
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser& other) : User(other), counterTag(), tagsMap(),
    sortedTagsMap() {
    for (auto x : other.counterTag){
        counterTag.insert(x);
    }
    for (auto x : other.tagsMap){
        tagsMap.insert(x);
    }
    for (auto x : other.sortedTagsMap){
        sortedTagsMap.insert(x);
    }
}
//Copy Assignment
GenreRecommenderUser &GenreRecommenderUser::operator=(const GenreRecommenderUser &other) {
    User::operator = (other);
    for (auto x: other.counterTag){
        counterTag.insert(x);
    }
    for (auto x: other.tagsMap){
        tagsMap.insert(x);
    }
    for (auto x: other.sortedTagsMap){
        sortedTagsMap.insert(x);
    }
    return *this;
}

Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    // Sorting the tags by appearance
    for (auto x: counterTag) {
        tagsMap.insert(make_pair(x.second,x.first));
    }
    // Sorting the tags by lex
    for (auto x: tagsMap) {
        sortedTagsMap.insert(make_pair(x.second,x.first));
    }
    // Searching the relevant tag
    for (auto x: sortedTagsMap) {
        std::string tag = x.first;
        for (int unsigned i = 0; i < s.getContent().size(); i++) {
            if (std::find(history.begin(), history.end() , s.getContent().at(i)) == history.end()) {
                std::vector<std::string> temp_v = s.getContent().at(i)->getTags();
                for (auto str : temp_v) {
                    if (str == tag) {
                        return s.getContent().at(i);
                    }
                }
            }
        }
    }

    return nullptr;
}

void GenreRecommenderUser::updateRec(Watchable *watchable) {
    //TODO make it prettier and better

    // Updating counter for each tag in the unordered map
    for (int unsigned i = 0; i < watchable->getTags().size(); i++) {
        std::string temp_tag = watchable->getTags().at(i);
        if (counterTag.find(temp_tag) != counterTag.end()) {
            counterTag.at(temp_tag)++;
        }
        else {
            counterTag.insert(make_pair(temp_tag,1));
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

User *GenreRecommenderUser::clone(Session &s) {
    GenreRecommenderUser* temp_user = new GenreRecommenderUser(getName());
    for (auto x: history) {
        int id = x->getId();
        temp_user->history.push_back(s.getContent().at(id));
    }
    for (auto x : counterTag) {
        temp_user->counterTag.insert(x);
    }
    for (auto x : tagsMap) {
        temp_user->tagsMap.insert(x);
    }
    for (auto x : sortedTagsMap) {
        temp_user->sortedTagsMap.insert(x);
    }

    return temp_user;
}

GenreRecommenderUser::~GenreRecommenderUser() {
    tagsMap.clear();
    sortedTagsMap.clear();
    counterTag.clear();
}
