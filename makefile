# All Targets
all: SPLflix

# Tool invocations
SPLflix: bin/Main.o bin/Action.o bin/User.o bin/Session.o bin/Watchable.o bin/Episode.o bin/Movie.o
	g++ -o bin/SPLflix bin/Main.o bin/Action.o bin/User.o bin/Session.o bin/Watchable.o bin/Episode.o bin/Movie.o

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files
bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

# Depends on the source and header files
bin/User.o: src/User.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp

# Depends on the source and header files
bin/Session.o: src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp

# Depends on the source and header files
bin/Watchable.o: src/Watchable.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp

# Depends on the source and header files
bin/Episode.o: src/Episode.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Episode.o src/Episode.cpp

# Depends on the source and header files
bin/Movie.o: src/Movie.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Movie.o src/Movie.cpp

#Clean the build directory
clean:
	rm -f bin/*
