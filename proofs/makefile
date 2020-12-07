# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall 

# the build target executable:
# Aca poner el name del .c

TARGET1 = proof
TARGET2 = funciones


all: $(TARGET1)

$(TARGET1): $(TARGET1).cpp
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).cpp $(TARGET2).cpp -lpthread

clean:
	$(RM) $(TARGET1)
