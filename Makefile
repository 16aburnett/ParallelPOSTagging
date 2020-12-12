CXX := g++
CXXFLAGS := -pthread -std=c++17 -O3

driver : driver.cpp

handin : handin.tar

handin.tar : driver.cpp parts_of_speech.hpp README Makefile Timer.hpp war_and_peace.txt dictionary.txt input10k.txt
	tar cvf $@ $^
