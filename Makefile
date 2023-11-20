CC = g++
CFLAGS = -Wall

PROGRAM = SerialPortExample

# Add any additional source files here
SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del $(OBJS) $(PROGRAM).exe
