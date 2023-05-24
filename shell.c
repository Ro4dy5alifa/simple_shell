#include "main.h"

/**
 * parse_input - Parses user input into individual command
 * @input: User input
 * @args: Array
 *
 * Return: Number of commands and arguments
 */

int parse_input(char *input, char **args)
{
	int inde = 0;

	args[inde] = strtok(input, " ");
	while (inde < 10 && args[inde] != NULL)
	{
		inde++;
		args[inde] = strtok(NULL, " ");
	}
	return (inde);
}

/**
 * search_command - Searches and executes the specified command
 * @args: Array of arguments
 *
 * Return: None
 */

void search_command(char **args)
{
	char *command, *path, *path_copy, *directory, *full_command;

	command = strdup(args[0]);
	if (command == NULL)
		exit(1);

	if (access(command, X_OK) == -1)
	{
		path = getenv("PATH");
		path_copy = strdup(path);
		directory = strtok(path_copy, ":");

		while (directory != NULL)
		{
			full_command = malloc(strlen(directory) + strlen(command) + 2);
			if (full_command == NULL)
				exit(1);
			sprintf(full_command, "%s/%s", directory, command);
			if (access(full_command, X_OK) == 0)
			{
				args[0] = full_command;
				break;
			}
			free(full_command);
			directory = strtok(NULL, ":");
		}
		free(path_copy);
		free(command);
	}

	if (execvp(args[0], args) == -1)
	{
		printf("%s: command not found\n", args[0]);
		exit(1);
	}
}

/**
 * print_environment - Prints the current environment
 *
 * Return: None
 */

void print_environment(void)
{
	int inde;

	inde = 0;
	while (environ[inde] != NULL)
	{
		printf("%s\n", environ[inde]);
		inde++;
	}
}

/**
 * main - Main function
 *
 * Return: Returns 0
 */

int main(void)
{
	char input[150];
	char *args[20];
	pid_t pid;
	int num;

	while (1)
	{
		if (fgets(input, 150, stdin) == NULL)
			exit(0);

		input[strcspn(input, "\n")] = 0;

		if (strcmp(input, "exit") == 0)
			break;
		else if (strcmp(input, "env") == 0)
		{
			print_environment();
			continue;
		}

		pid = fork();
		if (pid == 0)
		{
			num = parse_input(input, args);
			args[num] = NULL;

			search_command(args);
		}
		else
			wait(NULL);
	}
	return (0);
}

