COMPILER = g++
FLAGS = -g -Wall -std=c++11x
EXEC = exec
TARGET = program4

all: $(TARGET).o
	$(COMPILER) $(FLAGS) -o $(EXEC) $(TARGET).o

$(TARGET): $(TARGET)
	$(COMPILER) $(FLAGS) -c $(TARGET).cpp

clean:
	$(RM) $(TARGET) rm *.o exec

