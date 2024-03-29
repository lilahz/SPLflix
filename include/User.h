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
    User(const User& other); // Copy Constructor
    User& operator= (const User& other); // Copy Assignment
    virtual ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> getHistory();
    virtual void setHistory(std::vector<Watchable*> historyToCopy);
    virtual void addToHistory(Watchable* watchable);
    virtual void updateRec(Watchable* watchable);
    virtual User* duplicateUser(const std::string& name) = 0;
    virtual User* clone(Session &s) = 0;
protected:
    std::vector<Watchable*> history;
private:
    std::string name;
};

//===========================================LengthRecommenderUser.h====================================================

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    LengthRecommenderUser(const LengthRecommenderUser& other);
    LengthRecommenderUser& operator= (const LengthRecommenderUser& other);
     ~LengthRecommenderUser();
    virtual void setHistory(std::vector<Watchable*> historyToCopy);
    virtual Watchable* getRecommendation(Session& s);
    virtual void updateRec(Watchable* watchable);
    User* duplicateUser(const std::string& name) override;
    virtual User* clone(Session &s);

private:
    int totalTime;
    int howManyMovies;
    int averageTime;
};

//============================================RerunRecommenderUser.h====================================================


class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const RerunRecommenderUser& other);
    RerunRecommenderUser& operator= (const RerunRecommenderUser& other);
     ~RerunRecommenderUser();
    virtual Watchable* getRecommendation(Session& s);
    virtual void updateRec(Watchable* watchable);
    User* duplicateUser(const std::string& name) override;
    virtual User* clone(Session &s);
private:
    int recIndex;
    int histLength;
};

//============================================GenreRecommenderUser.h====================================================

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    GenreRecommenderUser(const GenreRecommenderUser& other);
    GenreRecommenderUser& operator= (const GenreRecommenderUser& other);
    ~GenreRecommenderUser();
    virtual Watchable* getRecommendation(Session& s);
    virtual void updateRec(Watchable* watchable);
    User* duplicateUser(const std::string& name) override;
    virtual User* clone(Session &s);
private:
    std::unordered_map<std::string, int> counterTag;
    std::multimap<int, std::string> tagsMap;
    std::multimap<std::string, int> sortedTagsMap;
};

#endif
