#include <iostream>
#include "../include/Session.h"
#include "../include/Watchable.h"

using namespace std;

int main(int argc, char** argv){
	if(argc!=2)
	{
		cout << "usage splflix input_file" << endl;

		return 0;
	}

    // Copy Constructor test
//    Session* s = new Session(argv[1]);
//	s->start();
//	delete(s);
//    Session s2= *s;
//    delete(s);
//    s2.start();



//    // Copy Assignment test
//    Session s(argv[1]);
//	  s.start();
//    Session s2(argv[1]);
//    s2 = s;
//    s2.start();
//    return 0;


//  Move constructor
//	Session s2 = std::move(s);
//	Session s3(argv[1]);

//  Move Assignment
//	s3 = std::move(s2);
//	s3.start();

// user copy con
//   GenreRecommenderUser *s=new GenreRecommenderUser("ofeer");
//	s->addToHistory(new Movie(1 , "ani" , 120 , {"dieeeee"}));
//	GenreRecommenderUser x = *s;
//	cout << x.getName();
//	cout << x.getHistory().at(0)->toString();
//	delete(s);

	// user copy assignment
	GenreRecommenderUser lil("lilah");
	GenreRecommenderUser ofeer("ofeer");
	lil.addToHistory(new Movie(5, "harry", 127, {"potter"}));
	ofeer = lil;
	cout << ofeer.getName() << endl;
	cout << ofeer.getHistory().at(0)->toString() << endl;



}
