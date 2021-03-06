TARGET = shell
LIBS = -lreadline
CC = cc
override CFLAGS += -g -pedantic -Wall -ansi -O3

.PHONY: default all clean manual

default: $(TARGET)
all: default

manual:
	pandoc -s -t man man/Shell.md -o ./man/Shell.1
	mv ./man/Shell.1 /usr/local/share/man/man1
	gzip /usr/local/share/man/man1/Shell.1

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f ./man/Shell.1
