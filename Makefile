CC=gcc
CFLAGS=-Wall -Wextra -g -O3
LDFLAGS= -lm -lGL -lGLU -lglut -lpng 
SRCFILES=$(wildcard *.c)
OBJFILES=$(SRCFILES:.c=.o)
DEPFILES=$(SRCFILES:.c=.d)
TARGET=main
.PHONY: all clean

all: $(TARGET)

-include $(DEPFILES)

main: $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o main $(LDFLAGS)

%.o: %.cc Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	-@rm -f $(TARGET) $(OBJFILES) $(DEPFILES)

#  gprof main gmon.out > prof_output
profile: $(OBJFILES)
	 $(CC) $(CFLAGS) -pg $(OBJFILES) -o main $(LDFLAGS)

print-%: ; @echo $*=$($*)
