default:
	gcc ./src/matrix.c ./src/io.c -g -lm  && ./a.out

gif:
	convert -delay 10 hoge-*.ppm anim.gif

clean:
	rm *.ppm *.out