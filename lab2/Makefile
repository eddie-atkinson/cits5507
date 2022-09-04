.PHONY: clean
CXX  = g++
CXXFLAGS=-std=c++11 -Wall -Werror -fopenmp
LDFLAGS=-fopenmp
CXXFILES = $(shell find src -name "*.cpp")
OBJS = $(CXXFILES:.cpp=.o)
TARGET = main

TEST_CXXFILES = $(shell find tests -name "*.cpp") 
CXXFILES_NO_MAIN = $(shell find src -name "*.cpp" | grep -v main.cpp)
OBJS_NO_MAIN = $(CXXFILES_NO_MAIN:.cxx=.o)
TEST_OBJS = $(TEST_CXXFILES:.cxx=.o) 
TEST_TARGET = test


RM = -rm -f

all: $(TARGET)
check: $(TEST_TARGET)

	
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

clean:
	$(RM) $(TARGET) $(TEST_TARGET) $(OBJS) $(DATA)
