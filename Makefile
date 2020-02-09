TARGET := lib3d
CC := gcc
CCOPT := -g -lm -O3
SRCDIR := src

SRC := $(shell find $(SRCDIR) -name *.c)
OBJ := $(addsuffix .o, $(basename $(SRC)))

$(TARGET):$(OBJ)
	$(CC) $^ $(CCOPT) -o $@

%.o: %.c
	$(CC) $^ $(CCOPT) -c -o $@

gif:
	convert -delay 10 *.ppm anim.gif

clean:
	-rm *.ppm *.out *.exe *.gif $(TARGET)
	-rm src/*.o *.o