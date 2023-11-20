# Makefile for Windows

CC = g++  # Compiler
SRC = src/main.cpp  # Source file
OBJ = main.o  # Object file
TARGET = main.exe  # Executable name

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) -static-libgcc -static-libstdc++

$(OBJ): $(SRC)
	$(CC) -c $(SRC) -o $(OBJ) -static-libgcc -static-libstdc++

clean:
	del $(TARGET) $(OBJ)
