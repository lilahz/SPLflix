#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    User& operator= (const User& other)  ;
    User(const User& other);
    ~User();
//    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    std::vector<Watchable*>* get_notSeen();
    virtual std::string getAlgo();
    void setAlgo(std::string algo);
    std::vector<Watchable*> getHistory();
    void setHistory(std::vector<Watchable*> historyToCopy);
    std::vector<Watchable*> getNotSeen();
    void setNotSeen(std::vector<Watchable*> notSeenToCopy);
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;
    std::vector<Watchable*> notSeen;
    std::string userAlgo;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo();
private:
    int totalTime;
    int howManyMovies;
    int averageTime;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo();
private:
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo() ;
private:
};

#endif
