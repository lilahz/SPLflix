#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>

class Watchable;
class Session;

//==================================================User.h==============================================================

class User{
public:
    User(const std::string& name);
    User& operator= (const User& other)  ;
    User(const User& other);
    ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_notSeen();
    virtual std::string getAlgo();
    void setAlgo(std::string algo);
    std::vector<Watchable*> getHistory();
    virtual void setHistory(std::vector<Watchable*> historyToCopy);
    virtual void addToHistory(Watchable* watchable);
    void setNotSeen(std::vector<Watchable*> notSeenToCopy);
    void removeFromNotSeen(Watchable* watchToDelete, int id);
    virtual void updateRec(Watchable* watchable) ;
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;
    std::vector<Watchable*> notSeen;
    std::string userAlgo;
};

//===========================================LengthRecommenderUser.h====================================================

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual void setHistory(std::vector<Watchable*> historyToCopy);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo();
    virtual void updateRec(Watchable* watchable);
private:
    int totalTime;
    int howManyMovies;
    int averageTime;
};

//============================================RerunRecommenderUser.h====================================================


class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo();
    virtual void updateRec(Watchable* watchable);
private:
    int watchIndex;
    int histLength;
};

//============================================GenreRecommenderUser.h====================================================

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo() ;
    virtual void updateRec(Watchable* watchable);
private:
    std::unordered_map<std::string, int> counterTag;
    std::multimap<int, std::string> tagsMap;
    std::multimap<std::string, int> sortedTagsMap;
};

#endif
