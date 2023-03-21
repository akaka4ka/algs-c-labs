#include "assert.h"
#include <stdbool.h>
#include <stdio.h>

ASSERTS asserts = {
	NULL,
	0,
	0
};

void AssertFL(bool condition, const char* msg, const char* file, int line)
{
	char** _memCheckMsgs = NULL;
	char* _memCheckMsg = NULL;
	char* fileNamePtr = strrchr(file, '\\') + 1;
	static bool _skip = false;
	
	#pragma region SkipAssertsCheck

	if (_skip == true) return;

	if (asserts.length > 0)
	{
		for (size_t assert = 0; assert < asserts.length; assert++)
		{
			if (strlen(msg) > strlen(asserts.assertMsgs[assert]))
			{
				size_t length = strlen(asserts.assertMsgs[assert]);
				if (!strncmp(msg, asserts.assertMsgs[assert], length))
				{
					return;
				}
			}
			else
			{
				size_t length = strlen(msg);
				if (!strncmp(msg, asserts.assertMsgs[assert], length))
				{
					return;
				}
			}
		}
	}

	#pragma endregion

	if (!condition)
	{
		printf(FillColor(YELLOW_COLOR, "In file: %s on line: %d\n\tAssert with message: %s\n"), fileNamePtr, line, msg);
		if ((asserts._maxLength == INVALID_ASSERTS_NUMBER) || (asserts.length >= asserts._maxLength))
		{
			printf(FillColor(YELLOW_COLOR, "\t~You don't have any spare asserts~\n"));
			printf("\tWhat would you like to do with it?\n"
				   "\t\ta - skip all asserts next time\n"
				   "\t\tq - quit programm\n"
				   "\t\tAnswer: ");
		}
		else
		{
			printf("\tWhat would you like to do with it?\n"
				   "\t\tt - skip this assert next time\n"
				   "\t\ta - skip all asserts next time\n"
				   "\t\tq - quit programm\n"
				   "\t\tAnswer: ");
		}
		char answer = 0;
		while ((answer != 't') && (answer != 'a') && (answer != 'q'))
		{
			ClearStdin();
			answer = getchar();
			switch (answer)
			{
			case 'q':
			{
				if (asserts.assertMsgs != NULL)
				{
					free(asserts.assertMsgs);
				}
				exit(0);

				return;
			} break;

			case 'a':
			{
				_skip = true;
				return;
			} break;

			case 't':
			{
				int msgLength = strlen(msg);
				int memLength = 0;

				#pragma region NewAssertAdd
					
				asserts.length += 1;
				_memCheckMsgs = (char**)realloc(asserts.assertMsgs, asserts.length * sizeof(char*));
				if (_memCheckMsgs != NULL)
				{
					asserts.assertMsgs = _memCheckMsgs;
					asserts.assertMsgs[asserts.length - 1] = NULL;
				}
				else
				{
					asserts.length -= 1;
					printf("\tFailed to memorize an assert. Would you like to continue skipping all asserts next time?\n"
						   "\t\ty - yes\n"
						   "\t\tn - no\n"
						   "\t\tAnswer: ");
					answer = 0;
					while ((answer != 'y') && (answer != 'n'))
					{
						ClearStdin();
						answer = getchar();
						switch (answer)
						{
						case 'y':
						{
							_skip = true;
							return;
						} break;

						case 'n':
						{
							exit(0);
							return;
						} break;

						default:
						{
							printf("\tYou entered an incorrect answer, try it again...\n"
								"\t\tAnswer: ");
						} break;
						}
					}
				}

				if (msgLength > MAX_ASSERT_MSG)
				{
					memLength = MAX_ASSERT_MSG;
				}
				else
				{
					memLength = strlen(msg);
				}

				_memCheckMsg = (char*)realloc(asserts.assertMsgs[asserts.length - 1], (memLength + 1) * sizeof(char));
				if (_memCheckMsg != NULL)
				{
					asserts.assertMsgs[asserts.length - 1] = _memCheckMsg;
				}
				else
				{
					asserts.length -= 1;
					printf("Failed to memorize an assert. Would you like to continue skipping all asserts next time?");
					ClearStdin();
					answer = getchar();
					while ((answer != 'y') && (answer != 'n'))
					{
						ClearStdin();
						answer = getchar();
						switch (answer)
						{
						case 'y':
						{
							_skip = true;
						} break;

						case 'n':
						{
							exit(0);
							return;
						} break;

						default:
						{
							printf("\tYou entered an incorrect answer, try it again...\n"
								   "\t\tAnswer: ");
						} break;
						}
					}
				}

				#pragma endregion


				memset(asserts.assertMsgs[asserts.length - 1], 0, memLength + 1);
				for (size_t i = 0; i < memLength; i++)
				{
					asserts.assertMsgs[asserts.length - 1][i] = msg[i];
				}
			} break;

			default:
			{
				printf("\tYou entered an incorrect answer, try it again...\n"
					   "\t\tAnswer: ");
			} break;
			}
		}
	}
}
void SystemOpen(int maxLen)
{
	if (maxLen > 0)
	{
		asserts._maxLength = maxLen;
		asserts.assertMsgs = NULL;
	}
	else
	{
		asserts._maxLength = INVALID_ASSERTS_NUMBER;
	}
}

void SystemClose()
{
	if (asserts.assertMsgs != NULL)
	{
		free(asserts.assertMsgs);
	}
}