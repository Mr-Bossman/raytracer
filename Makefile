CC=gcc
CXX=g++
CXXFLAGS= -g -Wall -fopenmp -std=c++20 -O3 

LIBS=-lpthread -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer 
TARGET = tracer

SRCS= $(shell find . -name "*.cpp")
OBJ=$(subst .cpp,.o,$(SRCS))

all: clean $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	
run: all 
	./tracer

.PHONY: clean

clean:
	rm -f *.o *~ $(TARGET) 