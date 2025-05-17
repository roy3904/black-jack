CXX = g++
CXXFLAGS = -Wall

blackjack: blackjack.o main.cpp
		$(CXX) $(CXXFLAGS) blackjack.o main.cpp -o blackjack

test: blackjack.o test.cpp
		$(CXX) $(CXXFLAGS) blackjack.o test.cpp -o test

blackjack.o: blackjack.cpp blackjack.h
		$(CXX) $(CXXFLAGS) -c blackjack.cpp