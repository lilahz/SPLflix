#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"


class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath); // Session Constructor
    Session(const Session& other); // Copy Constructor
    Session(Session &&other); // Move Constructor
    Session& operator= (const Session& other); // Copy Assignment Operator
    Session& operator=(Session &&other); // Move Assignment Operator
    ~Session(); // Destructor
    void start();
    // Our own functions
    void addToUserMap(std::string name,User* user);
    User* findInUserMap(std::string name);
    void setActiveUser(User *user);
    void deleteFromUserMap(std::string name);
    std::vector<Watchable*> getContent();
    User* getActiveUser();
    void addToActionsLog(BaseAction* act);
    std::string getSecondParameter();
    std::string getThirdParameter();
    std::vector<BaseAction*> getActionLog();


private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    // Our own fields
    std::string secondParameter;
    std::string thirdParameter;


};
#endif
