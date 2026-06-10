CC = gcc
CFLAGS = -Wall -g -O2

SRCDIR = src
BINDIR = .

SORTING_DIR = $(SRCDIR)/sorting
KNAPSACK_DIR = $(SRCDIR)/knapsack

TARGET = algorithm_experiment

SRC = $(SRCDIR)/main.c \
      $(SRCDIR)/common.c \
      $(SORTING_DIR)/bubble_sort.c \
      $(SORTING_DIR)/merge_sort.c \
      $(SORTING_DIR)/quick_sort.c \
      $(SORTING_DIR)/sorting_test.c \
      $(KNAPSACK_DIR)/knapsack_utils.c \
      $(KNAPSACK_DIR)/brute_force.c \
      $(KNAPSACK_DIR)/dynamic_programming.c \
      $(KNAPSACK_DIR)/greedy.c \
      $(KNAPSACK_DIR)/backtracking.c \
      $(KNAPSACK_DIR)/knapsack_test.c

OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-del $(subst /,\,$(OBJ)) 2>nul
	-del $(TARGET).exe 2>nul
