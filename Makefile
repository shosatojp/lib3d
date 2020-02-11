TARGET := lib3d
CC := gcc
CCOPT := -g -lm -O0 -std=c99 -pthread
SRCDIR := src

SRC := $(shell find $(SRCDIR) -name "*.c")
OBJ := $(addsuffix .o, $(basename $(SRC)))

$(TARGET):$(OBJ)
	mkdir -p bin
	$(CC) $^ $(CCOPT) -o $@

run: $(TARGET) FORCE
	./$(TARGET)

%.o: %.c
	$(CC) $^ $(CCOPT) -c -o $@

gif:
	convert -delay 2 bin/*.ppm anim.gif

clean:
	-rm bin/*.ppm *.out *.exe *.gif $(TARGET)
	-rm src/*.o *.o
	-rm -rf bin

FORCE:;