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
    void addToUserMap(std::string &name,User* user);
    void setActiveUser(User *user);
    void deleteFromUserMap(std::string name);
    std::vector<Watchable*> getContent();
    User* getActiveUser();
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;


};
#endif
