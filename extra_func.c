#include "monty.h"

/**
 * _strtol - wrapper around strtol, handling monty specific errors
 * @num_string: string that *should* represent an integer
 * @line_number: line counter of monty file
 * Return: long int that strtol converted
 **/

int _strtol(char *num_string, unsigned int line_number)
{
	int base = 10;
	char *endptr;
	long val;


	errno = 0;    /* To distinguish success/failure after call */
	val = strtol(num_string, &endptr, base);

	/* Check for various possible errors */

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
									|| (errno != 0 && val == 0))
	{
		/* perror("strtol"); */
		exit(EXIT_FAILURE);
	}

	if (endptr == num_string)
	{
		/* No digits were found */
		printf("L%d: usage: push integer\n", line_number);
		exit(EXIT_FAILURE);
	}

	/* If we got here, strtol() successfully parsed a number */
	/* return val; */

	if (num_string[0] != '\0')
		if (!isdigit(num_string[0]) && *endptr != '\0')
		/* I don't think this is testing for what I want. Oh, well ... */
		{
			/* trailing alpha on tail of number */
			printf("L%d: usage: push integer\n", line_number);
			exit(EXIT_FAILURE);
		}
	return (val);
}

/**
 * delegate_op - based on tokens from single line, decide what function to call
 * @stack: double pointer to head of stack data structure
 * @op: operator, aka token[0] from getline
 * @line_number: line in byte-code file being processed, zero indexed
 * Return: int for mysterious purposes
 **/
void delegate_op(stack_t **stack, char *op, unsigned int line_number)
{
	int i = 0;
	instruction_t all_ops[] = {
		{"push", instruction_push},
		{"pall", instruction_pall},
		{"pint", instruction_pint},
		{"pop", instruction_pop},
		{"swap", instruction_swap},
		{"add", instruction_add},
		{"nop", instruction_nop},
		{"sub", instruction_sub},
		{"div", instruction_div},
		{"mul", instruction_mul},
		{"mod", instruction_mod},
		{"pchar", instruction_pchar},
		{"pstr", instruction_pstr},
		{"rotl", instruction_rotl},
		{"rotr", instruction_rotr},
		{NULL, NULL}
	};

	for (i = 0; all_ops[i].opcode; i++)
	{
		if (strcmp(op, all_ops[i].opcode) == 0)
		{
			all_ops[i].f(stack, line_number);
			return;
		}
	}
	if (strlen(op) != 0 && op[0] != '#')
	{
		printf("L%u: unknown instruction %s\n", line_number, op);
		exit(EXIT_FAILURE);
	}
	/* success if you got this far */
}
/**
 * process_file - process the whole monty file
 * @filename: str name of monty opcode file
 * @stack: double pointer to top of stack data struct
 * Return: return an error code or success
 **/
int process_file(char *filename, stack_t **stack)
{
	size_t len;
	ssize_t read;
	unsigned int line_number = 0;
	char *line = NULL;
	FILE *fp;
	char *op;


	if (!filename)
	{
		printf("Error: Can't open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("Error: Can't open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	atexit(free_all);
	while ((read = getline(&line, &len, fp)) != -1)
	{
		/*printf("%s", line); */
		op = strtok(line, DELIMS);
		/* printf("<TOKEN1> %s  %u<--\n", op, line_number); */
		line_number++;
		/* printf("%s (%u) ", op, line_number); */
		if (op)
			delegate_op(stack, op, line_number);
	}
	free(line);
	fclose(fp);
	return (EXIT_SUCCESS);
}
