CXX = g++
CFLAGS ?= -Wall -Wextra -I include

all: systat printstat

systat: main.o system.o server.o common.o
	$(CXX) $(CFLAGS) -o $@ $^

printstat: printstat.o system.o common.o
	$(CXX) $(CFLAGS) -o $@ $^

client_test: client_test.o socket.o system.o common.o
	$(CXX) $(CFLAGS) -o $@ $^

%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o

.PHONY: all clean