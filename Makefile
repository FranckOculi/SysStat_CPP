CXX = g++
CFLAGS ?= -Wall -Wextra -I include

all: sys-stat print-stat client-test

sys-stat: main.o system.o server.o common.o
	$(CXX) $(CFLAGS) -o $@ $^

print-stat: printstat.o system.o common.o
	$(CXX) $(CFLAGS) -o $@ $^

client-test: client_test.o system.o common.o
	$(CXX) $(CFLAGS) -o $@ $^

%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o

.PHONY: all clean