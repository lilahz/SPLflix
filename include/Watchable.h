#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, std::vector<std::string>  tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    int getLength();
    int getId();
    std::string tagsToString();
    std::vector<std::string> getTags();
    virtual std::string getSeriesName();
    virtual Watchable* clone() const = 0;
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual Watchable* clone() const;
private:
    std::string name;
    bool mov;

};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual bool isEpisode();
    virtual std::string getSeriesName();
    virtual Watchable* clone() const;
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
    bool eps;

};

#endif
