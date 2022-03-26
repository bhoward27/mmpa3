CC = g++
STD = -std=c++17
D = -Og -D DEBUG
R = -O3
CONFIG = $(D)
OFLAGS = $(STD) -g -c $(CONFIG) -Wall
CFLAGS = $(STD) -o a.out
OBJS = main.o

a.out: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)

main.o: main.cpp
	$(CC) $(OFLAGS) main.cpp

clean:
	rm -f *.out *.o
