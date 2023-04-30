CC=gcc
CFLAGS=-Wall -Wextra -std=c99
LDFLAGS=-L. -Wl,-rpath,.
LIBS=-ldl

.PHONY: all clean

all: encode decode cmp copy stshell

stshell.o: stshell.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

cmp.o: cmp.c
	$(CC) $(CFLAGS) -c $< -o $@

copy.o: copy.c
	$(CC) $(CFLAGS) -c $< -o $@

stshell.so: stshell.o
	$(CC) -fPIC -shared -o $@ $<

cmp.so: cmp.o
	$(CC) -fPIC -shared -o $@ $<

copy.so: copy.o
	$(CC) -fPIC -shared -o $@ $<

codecA_lib.o: codecA.c codec.h
	$(CC) $(CFLAGS) -c $< -o $@

codecB_lib.o: codecB.c codec.h
	$(CC) $(CFLAGS) -c $< -o $@

codecA.so: codecA_lib.o
	$(CC) -fPIC -shared -o $@ $<

codecB.so: codecB_lib.o
	$(CC) -shared -o $@ $<

encode.o: encode.c
	$(CC) $(CFLAGS) -c $<

decode.o: decode.c
	$(CC) $(CFLAGS) -c $<

stshell: stshell.o stshell.so
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

encode: encode.o codecA_lib.o codecA.so codecB.so
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

decode: decode.o codecA.so codecB.so
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

cmp: cmp.o cmp.so
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

copy: copy.o copy.so
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f *.o *.so encode decode copy cmp stshell

