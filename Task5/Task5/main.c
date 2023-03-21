#include "assert.h"

int main(void)
{
	float devisible = 0;
	float devisor = 0;
	int maxAssertsNum = 0;
	size_t tries = MAX_TRIES;

	printf("Enter max asserts number...\n");
	while (!scanf("%d", &maxAssertsNum) && (tries != 0))
	{
		tries -= 1;
		if (tries == 0)
		{
			FatalError(0, "Fatal input error occurred\n\t", INPUT_ERROR);
		}
		printf(FillColor(YELLOW_COLOR, "Some input error occurred, try again...\n"));
		ClearStdin();
		
	}

	SystemOpen(maxAssertsNum);

	for (size_t i = 0; i < MAX_TRIES; i++)
	{
		Assert(false, "Hello");
	}

	printf("Devising:\n"
		   "Enter the first number...\n"
		   "\tDevisible: ");
	ClearStdin();
	scanf("%F", &devisible);

	printf("Enter the second number...\n"
		   "\tDevisor: ");
	ClearStdin();
	scanf("%F", &devisor);

	FatalError(devisor != 0, "Fatal error, devision by zero:\n\t", 1);

	printf("Answer: %f\n", (devisible / devisor));

	for (size_t i = 0; i < MAX_TRIES; i++)
	{
		Assert(false, "Bye!");
	}

	SystemClose();

	return 0;
}