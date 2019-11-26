#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/Session.h"

#include <algorithm>


using namespace std;

// User Constructor
User::User(const std::string &name):name(name) {}

//===============================================Rule of Three==========================================================

 //Copy Constructor
User::User(const User &other) : name(other.name) {
     for (int i = 0; i < other.history.size(); i++){
         Watchable* temp_watch = other.history[i] ;
         history.push_back(temp_watch);
     }
}

//copy Assignment
User &User::operator=(const User &other) {
    if (this != &other)
    {
        name = other.name;
        // Delete and create new history
        for (auto x: history) {
          delete x;
        }
        history.clear();
        for (int i = 0; i < other.history.size(); i++){
            Watchable* temp = other.history[i];
            history.push_back(temp);
        }
    }
}

User::~User() {
    for (auto x: history){
       x = nullptr;
    }
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

void User::addToHistory(Watchable *watchable) {
    history.push_back(watchable);
}

void User::updateRec(Watchable *watchable) {}


//==========================================Length Recommender User=====================================================

LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {
    totalTime=0;
    averageTime=0;
    howManyMovies=0;
}

LengthRecommenderUser::~LengthRecommenderUser() {}

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

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) {
    recIndex = 0;
    histLength = 0;
}

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
        int id = x->getId();
        temp_user->history.push_back(s.getContent().at(id));
    }
    temp_user->recIndex = recIndex;
    temp_user->histLength = histLength;

    return temp_user;
}

RerunRecommenderUser::~RerunRecommenderUser() {

}

//==========================================Genre Recommender User=====================================================
// TODO possible to make it faster by deleting and inserting back to the multimap
// https://stackoverflow.com/questions/11343822/how-to-replace-key-value-in-a-std-multimap
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) {}

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

}
