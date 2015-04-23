COMPILER = g++
FLAGS = -g -Wall -std=c++11
EXEC = program4
TARGET = program4

all: $(TARGET).o
	$(COMPILER) $(FLAGS) -o $(EXEC) $(TARGET).o

$(TARGET): $(TARGET)
	$(COMPILER) $(FLAGS) -c $(TARGET).cpp

clean:
	$(RM) $(TARGET) rm *.o program4

