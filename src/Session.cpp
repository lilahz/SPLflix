#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"
#include "../include/Action.h"

#include <vector>
#include <unordered_map>
#include <string>

#include "../include/json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

Session::Session(const std::string &configFilePath) {
    activeUser = nullptr;
    std::ifstream i(configFilePath);
    json configFile;
    i >> configFile;

    int id = 0;
    int count = 0;
    while (configFile["movies"][count] != nullptr) {
        std::string temp_name =  configFile["movies"][count]["name"];
        int temp_length = configFile["movies"][count]["length"];
        std::vector<std::string> temp_tags = configFile["movies"][count]["tags"] ;
        Movie temp_movie = Movie(id, temp_name, temp_length, temp_tags);
        count++;
        id++;
        content.push_back(&temp_movie);

    }
    count = 0;
    while (configFile["tv_series"][count] != nullptr) {
        std::string temp_name =  configFile["tv_series"][count]["name"];
        int temp_length = configFile["tv_series"][count]["episode_length"];
        std::vector<std::string> temp_tags = configFile["tv_series"][count]["tags"] ;
        for (int numOfSeason=0;configFile["tv_series"][count]["seasons"][numOfSeason]!= nullptr;numOfSeason++){
            for (int numOfEpisode=1;numOfEpisode<=configFile["tv_series"][count]["seasons"][numOfSeason];numOfEpisode++){
                Episode temp_Episode = Episode(id, temp_name, temp_length,numOfSeason+1,numOfEpisode, temp_tags);
                id++;
                content.push_back(&temp_Episode);
                cout << temp_Episode.toString() + '\n';
            }
        }

        count++;


    }


}

//      ~Session() {
//
//      }
//
//        void start() {
//
//        }




