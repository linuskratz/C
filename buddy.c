#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

void printHelp(char* executable_name);
void printMenu();


struct block {
  _Bool used;
  int size;
};

struct query{
  int position;
  _Bool active;
  struct query *next;
};

struct query *new_query(int position){
  struct query *new_block = (struct query*) malloc(sizeof(struct query));
  if(new_block){
    new_block->active=1;
    new_block->position=position;
    new_block->next = NULL;
  }
  return new_block;
}

struct query *insertIntoList(struct query *head, struct query *new){
  struct query *current = head;
  if (head){
    while (current->next){
      current = current->next;
    }
    current->next = new;
    return head;
  }else{
    return new;
  }
}

int find_position(struct query *c, int querynumber){
  struct query *head = c;
  if (head && querynumber>0){
    for (int i = 1; i < querynumber; i++) {
      if (head->next){
        head = head->next;
      }else{
        return -1;
      }
    }
    if (!(head->active)){
      return -1;
    }
    head->active = 0;
    return head->position;
  }else{
    return -1;
  }
}


struct block *new_block(int size, _Bool used){
  struct block *new_block = (struct block*) malloc(sizeof(struct block));
  if(new_block){
    new_block->size=size;
    new_block->used=used;
  }
  return new_block;
}



void splitBlock(int position, struct block* blocks[]){
  int size = blocks[position]->size;
  if (size<=1){
    printf("%s\n","error ->blocksize <=1");
  }else{
    if (blocks[position]->used){
      printf("%s\n","eror -> splitting used block");
    }
    blocks[position]->size = size/2;
    blocks[position+size/2]->size = size/2;
  }
}


//returns position of block if sucessful allocation else returns -1
int allocateBlock(int noBlocks, struct block* blocks[], int blockslength){
  int foundPosition = -1;
  int searchSize = 1;
  while (foundPosition == -1 && searchSize*noBlocks<=blockslength){
    for(int i = 0; i<blockslength; i++){
      if (blocks[i]->size == searchSize*noBlocks && !(blocks[i]->used)){
        foundPosition = i;
        break;
      }
    }
    if(foundPosition == -1){
      searchSize = searchSize*2;
    }
  }
  if(foundPosition != -1){
    for(int i = 0; i<(int)(log(searchSize)/log(2)); i++){
      splitBlock(foundPosition, blocks);
    }
    for(int i = 0; i<noBlocks; i++){
      blocks[foundPosition+i]->used = 1;
    }
  }
  return foundPosition;
}



void freeBlock(struct block* blocks[], int position, int blockslength){
  int size = blocks[position]->size;
  if (size < blockslength){
    int sum = position%(size*2);
    int buddy;
    if (sum == 0){
      buddy = position+size;
    }else{
      buddy = position-size;
    }
    if(blocks[buddy]->used || (blocks[buddy]->size != size)){
      for(int i= 0; i<size; i++){
        blocks[position+i]->used = 0;
      }
    }else{
      for(int i = 0; i<size; i++){
        blocks[position+i]->used= 0;
        blocks[buddy+i]->used= 0;
      }
      if(sum == 0){
        blocks[buddy]->size = 0;
        blocks[position]->size = 2*size;
        freeBlock(blocks,position,blockslength);
      }else{
        blocks[buddy]->size = 2*size;
        blocks[position]->size =0;
        freeBlock(blocks,buddy,blockslength);
      }
    }
  }
}




int main(int argc, char* argv[]) {
    int blocksize = 4, numblocks = 32;
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i],"-b")==0){
          blocksize = atoi(argv[i+1]);
          float log2val = log(blocksize)/log(2);
          if (log2val!=floor(log2val)){
            printHelp(argv[0]);
            return 1;
          }
      }else if(strcmp(argv[i], "-n")==0){
          numblocks = atoi(argv[i+1]);
          float log2val = log(numblocks)/log(2);
          if (log2val!=floor(log2val)){
            printHelp(argv[0]);
            return 1;
          }
      }else if(*argv[i] == '-'){
        printHelp(argv[0]);
        return 1;
      }
    }
    printMenu();

    struct block* blocks[numblocks];
    blocks[0] = new_block(numblocks,0);
    for(int i = 1; i<numblocks; i++){
      blocks[i] = new_block(0,0);
    }
    struct query * head = NULL;
    char* buf = malloc(sizeof(char)*12);

    while (1) {
      fgets(buf, 12, stdin);
      if (buf[0] == 'm'){
        int position = -1;
        int allocInByte = atoi(&buf[2]);
        int blocksToAlloc = (int) ceil(allocInByte*1.0/(blocksize*1024)); // muss 2 potenz sein
        blocksToAlloc = pow(2, ceil(log(blocksToAlloc)/log(2)));
        if(blocksToAlloc>0 && blocksToAlloc <= numblocks){
          position = allocateBlock(blocksToAlloc, blocks, numblocks);
        }
        if (position == -1){
          printf("error\n");
        }else{
          printf("success\n");
          struct query * new =new_query(position);
          head = insertIntoList(head, new);
        }
      }else if (buf[0] =='l'){
          for(int i = 0; i<numblocks; i++){
            if(blocks[i]){
              if(blocks[i]->used){
                printf("%c",'P');
              }else{
                printf("%c",'F');
              }
            }
          }
          printf("\n");

        /*
        for (int i = 0; i < numblocks; i++) {
          printf("%d",blocks[i]->size);
        }
        printf("\n");
        */
      }else if (buf[0] =='f'){
        int tId = atoi(&buf[2]);
        int position = find_position(head, tId);

        if (position == -1){
          printf("Could not free %d - number is invalid!\n", tId);
        }else{
          freeBlock(blocks,position, numblocks);
          printf("success\n");
        }
      }else if (buf[0] =='q'){
        for(int i = 0;i<numblocks; i++){
          free(blocks[i]);
        }
        if (head){
          while(head->next){
            struct query * next = head ->next;
            free(head);
            head = next;
          }
        }
        free(buf);
        return 0;
      }else{
          printMenu();
      }
    }
}




/* Tester expects these exact (formatted) strings
 * V   V   V   V   V   V   V   V   V   V    */
void printHelp(char* executable_name) {
    printf("Usage: %s [-b BLOCKSIZE] [-n NUM_BLOCKS]\n"
           "BLOCKSIZE: integer, interpreted as kiB (i.e. -b 4 => 4096 Bytes). Has to be power of 2! The default is 4.\n"
           "NUM_BLOCKS: integer, number of blocks to be used. Has to be power of 2! The default is 32.\n",
           executable_name);
}

void printMenu() {
    printf("Type your command, then [enter]\n"
           "Available commands:\n"
           "m SIZE\t\tallocate SIZE Bytes of memory. SIZE has to be > 0.\n"
           "f POINTER\tfree the memory referenced by POINTER. If it is invalid, an error is shown and nothing else happens.\n"
           "l\t\tprint current memory layout.\n"
           "q\t\tquit the program.\n");
}
/* /\  /\  /\  /\  /\  /\  /\  /\  /\  /\ */
