CXX=g++
CXXFLAGS=-g -Wall -Wextra

hull.exe: hull.o
	$(CXX) $(CXXFLAGS) -o $@ $^

hull.o: hull.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
