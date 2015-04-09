COMPILER = g++
FLAGS = -g -Wall -std=c++0x
EXEC = lab3
TARGET = lab3

all: $(TARGET).o
	$(COMPILER) $(FLAGS) -o $(EXEC) $(TARGET).o

$(TARGET): $(TARGET)
	$(COMPILER) $(FLAGS) -c $(TARGET).cpp

clean:
	$(RM) $(TARGET) rm *.o

