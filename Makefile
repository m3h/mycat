mycat: cmdline.o mycat.o
	cc -o mycat cmdline.o mycat.o

cmdline.c: cmdline.ggo
	gengetopt --input=cmdline.ggo --unamed-opts
cmdline.h: cmdline.c

cmdline.o: cmdline.c cmdline.h
	cc -c cmdline.c
mycat.o: mycat.c
	cc -c mycat.c

clean:
	rm -rf *.o cmdline.c cmdline.h
	

