#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TRIES				25
#define MAX_ASSERT_MSG			25
#define INVALID_ASSERTS_NUMBER	-1
#define INPUT_ERROR				1
#define RED_COLOR				"\x1B[31m"
#define YELLOW_COLOR			"\x1B[33m"
#define DEF_COLOR				"\x1b[0m"

typedef struct
{
	char** assertMsgs;
	size_t length;
	size_t _maxLength;
} ASSERTS;

#ifdef NDEBUG

	#define Assert(condition, msg)

#endif

#ifdef _DEBUG

	#define Assert(condition, msg) AssertFL(condition, msg, __FILE__, __LINE__)

	void AssertFL(bool condition, const char* msg, const char* file, int line);

#endif

#define FillColor(clr, msg) clr msg DEF_COLOR
#define FILENAME strrchr(__FILE__, '\\') + 1
#define FatalError(condition, msg, val)	if(!(condition)){ printf(FillColor(RED_COLOR, msg " In file: %s on line: %d"), FILENAME, __LINE__); SystemClose(); return val;}
#define ClearStdin() while (getchar() != '\n')

void SystemOpen(int maxLen);
void SystemClose();