CC = gcc

all:pd user as fs

pd:pd.c
	$(CC) -g pd.c -o pd

user:user.c
	$(CC) -g user.c -o user

as:as.c
	$(CC) -g as.c -o as

fs:fs.c
	$(CC) -g fs.c -o fs

clean:
	rm pd user as fs
	rm -rf USERS
