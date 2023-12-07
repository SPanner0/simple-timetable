CC = g++

OBJ = main.cpp

RELEASE_BIN = timetable

DEBUG_BIN = debug-timetable

release : $(OBJ)
	$(CC) $(OBJ) -o $(RELEASE_BIN)

debug : $(OBJ)
	$(CC) $(OBJ) -o $(DEBUG_BIN) -g