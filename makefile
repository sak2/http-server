server: server.c get.c
	gcc -o server server.c get.c -lpthread
debug: server.c get.c
	gcc -o server server.c get.c -lpthread -g
client: client.c
	gcc -o client client.c

clean:
	rm server
	rm client