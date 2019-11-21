//
// Created by lilachzi@wincs.cs.bgu.ac.il on 20/11/2019.
//

#include "../include/Watchable.h"
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags):Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName) {}

std::string Episode::toString() const {
    std::string seasonString =std::to_string(season);
    std::string episodeString=std::to_string(episode);
    if (season<10)
         seasonString= "0"+std::to_string(season);
    if (episode<10)
         episodeString="0"+std::to_string(episode);
    return seriesName+" "+"S"+seasonString+"E"+episodeString;

}

//Watchable *Episode::getNextWatchable(Session &) const {
//    return nullptr;
//}

