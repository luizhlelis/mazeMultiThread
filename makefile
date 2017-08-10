all: maze

maze: maze.cpp
	g++ -pthread -std=c++11 maze.cpp -o maze