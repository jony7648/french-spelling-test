#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "file-handler.h"

void splitLineBuffer(char* lineBuffer, char engBuffer[20], char frenBuffer[20]) {
	bool foundColon = false;
	int stringEnd = 0;
	int startingOffset = 0;

	//get english buffer
	for (int i=0; i<strlen(lineBuffer); i++) {
		char character = lineBuffer[i];		
		if (foundColon == true) {

			if (character != ' ' && character != '\t') {
				startingOffset = i + 1;
				break;
			}

			continue;
		}
	
		if (character == ':') {
			foundColon = true;
			continue;
		}

		stringEnd = i+1;
		engBuffer[i] = lineBuffer[i];
			
		
	}

	engBuffer[stringEnd] = '\0';


	//get french buffer
	int loopLen = strlen(lineBuffer) - startingOffset;
	for (int i=0; i<loopLen; i++) {
		stringEnd = i+1;
		frenBuffer[i] = lineBuffer[i + startingOffset - 1];
		//printf("%c", lineBuffer[i + startingOffset - 1]);
	}


	frenBuffer[stringEnd] = '\0';
}

struct Lesson* getLessonData(char* filePath) {
	FILE* fptr;

	fptr = fopen(filePath, "r");

	if (fptr == NULL) {
		printf("ERROR: %s could not be found!\n", filePath);
		return NULL;
	}


	struct Lesson* lptr;	
	lptr = (struct Lesson*)malloc(sizeof(struct Lesson));


	char lineBuffer[50];
	char engBuffer[20];
	char frenBuffer[20];
	lineBuffer[49] = '\0';
	engBuffer[19] = '\0';
	frenBuffer[19] = '\0';

	int maxLineCount = 50;
	int maxLineLen = 50;
	int maxWordLenOffset = 2;

	lptr->count = 0;

	while (fgets(lineBuffer, maxLineLen + maxWordLenOffset, fptr)) {
		if (lptr->count >= maxLineCount) {
			break;
		}


		splitLineBuffer(lineBuffer, engBuffer, frenBuffer);

		strcpy(lptr->engArr[lptr->count], engBuffer);
		strcpy(lptr->frenArr[lptr->count], frenBuffer);
		lptr->count++;

		lineBuffer[0] = '\0';
		engBuffer[0] = '\0';
		frenBuffer[0] = '\0';

	}
	

	fclose(fptr);

	return lptr;
}

void saveResults(char wrongWords[100][50], int correctCount, int wrongCount, double score) {
	FILE* fptr;

	time_t currentTime = time(NULL);
	char gradeLetter = 'A';

	char* outputPath = "previous-tests.txt";

	fptr = fopen(outputPath, "a");

	if (fptr == NULL) {
		printf("File not found!\n");
		return;
	}


	fprintf(fptr, "\n%sscore %d/%d   Grade %f %c\n", 
			ctime(&currentTime), 
			correctCount, 
			wrongCount,
			score,
			gradeLetter
	);

	for (int i=0; i<wrongCount; i++) {
		fprintf(fptr, "%d: %s", i+1, wrongWords[i]);
	}


	fclose(fptr);
}

/*
int main() {
	struct Lesson* lptr = getLessonData("lesson1.txt");

	return 0;
}
*/
