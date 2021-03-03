/* Sistemas Operativos, DEI/IST/ULisboa 2019-20 */

#include "fs.h"
#include "lib/bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sync.h"
#include "lib/inodes.h"




tecnicofs* new_tecnicofs(){
	tecnicofs*fs = malloc(sizeof(tecnicofs));
	if (!fs) {
		perror("failed to allocate tecnicofs");
		exit(EXIT_FAILURE);
	}
	fs->bstRoot = NULL;
	fs->nextINumber = 0;
	sync_init(&(fs->bstLock));
	return fs;
}

void free_tecnicofs(tecnicofs* fs){
	free_tree(fs->bstRoot);
	sync_destroy(&(fs->bstLock));
	free(fs);
}

int create(tecnicofs* fs, char *name, uid_t uid, int ownerPerm, int othersPerm, int existeiNumber){
	if (lookup(fs, name) != -1) {
		return -1;
	}
	sync_wrlock(&(fs->bstLock));
	int iNumber;
	if (existeiNumber >= 0) iNumber = existeiNumber;
	else iNumber = inode_create(uid, ownerPerm, othersPerm);
	fs->bstRoot = insert(fs->bstRoot, name, iNumber);
	sync_unlock(&(fs->bstLock));
	return 0;
}

void delete(tecnicofs* fs, char *name){
	sync_wrlock(&(fs->bstLock));
	fs->bstRoot = remove_item(fs->bstRoot, name);
	sync_unlock(&(fs->bstLock));
}

int lookup(tecnicofs* fs, char *name){
	sync_rdlock(&(fs->bstLock));
	int inumber = 0;
	node* searchNode = search(fs->bstRoot, name);
	if ( searchNode ) {
		inumber = searchNode->inumber;
	}
	else inumber = -1;
	sync_unlock(&(fs->bstLock));
	return inumber;
}

void print_tecnicofs_tree(FILE * fp, tecnicofs *fs){
	print_tree(fp, fs->bstRoot);
}
