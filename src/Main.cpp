#include <iostream>
#include "../include/Session.h"

using namespace std;

int main(int argc, char** argv){
	if(argc!=2)
	{
		cout << "usage splflix input_file" << endl;

		return 0;
	}
	Session s(argv[1]);
	s.start();

//	Session s2("../config2.json");
//	s2.start();
//
	Session s2 = std::move(s);
	Session s3(argv[1]);
	s3 = std::move(s2);
	s3.start();
}
