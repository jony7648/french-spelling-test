#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

struct Lesson {
	char engArr[50][20];
	char frenArr[50][20];
	int count;
};

extern struct Lesson* getLessonData(char* filePath);
extern void saveResults(char wrongWords[100][50], int correctCount, int wrongCount, int totalCount, double score);


#endif
