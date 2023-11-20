# Makefile for Windows

CC = g++  # Compiler
SRC = src/main.cpp  # Source file
OBJ = main.o  # Object file
TARGET = main.exe  # Executable name

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC)
	$(CC) -c $(SRC) -o $(OBJ)

clean:
	del $(TARGET) $(OBJ)
