#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int preventIterSkip(char *inputBuffer) {
	int len = strlen(inputBuffer);	

	if (len > 0 && inputBuffer[len - 1] != '\n') {
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
	}

	return len;
}

void clearScreen() {
	#ifdef _WIN32
		system("cls");
	#elif __APPLE__
		system("clear");
	#elif __linux__
		system("clear");
	#endif
}
