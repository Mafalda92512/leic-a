# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2019-20

SOURCES = server.c fs.c sync.c
SOURCES+= lib/bst.c
OBJS_RWLOCK = $(SOURCES:%.c=%-rwlock.o)
OBJS = $(OBJS_RWLOCK)
CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -I../ -g
LDFLAGS=-lm -pthread
TARGETS = tecnicofs-rwlock

.PHONY: all clean

all: $(TARGETS)


$(TARGETS):

	$(LD) $(CFLAGS) $^ -o $@ $(LDFLAGS)



### RWLOCK ###
server/lib/bst-rwlock.o: CFLAGS+=-DRWLOCK
server/lib/bst-rwlock.o: server/lib/bst.c server/lib/bst.h

server/fs-rwlock.o: CFLAGS+=-DRWLOCK
server/fs-rwlock.o: server/fs.c server/fs.h server/lib/bst.h

server/sync-rwlock.o: CFLAGS+=-DRWLOCK
server/sync-rwlock.o: server/sync.c server/sync.h server/constants.h

client/client-rwlock.o: CFLAGS+=-DRWLOCK
client/client-rwlock.o: client/client.c client/tecnicofs-client-api.h

server/inodes-rwlock.o: CFLAGS+=-DRWLOCK
server/inodes-rwlock.o: server/lib/inodes.c server/lib/inodes.h

server/main-rwlock.o: CFLAGS+=-DRWLOCK
server/main-rwlock.o: server/server.c server/fs.h server/lib/bst.h server/constants.h server/lib/timer.h server/sync.h client/tecnicofs-client-api.h server/lib/inodes.h
tecnicofs-rwlock: server/lib/bst-rwlock.o server/fs-rwlock.o server/sync-rwlock.o client/client-rwlock.o server/inodes-rwlock.o server/main-rwlock.o


%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGETS)
	rm -f server/main-rwlock.o server/inodes-rwlock.o client/client-rwlock.o server/sync-rwlock.o server/fs-rwlock.o server/lib/bst-rwlock.o
