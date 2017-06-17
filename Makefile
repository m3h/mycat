mycat: cmdline.o mycat.o
	gcc -o mycat cmdline.o mycat.o

cmdline.c: cmdline.ggo
	gengetopt --input=cmdline.ggo --unamed-opts
cmdline.h: cmdline.c

cmdline.o: cmdline.c cmdline.h
	gcc -c cmdline.c
mycat.o: mycat.c
	gcc -c mycat.c

clean:
	rm -rf *.o cmdline.c cmdline.h
	

