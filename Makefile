TARGET := lib3d
CC := gcc
CCOPT := -g -lm -O0 -std=c11 -pthread -mtune=native -march=native -mfpmath=both
SRCDIR := src
TARGETOPT := -o bin -t 16 -f 10 -w 1920 -h 1080 -r raytrace
BINDIR := bin

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
	convert -delay 2 $(BINDIR)/*.ppm out.gif

mp4:
	ffmpeg -pattern_type glob -framerate 30 -i "$(BINDIR)/*.ppm" -c:v libx264 -strict -2 -preset slow -pix_fmt yuv420p -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -f mp4 out.mp4 -y

clean:
	-rm bin/*.ppm *.out *.exe $(TARGET)
	-rm src/*.o *.o
	-rm -rf bin

FORCE:;