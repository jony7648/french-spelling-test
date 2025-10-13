#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <locale.h>
#include <ctype.h>
#include "file-handler.h"
#include "libraries/config_reader.h"

bool promptWord(char *engWord, char *frenWord) {
	char userBuffer[100];
	userBuffer[99] = '\0';

	int maxUserLen = 100;

	printf("English: %s\n\nFrench: ", engWord);

	if (fgets(userBuffer, maxUserLen, stdin) != NULL) {
		//prevent the skiping of iterations
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

char getLetterGrade(float userGrade, char* letterConfigPath) {
	Node *defNode = parseConfig(letterConfigPath);
	Node *currentNode;

	if (defNode == NULL) {
		return '\0';
	}

	currentNode = getChildNode(getArrNode(defNode, "default"));


	// change this part
	

	while (currentNode != NULL) {
		Node *scoreNode = getChildNode(currentNode);

		char *keyValue = (char*)getNodeValue(currentNode);
		char letterGrade = keyValue[0];
		int threshGrade = atoi((char*)getNodeValue(scoreNode));

		if (userGrade >= threshGrade) {
			return letterGrade;
		}

		currentNode = getChildNode(scoreNode);
	}

	return 'F';
}

void lowerCaseWord(char *word) {
	for (int i=0; i<strlen(word); i++) {
		word[i] = tolower(word[i]);
	}
}

int startTest(Node *lessonNode) {

	Node* currentNode;
	
	float grade = 5.0f;

	char wrongWords[100][50];
	int wrongWordsLen = 100;

	int wordCount = getCfgValueCount(lessonNode, "default");

	int correctCount = 0;

	int wrongCount = 0;


	printf("\n\n\n");

	currentNode = getChildNode(getArrNode(lessonNode, "default"));
	currentNode = getChildNode(lessonNode->nodeArr[0]);

	//start test
	while (currentNode != NULL) {
		Node *frenNode = getChildNode(currentNode);
		char *engWord = (char*)getNodeValue(currentNode);
		char *frenWord = (char*)getNodeValue(frenNode);

		lowerCaseWord(frenWord);

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

		currentNode = getChildNode(frenNode);
	}

	//calculate grade
	grade = calculateGrade(correctCount, wordCount);

	printf("\n\nGrade %f\n", grade);

	//Tell user what words to review
	if (wrongCount > 0) {
		printf("\n\nReview the following\n");

		for (int i=0; i<wrongCount; i++) {
			printf("%d: %s", i+1, wrongWords[i]);
		}
	}

	char letterGrade = getLetterGrade(grade, "letter-grades.cfg");

	if (letterGrade == '\0') {
		printf("Error reading letter grade config file!\nFalling back to X\n");
		letterGrade = 'X';
	}	

	saveResults(wrongWords, correctCount, wrongCount, wordCount, grade, letterGrade);

	return grade;

}

int main() {
	setlocale(LC_ALL, "fr-FR");

	Node *cfgNode;
	Node *lessonNode;
	char *lessonPath;

	cfgNode = parseConfig("config.cfg");

	lessonPath = (char*) getCfgValue(cfgNode, NULL, "lesson");

	lessonNode = parseConfig(lessonPath);


	if (lessonNode == NULL) {
		printf("ERROR lesson file failed to read!!\n");
		return 1;
	}

	startTest(lessonNode);

	freeNodeTree(lessonNode);
	return 0;
}
