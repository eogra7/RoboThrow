sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)
CPPFLAGS = -std=c++11

runRoboThrow: $(objects)
	g++ $(CPPFLAGS) -o $@ $^ 

.PHONY: clean
clean:
	$(RM) runRoboThrow $(objects)
