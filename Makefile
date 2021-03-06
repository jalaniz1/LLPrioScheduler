# Makefile for the CS:APP Shell Lab


VERSION = 1
sim = ./sim
simec= ./simec
CC = gcc
CXX = g++
CFLAGS = -Wall -O
FILES = $(sim) $(simec)

all: $(FILES)


simec: simec.o schedule_ec.o
	$(CC) -o simec simec.o schedule_ec.o

clean:
	rm -f $(FILES) *.o
