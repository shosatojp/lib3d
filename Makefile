TARGET := lib3d
CC := gcc
# -mtune=native -march=native -mfpmath=both 
CCOPT := -g -lm -O0 -std=c11 -pthread -Wall -W -Wno-unused-variable -Wno-unused-parameter -Wno-parentheses -Wno-type-limits -Wno-cast-function-type
SRCDIR := src
TARGETOPT := -o bin -t 4 -f 10 -q ld
# TARGETOPT := -o bin -t 1 -f 1 -q ld -b 59
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
	# convert -delay 3.33 $(BINDIR)/*.ppm out.gif
	ffmpeg -pattern_type glob -i "$(BINDIR)/*.ppm" -vf palettegen palette.png -y
	ffmpeg -pattern_type glob -i "$(BINDIR)/*.ppm" -i palette.png -filter_complex paletteuse out.gif -y
	rm palette.png

mp4:
	ffmpeg -pattern_type glob -framerate 30 -i "$(BINDIR)/*.ppm" -c:v libx264 -strict -2 -preset slow -pix_fmt yuv420p -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -f mp4 out.mp4 -y

clean:
	-rm bin/*.ppm *.out *.exe *.png $(TARGET)
	-rm src/*.o *.o
	-rm -rf bin

concat:
	-find bin -name *.ppm | xargs -L 3 -P 16 -I @ bash -c 'convert @ `echo @ | sed -e "s/.ppm/.png/"` && rm @'
	-seq 0 100 | xargs -L 1 -P 16 -I @ bash -c 'montage "bin/`printf "%06d" @`-*.png" -geometry +0+0 -tile 2x2 bin/`printf "%06d" @`.png && convert bin/`printf "%06d" @`.png bin/`printf "%06d" @`.png && rm bin/`printf "%06d" @`-*.png'
	ffmpeg -pattern_type glob -framerate 30 -i "bin/*.png" -c:v libx264 -strict -2 -preset slow -pix_fmt yuv420p -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" -f mp4 bin/out.mp4 -y
	rm bin/*.png

FORCE:;