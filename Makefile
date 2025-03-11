CFLAGS = -lllhttp
CC = gcc

all: final

final: server.o urls.o urlhelper.o
	@echo "Linking and creating final output file"
	$(CC) server.o urls.o urlhelper.o $(CFLAGS) -o serverout
	@echo "Cleaning up object files"
	rm -f *.o

urls.o: urls.c
	@echo "Compiling urls"
	$(CC) -c urls.c -o urls.o

server.o: server.c
	@echo "Compiling server"
	$(CC) -c server.c -o server.o

urlhelper.o: helpers/url_all.c
	@echo "Compiling urlhelper"
	$(CC) -c helpers/url_all.c -o urlhelper.o

clean:
	@echo "Cleaning up object files"
	rm -f *.o serverout
