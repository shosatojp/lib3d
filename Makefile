TARGET := lib3d
CC := gcc
CCOPT := -g -lm -O3 -std=c11 -pthread -mtune=native -march=native -mfpmath=both
SRCDIR := src
TARGETOPT := -o bin -t 16 -f 10 -w 1920 -h 1080 -r raytrace

SRC := $(shell find $(SRCDIR) -name "*.c")
OBJ := $(addsuffix .o, $(basename $(SRC)))

$(TARGET):$(OBJ)
	mkdir -p bin
	$(CC) $^ $(CCOPT) -o $@

run: $(TARGET) FORCE
	./$(TARGET) $(TARGETOPT)

%.o: %.c
	$(CC) $^ $(CCOPT) -c -o $@

gif:
	convert -delay 2 bin/*.ppm out.gif

mp4:
	ffmpeg -pattern_type glob -framerate 30 -i "bin/*.ppm" out.mp4 -y -vcodec libx264 

clean:
	-rm bin/*.ppm *.out *.exe $(TARGET)
	-rm src/*.o *.o
	-rm -rf bin

FORCE:;