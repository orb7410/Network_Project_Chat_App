# makefile

TARGET_CLIENT= mainclientMng.out
TARGET_SERVER= mainserverMng.out
TARGET_SEND=send.out
TARGET_RECV=recv.out
CC= gcc
CFLAGS= -ansi -g -pedantic

OBJS_C= clientMng.o mainclientMng.o clientNet.o ui.o protocol.o system.o
OBJS_S= serverManager.o userManager.o protocol.o server.o user.o mainserverMng.o group.o groupManager.o
OBJS_SEND= send.o
OBJS_RECV= recv.o

LIST_PATH= ../../Day28/itrFunctions
HASH_PATH= ../../Day35/HashMap
QUE_PATH= ../../Day25/genQ
VECTOR_PATH = ../../Day26/genericheapCR

$(TARGET_CLIENT): $(OBJS_C) $(TARGET_SERVER) $(TARGET_SEND) $(TARGET_RECV)
	$(CC) -o $(TARGET_CLIENT) $(OBJS_C) -lm -L ../../gends -lgends
	
$(TARGET_SERVER): $(OBJS_S) 
	$(CC) -o $(TARGET_SERVER) $(OBJS_S) -lm -L ../../gends -lgends
	
$(TARGET_SEND): $(OBJS_SEND)
	$(CC) -o $(TARGET_SEND) $(OBJS_SEND)
	
$(TARGET_RECV): $(OBJS_RECV)
	$(CC) -o $(TARGET_RECV) $(OBJS_RECV)

serverManager.o: serverManager.c serverManager.h userManager.h protocol.h server.h
	$(CC) $(CFLAGS) -I $(LIST_PATH) -I $(VECTOR_PATH) -c serverManager.c
	
mainserverMng.o: mainserverMng.c serverManager.c serverManager.h
	$(CC) $(CFLAGS) -c mainserverMng.c

clientNet.o: clientNet.c clientNet.h 
	$(CC) $(CFLAGS) -c clientNet.c 
	
system.o: system.c system.h
	$(CC) $(CFLAGS) -c system.c 

clientMng.o: clientMng.c mainclientMng.c clientMng.h clientNet.h protocol.h ui.h
	$(CC) $(CFLAGS) -I $(VECTOR_PATH) -I $(HASH_PATH) -c clientMng.c
	
mainclientMng.o: mainclientMng.c clientMng.c clientMng.h
	$(CC) $(CFLAGS) -c mainclientMng.c

server.o: server.c server.h
	$(CC) $(CFLAGS) -I $(LIST_PATH) -c server.c
	
user.o: user.c user.h 
	$(CC) $(CFLAGS) -I $(LIST_PATH) -c user.c

userManager.o: userManager.c  user.h 
	$(CC) $(CFLAGS) -I $(HASH_PATH) -I $(LIST_PATH) -c userManager.c

protocol.o: protocol.c protocol.h 
	$(CC) $(CFLAGS) -I $(VECTOR_PATH) -c protocol.c
	
ui.o: ui.c ui.h 
	$(CC) $(CFLAGS) -c ui.c

group.o: group.c group.h
	$(CC) $(CFLAGS) -c group.c

groupManager.o: groupManager.c groupManager.h group.h
	$(CC) $(CFLAGS) -I $(HASH_PATH) -I $(QUE_PATH) -I $(VECTOR_PATH) -c groupManager.c
	
send.o: send.c
	$(CC) $(CFLAGS) -c send.c
	
recv.o: recv.c
	$(CC) $(CFLAGS) -c recv.c

valc:
	valgrind --log-file=Vc4.log --leak-check=full --track-origins=yes ./mainclientMng.out
	
val:
	valgrind --log-file=V4.log --leak-check=full --track-origins=yes ./mainserverMng.out

runs:
	./$(TARGET_SERVER)

runc:
	./$(TARGET_CLIENT)

clean:
	rm -f $(TARGET_SERVER) $(OBJS)
	
cleanc:
	rm -f $(TARGET_CLIENT) $(OBJS)


	

