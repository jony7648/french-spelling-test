#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

typedef struct {
	char *language;
	char *lessonPath;
} TestCfg;

extern void saveResults(char wrongWords[100][50], int correctCount, int wrongCount, int totalCount, double grade, char gradeLetter);

extern char *chooseLessonFile(char *dirPath);
extern TestCfg *newTestCfg();

#endif
