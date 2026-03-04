CXX = g++
CFLAGS ?= -Wall -Wextra -I include

all: systat printstat

systat: main.o system.o network.o common.o
	$(CXX) $(CFLAGS) -o $@ $^

printstat: printstat.o system.o network.o common.o
	$(CXX) $(CFLAGS) -o $@ $^

%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o

.PHONY: all clean