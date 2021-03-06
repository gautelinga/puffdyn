CXX      := -g++
CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror -O3 -std=c++17 -ffast-math
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := build
OBJ_DIR  := $(BUILD)
TARGET   := puff
INCLUDE  := -Iinclude/
SRC      := $(filter-out src/pybind11.cpp, $(wildcard src/*.cpp))

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

.PHONY: all build clean allclean

build:
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf build/*

allclean: clean
	rm $(TARGET)
