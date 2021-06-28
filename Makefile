FUT=futhark
FUT_CC=nvcc #gcc for opencl, nvcc for cuda
FUTFLAGS=cuda #opencl for opencl, cuda for cuda
FUT_CC_FLAGS= -g  -O3 -shared 

CC=gcc
CXX=g++
CXXFLAGS= -g -Wall -fopenmp -std=c++20 -O3 
CFLAGS= -g -Wall -O3 -fPIC -shared

LIBS=-lpthread -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lgomp -lm -lOpenCL  -lnvrtc -lcuda -lcudart #-lnvrtc -lcuda -lcudart for cuda -lOpenCL for opencl
TARGET = tracer

OBJ_DIR=./bin
SRC_DIR=./src

CXX_SRCS= $(shell find $(SRC_DIR) -name "*.cpp" -not -path "./futhark/*")
CXX_OBJ=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CXX_SRCS))

CC_SRCS=$(filter-out  $(FUT_OBJ),$(shell find $(SRC_DIR) -name "*.c" -not -path "./futhark/*"))
CC_OBJ=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CC_SRCS))


FUT_SRC_DIR=$(SRC_DIR)/futhark
FUT_SRC=$(FUT_SRC_DIR)/ray.fut
FUT_INTERM=$(patsubst $(FUT_SRC_DIR)/%.fut,$(OBJ_DIR)/%.c,$(FUT_SRC))
FUT_HEADER=$(subst .c,.h,$(FUT_INTERM))
FUT_OBJ=$(subst .c,.o,$(FUT_INTERM))


all:
	$(MAKE) clean
	$(MAKE) $(TARGET)


$(FUT_INTERM): $(FUT_SRC)
	$(FUT) $(FUTFLAGS) --library $< -o $(subst .c,,$(FUT_INTERM))

$(FUT_OBJ):$(FUT_INTERM)
	$(FUT_CC) $(FUT_CC_FLAGS) -c -o $@ $(FUT_INTERM)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(FUT_INTERM)
	$(CC) -include $(FUT_HEADER) -c -o $@ $< $(CCFLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(FUT_INTERM)
	$(CXX) -include $(FUT_HEADER) -c -o $@ $< $(CXXFLAGS) 
	
tracer: $(FUT_OBJ) $(CC_OBJ) $(CXX_OBJ)
	$(CXX) -o $@ $^ $(LIBS) $(CXXFLAGS) 

run: all 
	./tracer

.PHONY: clean

clean:
	rm -f bin/*  *~ $(TARGET) 
