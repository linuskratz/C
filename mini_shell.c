#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INPUT_SIZE 256
#define NUM_COMMANDS 5

void exit2();
void help();
void spell();
void countLetter();
void replace();
char *readIntoBuffer(char *buffer, unsigned int size);


unsigned int readUserInput(char *buffer, unsigned int size)
{
	static char * lookup[5] = {"exit", "help", "spell", "countLetter", "replace"};
	void (*functions[5]) () ={&exit2, &help, &spell, &countLetter, &replace};
	int minishell = 0;
	buffer = readIntoBuffer (buffer, size);

	for (int i = 0; i<5; i++){
		if(strcmp(buffer,lookup[i])==0){
			minishell = 1;
			(*functions[i])();
		}
	}
	if (!minishell){
		return system(buffer);
	}
	return 0;
}

void exit2() {
	printf("%s\n","goodbye");
}

void help(){
	printf("%s\n","-------- Mini Shell --------\nThe following commands are supported:\n- exit : Terminates the Mini Shell.\n- help : Prints help information.\n- spell : Asks for a string. Spells the string by printing each char of the string to a new line.");
	printf("%s\n","- countLetter : Asks for a string and a char. Prints the number of occurrences of the char in the string.\n- replace : Asks for a string and two characters. Replaces all occurrences of the first char within the string with the second char.\n- All regular shell commands are supported as well.\n----------------------------");
	char *newbuf = (char *) malloc(INPUT_SIZE * sizeof(char));
	readUserInput(newbuf, INPUT_SIZE);
}

void spell(){
 printf("%s\n","Please provide a string :");
 char *buffer = (char *) malloc(INPUT_SIZE * sizeof(char));
 buffer = readIntoBuffer (buffer, INPUT_SIZE);
 while ((*buffer)!='\0'){
	 printf("%c\n", *buffer);
	 buffer++;
 }
 char *newbuf = (char *) malloc(INPUT_SIZE * sizeof(char));
 readUserInput(newbuf, INPUT_SIZE);
}

char *readIntoBuffer(char *buffer, unsigned int size){
	char current=' ';
  unsigned int length = 0;
	fflush(stdin);
	if(size == 0)
		return NULL;
	if(size == 1)
		size=2;
	do{
 	 current = fgetc(stdin);
 	 if ((current<=122 && current>=97) || (current>=65 && current<=90) || (current>=48 && current<=57) || (current == ' ')){
 		 buffer[length] = current;
 		 length +=1;
 	 }
  }while(length < size && current!='\n');
  buffer[length]='\0';
	return buffer;
}

void countLetter(){
	printf("%s\n","Please provide a string:");
  char *buffer = (char *) malloc(INPUT_SIZE * sizeof(char));
	buffer = readIntoBuffer (buffer, INPUT_SIZE);
	printf("%s\n","Please provide a character:");
  char *buffer2 = (char *) malloc(sizeof(char));
  buffer2 = readIntoBuffer (buffer2,sizeof(char));
	int count = 0;
	if (buffer && buffer2){
  while ((*buffer)!='\0'){
		if (*buffer == *buffer2){
			count++;
		}
		buffer++;
  }
	}
	printf("Character %c occurred %d times.\n", *buffer2, count);
	char *newbuf = (char *) malloc(INPUT_SIZE * sizeof(char));
	readUserInput(newbuf, INPUT_SIZE);
}

void replace(){
	printf("%s\n","Please provide a string:");
  char *buffer = (char *) malloc(INPUT_SIZE * sizeof(char));
	buffer = readIntoBuffer (buffer, INPUT_SIZE);
	char *buffer4 = buffer;
	printf("%s\n","Please provide the first character:");
  char *buffer2 = (char *) malloc(sizeof(char));
  buffer2 = readIntoBuffer (buffer2, sizeof(char));
	printf("%s\n","Please provide the second character:");
	char *buffer3 = (char *) malloc(sizeof(char));
  buffer3 = readIntoBuffer (buffer3, sizeof(char));
	if (buffer && buffer2 && buffer3){
	while ((*buffer)!='\0'){
		if (*buffer == *buffer2){
			*buffer = *buffer3;
		}
		buffer++;
  }
	}
	printf("New word: %s\n", buffer4);
	char *newbuf = (char *) malloc(INPUT_SIZE * sizeof(char));
	readUserInput(newbuf, INPUT_SIZE);
}


int main(void)
{
	printf("%s\n","This is the Mini Shell: ");
	char *buffer = (char *) malloc(INPUT_SIZE * sizeof(char));
	return readUserInput(buffer, INPUT_SIZE);
	// TODO
}
