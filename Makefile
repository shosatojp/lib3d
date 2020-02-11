TARGET := lib3d
CC := gcc
CCOPT := -g -lm -O3 -std=c11 -pthread
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
	convert -delay 10 bin/*.ppm anim.gif

clean:
	-rm bin/*.ppm *.out *.exe *.gif $(TARGET)
	-rm src/*.o *.o
	-rm -rf bin

FORCE:;