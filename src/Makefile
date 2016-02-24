CC = g++
LD = $(CC)
CFLAGS = -c  -g -O0 -I./ 
LDFLAGS = -L./  -lgtest  -lpthread -lmordor  -lboost_system -lboost_thread -lcrypto -lboost_regex


TARGET = a.out

SOURCES=$(shell find . -name "*.cpp")
OBJECTS= $(SOURCES:.cpp=.o)
##OBJECTS = $(patsubst %.c, %.o, $(shell find . -name "*.cpp"))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

# You don't even need to be explicit here,
# compiling C files is handled automagically by Make.
%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(TARGET) $(OBJECTS)

test: all
	$(TARGET)
