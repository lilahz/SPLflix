#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"
#include "../include/Action.h"

#include <vector>
#include <unordered_map>
#include <string>

#include "../include/json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

Session::Session(const std::string &configFilePath) {
    // TODO initialize actionLog and userMap
    activeUser = nullptr;
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
                Episode* temp_Episode =new Episode(id, temp_name, temp_length,numOfSeason+1,numOfEpisode, temp_tags);
                id++;
                content.push_back(temp_Episode);
                // TODO if we keep the last Ep of the Last Season for recomndaition

            }
        }

        count++;
    }

    }

//copy assignment
Session& Session::operator= (const Session& other)  {
    if ( this != &other )
    {
        content = other.content;
        actionsLog = other.actionsLog;
        activeUser = other.activeUser;
        userMap = other.userMap;
    }
}

// copy constructor
Session::Session(const Session& other) : activeUser(other.activeUser)  {
    std::vector<Watchable*> content(other.content.begin(), other.content.end());
    std::vector<BaseAction*> actionsLog(other.actionsLog.begin(), other.actionsLog.end());
    std::unordered_map<std::string,User*> userMap(other.userMap.begin(), other.userMap.end());

}

Session::~Session() {
     for (Watchable* w: content){
         delete w;
     }
     content.clear();
     for (BaseAction* b: actionsLog) {
         delete b;
     }
     actionsLog.clear();
     userMap.erase(userMap.begin(), userMap.end());
     delete activeUser;
}

void Session::start() {
    cout << "SPLFLIX is now on!" << '\n';
    User* defaultUser = new LengthRecommenderUser("default");
    setActiveUser(defaultUser);

    std::string command;
    std::string name;
    std::string thirdString;
    while (command != "exit") {
        cin >> command;
        if (command == "createuser") {
            cin >> name;
            cin >> thirdString;
            if ((thirdString != "len") & (thirdString != "rer") & (thirdString != "gen")) {
                cout << " ERROR: Invalid algorithm input." << '\n';
            }
            else if (userMap.find(name) != userMap.end())
            {
                //TODO send to error and change status/.
                cout << " ERROR: User already exist ."<< '\n';
            }
            else {
                BaseAction* create = new CreateUser(name, thirdString);
                // TODO actionlog add action
                create->act(*this);

            }
        }
        if (command == "changeuser")
        {
            cin >> name;
            std::unordered_map<std::string,User*>::const_iterator temp_user=userMap.find(name);

            if (temp_user == userMap.end())
                cout << "ERROR: user does not exist."<< '\n';
            else{
                BaseAction* change= new ChangeActiveUser(temp_user->second); // does this works?
                change->act(*this);
            }

        }
        if (command == "delete"){
            cin >> name;
            std::unordered_map<std::string,User*>::const_iterator temp_user=userMap.find(name);

            if (temp_user == userMap.end())
                cout << "ERROR: user does not exist."<< '\n';
            else{
                BaseAction* deleteUser= new DeleteUser(temp_user->second); // does this works?
                deleteUser->act(*this);
            }
        }
        if (command == "dupuser"){
            cin>> name;
            cin>> thirdString;
            std::unordered_map<std::string,User*>::const_iterator temp_olduser=userMap.find(name);
            std::unordered_map<std::string,User*>::const_iterator temp_newuser=userMap.find(thirdString);
            if (temp_olduser == userMap.end())
                cout << "ERROR: user does not exist."<< '\n';
            if (temp_newuser != userMap.end())
                cout << "ERROR: user name already taken" << '\n';
            BaseAction* duplicateUser = new DuplicateUser(temp_olduser->second , thirdString);
            duplicateUser->act(*this );

        }
        if (command == "content")
        {

            BaseAction* printContent= new PrintContentList();
            printContent->act(*this);
        }
        if (command == "watchhist")
        {
            BaseAction* printWatchHistory = new PrintWatchHistory();
            printWatchHistory->act(*this);
        }
        if (command == "watch")
        {

            BaseAction* watch = new Watch();
            watch->act(*this);

        }


    }

}

void Session::addToUserMap(std::string &name, User* user) {
    std::pair<std::string, User*> temp_pair(name,user);
    userMap.insert(temp_pair);
}
void Session::setActiveUser(User* user){
    activeUser = user;
}
void Session::deleteFromUserMap(std::string name) {
    userMap.erase(name);
}

std::vector<Watchable *> Session::getContent() {
    return content;
}

User *Session::getAcitveUser() {
    return activeUser;
}







