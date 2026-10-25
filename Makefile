CC = gcc
CFLAGS = -Wall -Wextra -g -Istructs
EXEC = tema1

OBJ = main.o system.o incident.o unit.o intervention.o

build: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

main.o: main.c system.h
	$(CC) $(CFLAGS) -c main.c

system.o: system.c system.h
	$(CC) $(CFLAGS) -c system.c

incident.o: structs/incident.c structs/incident.h
	$(CC) $(CFLAGS) -c structs/incident.c -o incident.o

unit.o: structs/unit.c structs/unit.h
	$(CC) $(CFLAGS) -c structs/unit.c -o unit.o

intervention.o: structs/intervention.c structs/intervention.h structs/unit.h structs/incident.h
	$(CC) $(CFLAGS) -c structs/intervention.c -o intervention.o

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC) rezultat_valgrind.txt tema1.in tema1.out

.PHONY: build run clean