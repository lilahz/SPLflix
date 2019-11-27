#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"
#include "../include/Action.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

using namespace std;
using json = nlohmann::json;

// Session Constructor
Session::Session(const std::string &configFilePath) {
    activeUser = nullptr;  // TODO check if still needed to initialize
    std::ifstream i(configFilePath);
    json configFile;
    i >> configFile;

    int id = 0;
    int count = 0;
    while (configFile["movies"][count] != nullptr) {
        std::string temp_name =  configFile["movies"][count]["name"];
        int temp_length = configFile["movies"][count]["length"];
        std::vector<std::string> temp_tags = configFile["movies"][count]["tags"] ;
        Movie* temp_movie = new Movie(id, temp_name, temp_length, temp_tags);
        count++;
        id++;
        content.push_back(temp_movie);
    }
    count = 0;
    while (configFile["tv_series"][count] != nullptr) {
        std::string temp_name =  configFile["tv_series"][count]["name"];
        int temp_length = configFile["tv_series"][count]["episode_length"];
        std::vector<std::string> temp_tags = configFile["tv_series"][count]["tags"] ;
        for (int numOfSeason=0;configFile["tv_series"][count]["seasons"][numOfSeason]!= nullptr;numOfSeason++){
            for (int numOfEpisode=1;numOfEpisode<=configFile["tv_series"][count]["seasons"][numOfSeason];numOfEpisode++){
                Episode* temp_Episode = new Episode(id, temp_name, temp_length,numOfSeason+1,numOfEpisode, temp_tags);
                id++;
                content.push_back(temp_Episode);
            }
        }
        count++;
    }
}

//===================================================Rule of Five=======================================================

/*
 * Copy Constructor.
 * Copy all the values
 */
Session::Session(const Session& other) {
    std::string userName = other.activeUser->getName();
    for (auto *x: other.content){
        content.emplace_back(x->clone());
    }
    for (auto *x: other.actionsLog){
        actionsLog.emplace_back(x->clone());
    }
    for (auto user = other.userMap.begin(); user != other.userMap.end(); ++user){
        userMap.insert({user->first, user->second->clone(*this)});
    }
    activeUser = findInUserMap(userName);
}

/*
 * Move Constructor.
 * Moves all pointers from the old Session to point on this Session information, then delete the pointers to the
 * old Session
 */
Session::Session(Session &&other):content(other.content) , actionsLog(other.actionsLog) , userMap(other.userMap)
, activeUser(other.activeUser){
    //Delete old Session
    other.content.clear();
    other.actionsLog.clear();
    other.userMap.clear();
    other.activeUser=nullptr ;
}

/*
 * Copy Assignment Operator.
 * Overrides all the information of the other Session with this Session's information.
 */
Session& Session::operator= (const Session& other)  {
    if ( this != &other )
    {
        std:string userName = other.activeUser->getName();
        // Delete and create new contentList
        for (Watchable* w: content){
            delete w;
            w = nullptr;
        }
        content.clear();
        for (int i = 0; i < other.content.size(); i++){
            content.push_back(other.content[i] -> clone());
        }
        // Delete and create new actionsLog
        for (BaseAction* b: actionsLog) {
            delete b;
            b = nullptr;
        }
        actionsLog.clear();
        for (int i = 0; i < other.actionsLog.size(); i++){
            actionsLog.push_back(other.actionsLog[i] -> clone());
        }
        // Delete and create new userMap
        for (auto it = userMap.begin(); it != userMap.end(); ++it) {
            delete it->second;
            userMap.erase(it->first);
        }
        userMap.clear();
        for (auto user = other.userMap.begin(); user != other.userMap.end(); ++user){
            userMap.insert({user->first, user->second->clone(*this)});
        }
        // Delete and create new activeUser
        delete activeUser;
        activeUser = findInUserMap(userName);
    }
}

/* Move Assignment Operator
 * Moves the pointers from the other Session to this Session, and delete all values saved in the other Session.
 */
Session& Session::operator=(Session &&other){
    if ( this != &other ){

        //Delete actionslog
        for (BaseAction* b: actionsLog) {
            delete b;
            b = nullptr;
        }
        actionsLog.clear();
        //Delete userMap
        for (auto it = userMap.begin(); it != userMap.end(); ++it) {
            delete it->second;
            userMap.erase(it->first);
        }
        userMap.clear();
        // Delete contentList
        for (Watchable* w: content){
            delete w;
            w = nullptr;
        }
        content.clear();
        activeUser= nullptr;

        // Move pointers
        for (int i = 0; i < other.content.size(); i++){
            content.push_back(other.content[i]);
            other.content[i] = nullptr;
        }
        for (int i = 0; i < other.actionsLog.size(); i++){
            actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i] = nullptr;
        }
        for (auto user = other.userMap.begin(); user != other.userMap.end(); ++user){
            userMap.insert({user->first, user->second});
            other.userMap.at(user->first) = nullptr;
        }
        activeUser=other.activeUser;
        other.activeUser = nullptr;




    }
    return *this;
}


/*
 * Session Destructor.
 * Deletes content, actionLog and userMap.
 */
Session::~Session() {
     for (int i=0; i<content.size();i++){
         delete content[i];
         content[i]= nullptr;
     }
     content.clear();
     for (int i=0; i<actionsLog.size();i++) {
        delete actionsLog[i];
        actionsLog[i]= nullptr;
     }
     actionsLog.clear();
     for (auto it = userMap.begin(); it != userMap.end(); ++it) {
         delete it->second;
     }
     userMap.clear();
     activeUser= nullptr;

}

/*
 * Starts the program's main loop and calls all the actions by the
 * given command.
 */
void Session::start() {
    cout << "SPLFLIX is now on!" << '\n';
    if (findInUserMap("default") == nullptr) {
        User *defaultUser = new LengthRecommenderUser("default");
        addToUserMap("default", defaultUser);
        setActiveUser(defaultUser);
    }
    std::string command;
    BaseAction* action;
    while (command != "exit") {
        cin >> command;
        if (command == "createuser") {
            cin >> secondParameter; //userName
            cin >> thirdParameter; //algo
            action = new CreateUser;
            action->act(*this);
        } else if (command == "changeuser") {
            cin >> secondParameter; //userName
            action = new ChangeActiveUser;
            action->act(*this);
        } else if (command == "delete") {
            cin >> secondParameter; //userName
            action = new DeleteUser;
            action->act(*this);
        } else if (command == "dupuser") {
            cin >> secondParameter; //old userName
            cin >> thirdParameter; //new userName
            action = new DuplicateUser;
            action->act(*this);
        } else if (command == "content") {
            action = new PrintContentList;
            action->act(*this);
        } else if (command == "watchhist") {
            action = new PrintWatchHistory;
            action->act(*this);
        } else if (command == "watch") {

            cin >> secondParameter; // content id
            std::string answer="y";
            while (answer == "y"){
                action = new Watch;
                action->act(*this);
                Watchable* watched = content.at(std::stoi(secondParameter) - 1);
                watched = watched->getNextWatchable(*this);
                secondParameter=(std::to_string(watched->getId()+1));
                std::cout << "We recommend watching " ;
                std::cout << watched->toString();
                std::cout << ", continue watching? [y/n]" << std::endl;
                std::cin >> answer;

            }


        } else if (command == "log") {
            action = new PrintActionsLog;
            action->act(*this);
        } else if (command == "exit") {
            action = new Exit;
            action->act(*this);
            command = "exit";
        }
    }
}

//===============================================Our Own Functions======================================================

/*
 * Addes users to userMap vector.
 * @param name the name of the user to add to the map.
 * @param user pointer to the user to add.
 */
void Session::addToUserMap(std::string name, User* user) {
    std::pair<std::string, User*> temp_pair(name,user);
    userMap.insert(temp_pair);
}

/*
 * Changes the active user to the given user
 *
 * @param user pointer to the user which we want to make active user
 */
void Session::setActiveUser(User* user) {
    activeUser = user;
}

/*
 * Delete user from userMap vector.
 *
 * @param name the name of the user to delete.
 */
void Session::deleteFromUserMap(std::string name) {
    delete findInUserMap(name);
    userMap.erase(name);
}

/*
 * Returns content.
 *
 * @return content vector.
 */
std::vector<Watchable *> Session::getContent() {
    return content;
}

/*
 * Returns the currently active user.
 *
 * @return pointer to the active user.
 */
User *Session::getActiveUser() {
    return activeUser;
}

/*
 * Find in the userMap the user by name.
 *
 * @param name of the user to find.
 * @return pointer to the wanted user. If user not found return nullptr
 */
User *Session::findInUserMap(std::string name) {
    for (auto x: userMap) {
        if (x.first == name) {
            return x.second;
        }
    }
    return nullptr;
}

/*
 * Add new action to the actionsLog vector.
 *
 * @param pointer to action to add to the vector.
 */
void Session::addToActionsLog(BaseAction* act){
    actionsLog.push_back(act);
}

/*
 * Get user name.
 *
 * @return string of the user's name.
 */
std::string Session::getSecondParameter() {
    return secondParameter;
}

/*
 * Return the third parameter received by the cin.
 *
 * @return string of the third parameter entered by the console.
 */
std::string Session::getThirdParameter() {
    return thirdParameter;
}

/*
 * Get actionsLog vector.
 *
 * @return actionsLog vector.
 */
std::vector<BaseAction *> Session::getActionLog() {
    return actionsLog;
}






