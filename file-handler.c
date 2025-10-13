#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "file-handler.h"

void saveResults(char wrongWords[100][50], int correctCount, int wrongCount, int totalCount, double grade, char gradeLetter) {
	FILE* fptr;

	time_t currentTime = time(NULL);

	char* outputPath = "previous-tests.txt";

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
