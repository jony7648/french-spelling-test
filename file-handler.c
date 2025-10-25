#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>
#include "file-handler.h"

void saveResults(char wrongWords[100][50], int correctCount, int wrongCount, int totalCount, double grade, char gradeLetter) {
	FILE *fptr;

	time_t currentTime = time(NULL);

	char *outputPath = "previous-tests.txt";

	fptr = fopen(outputPath, "a");

	if (fptr == NULL) {
		printf("File \" %s \" not found!\n", outputPath);
		return;
	}


	fprintf(fptr, "\n%sgrade %d/%d   Grade %f %c\n", 
			ctime(&currentTime), 
			correctCount, 
			totalCount,
			grade,
			gradeLetter
	);

	for (int i=0; i<wrongCount; i++) {
		fprintf(fptr, "%d: %s", i+1, wrongWords[i]);
	}

	fclose(fptr);
}

char *chooseLessonFile(char *rootPath) {
	DIR *d;

	struct dirent *dir;

	char fileArr[50][50];
	char fileBuffer[50];
	int userChoice;
	char *promptMessage;
	char *retStr;

	int maxFileLen = 49;

	int arrIndex = 0;

	promptMessage = "Please choose a lesson file: ";
 
	d = opendir(rootPath);

	if (d && arrIndex < maxFileLen) {
		while ((dir = readdir(d)) != NULL) {
			if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
				continue;
			}
			if (arrIndex > maxFileLen) {
				break;
			}

			printf("%d: %s\n", arrIndex, dir->d_name);
			fileArr[arrIndex][0] = '\0';
			strncpy(fileArr[arrIndex], dir->d_name, maxFileLen);
			arrIndex++;
		}

		closedir(d);
	}
	else {
		return NULL;
	}

	printf("\n%s", promptMessage);
	scanf("%d", &userChoice);	
	
	if (userChoice >= arrIndex) {
		userChoice = arrIndex - 1;
	}
	else if (userChoice < 0) {
		userChoice = 0;
	}

	char *lessonFileName = fileArr[userChoice];

	//add two characters to account for extra / to seperate dir and for the terminating character
	int allocAmount = sizeof(char) * (strlen(rootPath) + strlen(lessonFileName) + 2);

	retStr = (char*)malloc(allocAmount);

	retStr[0] = '\0';

	strcpy(retStr, rootPath);
	strcat(retStr, "/");
	strcat(retStr, lessonFileName);

	int lastCharIndex = strlen(retStr) - 1;

	if (retStr[lastCharIndex] == '\n') {
		retStr[lastCharIndex] = '\0';
	}

	
	//prevent double enters
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);

	return retStr;
}



TestCfg	*newTestCfg() {
	TestCfg *p = malloc(sizeof(TestCfg));

	p->language = NULL;
	p->lessonPath = NULL;

	return p;
}
