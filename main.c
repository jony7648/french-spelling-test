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

double calculateGrade(int correctCount, int questionCount) {
	double grade;
	
	grade = (double)correctCount/ (double)questionCount;


	return grade;

}

int startTest(struct Lesson* lptr) {
	double grade = 5.0f;

	char wrongWords[100][50];
	int wrongWordsLen = 100;

	int correctCount = 0;

	int wrongCount = 0;

	for (int i=0; i<lptr->count; i++) {
		char* engWord = lptr->engArr[i];
		char* frenWord = lptr->frenArr[i];

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

	saveResults(wrongWords, correctCount, wrongCount, grade);

	return grade;


}

int main() {
	struct Lesson* lesson = getLessonData("lesson1.txt");

	//startTest(lesson);
	
	//struct ParsedConfig* cfgptr = parseConfig("lesson1.txt");



	return 0;
}
