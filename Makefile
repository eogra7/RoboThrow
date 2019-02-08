sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)
CPPFLAGS = -g -std=c++0x -pthread

runRoboThrow: $(objects)
	g++ $(CPPFLAGS) -o $@ $^ 

.PHONY: clean
clean:
	$(RM) runRoboThrow $(objects)
