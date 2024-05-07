CC=gcc
FLAGS=-Wall -Werror
OBJS=ttt.o main.o
TARGET= main

build: $(TARGET)
		./$^

$(TARGET): $(OBJS)
		$(CC) $(FLAGS) $^ -o $@

%.o: %.c
		$(CC) -g -c $(FLAGS) $< -o $@

.PHONY:
clean:
		rm -f $(OBJS)
		rm -f $(TARGET)