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
 * Copy all the
 */
Session::Session(const Session& other) {
    activeUser = other.activeUser -> clone();
    for (int i = 0; i < other.content.size(); i++){
        content.push_back(other.content.at(i) -> clone());
    }
    for (int i = 0; i < other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog.at(i) -> clone());
    }
    for (auto user = other.userMap.begin(); user != other.userMap.end(); ++user){
        userMap.insert({user->first, user->second->clone()});
    }
}

/*
 * Move Constructor.
 * Moves all pointers from the old Session to point on this Session information, then delete the pointers.
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
        // Delete and create new activeUser
        delete activeUser;
        activeUser = other.activeUser->clone();

        // Delete and create new contentList
        for (Watchable* w: content){
            delete w;
        }
        content.clear();
        for (int i = 0; i < other.content.size(); i++){
            content.push_back(other.content.at(i) -> clone());
        }

        // Delete and create new actionsLog
        for (BaseAction* b: actionsLog) {
            delete b;
        }
        actionsLog.clear();
        for (int i = 0; i < other.actionsLog.size(); i++){
            actionsLog.push_back(other.actionsLog.at(i) -> clone());
        }

        // Delete and create new userMap
        for (auto it = userMap.begin(); it != userMap.end(); ++it) {
            delete it->second;
        }
        userMap.clear();
        for (auto user = other.userMap.begin(); user != other.userMap.end(); ++user){
            userMap.insert({user->first, user->second->clone()});
        }
    }
}

/* Move Assignment Operator
 * Moves the pointers from the other Session to this Session, and delete all values saved in the other Session.
 */
Session& Session::operator=(Session &&other){
    if ( this != &other ){
        // Move pointers
        content=other.content;
        actionsLog=other.actionsLog;
        userMap=other.userMap;
        activeUser=other.activeUser;
        // Delete old Session
        for (Watchable* w: other.content){
            delete w;
        }
        other.content.clear();
        for (BaseAction* b: other.actionsLog) {
            delete b;
        }
        other.actionsLog.clear();
        for (auto it = other.userMap.begin(); it != other.userMap.end(); ++it) {
            delete it->second;
        }
        other.userMap.clear();
        other.activeUser= nullptr;
    }
}


/*
 * Session Destructor.
 * Deletes content, actionLog and userMap.
 */
Session::~Session() {
     for (Watchable* w: content){
         delete w;
     }
     content.clear();
     for (BaseAction* b: actionsLog) {
         delete b;
     }
     actionsLog.clear();
     for (auto it = userMap.begin(); it != userMap.end(); ++it) {
         delete it->second;
     }
     userMap.clear();
}

/*
 * Starts the program's main loop and calls all the actions by the
 * given command.
 */
void Session::start() {
    cout << "SPLFLIX is now on!" << '\n';
    User* defaultUser = new LengthRecommenderUser("default");
    addToUserMap("default", defaultUser);
    setActiveUser(defaultUser);

    std::string command;
    BaseAction* action;
    while (command != "exit") {
        cin >> command;
        if (command == "createuser") {
            cin >> userName;
            cin >> thirdParameter;
            action = new CreateUser;
            action->act(*this);
        } else if (command == "changeuser") {
            cin >> userName;
            action = new ChangeActiveUser;
            action->act(*this);
        } else if (command == "delete") {
            cin >> userName;
            action = new DeleteUser; // does this works?
            action->act(*this);
        } else if (command == "dupuser") {
            cin >> userName;
            cin >> thirdParameter;
            action = new DuplicateUser;
            action->act(*this);
        } else if (command == "content") {
            action = new PrintContentList;
            action->act(*this);
        } else if (command == "watchhist") {
            action = new PrintWatchHistory;
            action->act(*this);
        } else if (command == "watch") {
            action = new Watch;
            action->act(*this);
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
std::string Session::getUserName() {
    return userName;
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







