all:
	g++ main.cpp parser.cpp rule.cpp token.cpp transition.cpp -std=c++11 -static -Wall -pedantic -O3

debug:
		g++ main.cpp parser.cpp rule.cpp token.cpp transition.cpp -std=c++11 -g

teste:
	./a.out 1> out 2> err