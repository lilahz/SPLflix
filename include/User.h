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
//    User& operator= (const User& other); // Copy Constructor
//    User(const User& other);
    virtual ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> getHistory();
    virtual void setHistory(std::vector<Watchable*> historyToCopy);
    virtual void addToHistory(Watchable* watchable);
    virtual void updateRec(Watchable* watchable);
    virtual User* duplicateUser(const std::string& name) = 0;
    virtual User* clone() const = 0;
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;
};

//===========================================LengthRecommenderUser.h====================================================

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual void setHistory(std::vector<Watchable*> historyToCopy);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo() const;
    virtual void updateRec(Watchable* watchable);
    User* duplicateUser(const std::string& name) override;
    virtual User* clone() const;
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
    virtual std::string getAlgo() const;
    virtual void updateRec(Watchable* watchable);
    User* duplicateUser(const std::string& name) override;
    virtual User* clone() const;
private:
    int recIndex;
    int histLength;
};

//============================================GenreRecommenderUser.h====================================================

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo() const;
    virtual void updateRec(Watchable* watchable);
    User* duplicateUser(const std::string& name) override;
    virtual User* clone() const;
private:
    std::unordered_map<std::string, int> counterTag;
    std::multimap<int, std::string> tagsMap;
    std::multimap<std::string, int> sortedTagsMap;
};

#endif
