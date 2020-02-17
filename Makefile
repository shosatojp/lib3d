TARGET := lib3d
CC := gcc
CCOPT := -g -lm -O0 -std=c11 -pthread -mtune=native -march=native -mfpmath=both
SRCDIR := src
TARGETOPT := -o bin -t 16 -f 30 -w 480 -h 320 -r raytrace -f 100
BINDIR := bin

SRC := $(shell find $(SRCDIR) -name "*.c")
HEADER := $(shell find $(SRCDIR) -name "*.h")
OBJ := $(addsuffix .o, $(basename $(SRC)))

$(TARGET):$(OBJ) $(HEADER)
	mkdir -p $(BINDIR)
	$(CC) $(OBJ) $(CCOPT) -o $@

run: $(TARGET) FORCE
	./$(TARGET) $(TARGETOPT)

%.o: %.c
	$(CC) $^ $(CCOPT) -c -o $@

gif:
	# convert -delay 2 $(BINDIR)/*.ppm out.gif
	ffmpeg -pattern_type glob -i "$(BINDIR)/*.ppm" -vf palettegen palette.png -y
	ffmpeg -pattern_type glob -i "$(BINDIR)/*.ppm" -i palette.png -filter_complex paletteuse out.gif -y
	rm palette.png

mp4:
	ffmpeg -pattern_type glob -framerate 30 -i "$(BINDIR)/*.ppm" -c:v libx264 -strict -2 -preset slow -pix_fmt yuv420p -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -f mp4 out.mp4 -y

clean:
	-rm bin/*.ppm *.out *.exe *.png $(TARGET)
	-rm src/*.o *.o
	-rm -rf bin

FORCE:;