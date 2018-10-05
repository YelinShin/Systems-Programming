all: asst3 server

asst3: netclient.o libnetfiles.o
	gcc -g -o netclient netclient.c libnetfiles.c 

server: netfileserver.o
	gcc -g -lpthread -o netfileserver netfileserver.c

netfileserver: netfileserver.o
	gcc -g -c netfileserver.c 

libnetfiles: libnetfiles.o libnetfiles.h
	gcc -g -c libnetfiles.c

netclient: netclient.o 
	gcc -g -c netclient.c

clean:
	rm -f *.o all netclient netfileserver