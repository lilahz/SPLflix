#include <iostream>
#include <fstream>
#include "../include/Session.h"
#include "../include/json.hpp"

using namespace std;
using json = nlohmann::json;

/* Function to Read given Json file and add information to contect vector
 *
 * @param json file
 */
void readAndCreate(json& j){
    for (json::iterator it = j.begin(); it != j.end(); ++it){
        std::cout << *it << '\n';
    }
}

int main(int argc, char** argv){
//	if(argc!=2)
//	{
//		cout << "usage splflix input_file" << endl;
//		return 0;
//	}
//	Session s(argv[1]);
//	s.start();

    std::ifstream i(argv[1], std::ifstream::binary);
    //std::ifstream i("config1.json");
    json j;
    i >> j;
    readAndCreate(j);
	return 0;
}

