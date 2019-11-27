#include "../include/Watchable.h"
#include "../include/Session.h"

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) : Watchable(id,length,tags), seriesName(seriesName),
                 season(season), episode(episode), nextEpisodeId(id+1), eps(true) {}

std::string Episode::toString() const {
    std::string seasonString =std::to_string(season);
    std::string episodeString=std::to_string(episode);
    if (season<10)
         seasonString= "0"+std::to_string(season);
    if (episode<10)
         episodeString="0"+std::to_string(episode);
    return seriesName+" "+"S"+seasonString+"E"+episodeString;

}

Watchable *Episode::getNextWatchable(Session &s) const {
    // If it's an episode and not the last episode of the series
    if (seriesName ==  s.getContent().at(nextEpisodeId)->getName()) {
        return s.getContent().at(nextEpisodeId);
    }
    else {
        return s.getActiveUser()->getRecommendation(s);
    }
    return nullptr;
}

bool Episode::isEpisode(){
     return eps;
}

std::string Episode::getName(){
    return seriesName;

}

Watchable *Episode::clone() const {
    Episode* temp = new Episode(*this);
    return temp;
}
