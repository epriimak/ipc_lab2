CC=g++
CFLAGS=-Wall -Werror
LIBS=-lpthread -lrt
GRAPHLIB= -lglut -lGLU -lGL -lrt -IGLEW

HOST_SOURCE=main.cpp client.h host.cpp
CONN_SOURCES=$(shell find . -name "conn_*.cpp")
EXECUTABLES=$(CONN_SOURCES:./conn_%.cpp=host_%)

all: $(EXECUTABLES)
	
host_%: conn_%.o $(HOST_SOURCE:.cpp=.o)
	$(CC) -o $@ $^ $(LIBS) $(GRAPHLIB)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f `find . -maxdepth 1 -executable -type f`