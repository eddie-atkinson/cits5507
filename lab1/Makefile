.PHONY: clean
CC  = gcc
CFLAGS=-std=c99 -Wall -Werror
CFILES = $(shell find src -name "*.c")
OBJS = $(CFILES:.c=.o)
TARGET = main

TEST_CFILES = $(shell find tests -name "*.c") 
CFILES_NO_MAIN = $(shell find src -name "*.c" | grep -v main.c)
OBJS_NO_MAIN = $(CFILES_NO_MAIN:.c=.o)
TEST_OBJS = $(TEST_CFILES:.c=.o) 
TEST_TARGET = test


RM = -rm -f

all: $(TARGET)
check: $(TEST_TARGET)

	
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

$(TEST_TARGET): $(TEST_OBJS) $(OBJS_NO_MAIN)
	$(CC) -o $(TEST_TARGET) $(TEST_OBJS) $(OBJS_NO_MAIN)

%o : %c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(TARGET) $(TEST_TARGET) $(OBJS) $(DATA)