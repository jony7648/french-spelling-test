#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include "file-handler.h"
#include "config-reader.h"

bool promptWord(char* engWord, char frenWord[20]) {
	char userBuffer[20];
	userBuffer[19] = '\0';
	
	printf("English: %s\n\nFrench: ", engWord);

	if (fgets(userBuffer, sizeof(userBuffer), stdin) != NULL) {
		int len = strlen(userBuffer);	

		if (len > 0 && userBuffer[len - 1] != '\n') {
			int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
			frenWord[len - 1] = '\0';

		}
	}


	strcat(frenWord, "\n");
	printf("\n");


	if (strcmp(userBuffer, frenWord) == 0) {
		return true;
	}

	return false;
}

float calculateGrade(int correctCount, int questionCount) {
	float grade;
	
	grade = (float)correctCount/ (float)questionCount * 100;


	return grade;

}

char getLetterGrade(float userGrade, char* letterGradePath) {
	struct ParsedConfig* defPtr = parseConfig("letter-grades.cfg");

	for (int i=0; i<defPtr->count; i++) {
		char letterGrade = defPtr->keyArr[i][0];
		int threshGrade = atoi(defPtr->valueArr[i]);

		if (userGrade >= threshGrade) {
			return letterGrade;
		}


	}

	return 'F';
}

int startTest(struct ParsedConfig* lptr) {
	float grade = 5.0f;

	char wrongWords[100][50];
	int wrongWordsLen = 100;

	int correctCount = 0;

	int wrongCount = 0;

	for (int i=0; i<lptr->count; i++) {
		char* engWord = lptr->keyArr[i];
		char* frenWord = lptr->valueArr[i];

		bool correct = promptWord(engWord, frenWord);

		if (correct == true) {
			correctCount++;
		}
		else if (wrongCount >= wrongWordsLen) {
			continue;
		}
		else {
			strcpy(wrongWords[wrongCount], frenWord);
			wrongCount++;	
		}
	}

	grade = calculateGrade(correctCount, lptr->count);

	printf("\n\nGrade %f\n", grade);

	//Tell user what words to review
	if (wrongCount == 0) {
		return grade;	
	}

	printf("\n\nReview the following\n");

	for (int i=0; i<wrongCount; i++) {
		printf("%d: %s", i+1, wrongWords[i]);
	}

	saveResults(wrongWords, correctCount, wrongCount, lptr->count, grade);

	return grade;


}

int main() {
	struct ParsedConfig* cfgptr = parseConfig("config.cfg");

	char* lessonPath = getConfigValue("lesson", cfgptr);

	struct ParsedConfig* lesson = parseConfig(lessonPath);

	char letterGrade = getLetterGrade(80, "letter-grades.cfg");

	printf("%c\n", letterGrade); 


	//if strcmp(getConfigValue("lesson.txt", cfgptr), ")
	
	//free(cfgptr);
	startTest(lesson);

	free(lesson);





	return 0;
}
