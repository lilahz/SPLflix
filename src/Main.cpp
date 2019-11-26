#include <iostream>
#include "../include/Session.h"

using namespace std;

int main(int argc, char** argv){
	if(argc!=2)
	{
		cout << "usage splflix input_file" << endl;

		return 0;
	}

    // Copy Constructor test
//    Session* s = new Session(argv[1]);
//    s->start();
//    Session s2= *s;
//    delete(s);
//    s2.start();


    // Copy Assignment test
    Session s(argv[1]);
    Session s2(argv[1]);
    s.start();
    s2 = s;
    s2.start();
    return 0;



//    Session s(argv[1]);
//	s.start();
//
//	// Copy Constructor
//	Session s2 = s;
//	s2.start();

//
//	Session s2(argv[1]);
//    s2=s;
//    s2.start();

//  Move constructor
//	Session s2 = std::move(s);
//	Session s3(argv[1]);

//  Move Assignment
//	s3 = std::move(s2);
//	s3.start();
}
