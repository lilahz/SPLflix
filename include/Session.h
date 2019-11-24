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
    Session(const std::string &configFilePath);
    Session& operator= (const Session& other)  ;
    Session(const Session& other);
    ~Session();
    void start();
    void addToUserMap(std::string name,User* user);
    User* findInUserMap(std::string name);
    void setActiveUser(User *user);
    void deleteFromUserMap(std::string name);
    std::vector<Watchable*> getContent();
    User* getActiveUser();
    void addToActionsLog(BaseAction* act);
    std::string getUserName();
    std::string getThirdParameter();
    std::vector<BaseAction*> getActionLog();
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string userName;
    std::string thirdParameter;
};
#endif
