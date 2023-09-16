all: myfind 

myfind: myfind.cpp
	g++ -std=c++14 -Wall -Werror -o getopt myfind.cpp



