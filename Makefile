CXX=g++-14
CXXFLAGS=-g -Wall -Wextra

mbr.exe: mbr.o
	$(CXX) $(CXXFLAGS) -o $@ $^

mbr.o: mbr.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
