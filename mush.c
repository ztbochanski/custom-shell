// Zachary Bochanski
// mush "mega underwhelming shell" implementation for fun!

#include <stdio.h>    // for printf(), fgets(), perror()
#include <stdlib.h>   // for exit()
#include <string.h>   // for strtok(), strcmp()
#include <unistd.h>   // for fork(), execvp()
#include <sys/wait.h> // for wait()

#define BUFF_SIZE 1024

void initialize_shell(void)
{
  printf("-------------------------------------------\n");
  printf("***MUSH - The Mega Underwhelming Shell***\n");
  printf("...much caution, high underpower\n");
  printf("-------------------------------------------\n");
}

char *read_input(void)
{
  char *input = NULL;
  size_t buffer = 0;
  getline(&input, &buffer, stdin);
  return input;
}

char **parse_input(char *input)
{
  int buffer = 32;
  char *token;
  char **tokens = malloc(buffer * sizeof(char *));
  int array_index = 0;

  token = strtok(input, " \n");
  while (token != NULL)
  {
    tokens[array_index] = token;
    token = strtok(NULL, " \n");
    tokens = realloc(tokens, buffer * sizeof(char *));
    array_index++;
  }
  tokens[array_index] = NULL;
  return tokens;
}

int start_process(char **arguments)
{
  pid_t process_id;
  char *builtin = "cd";

  if (strcmp(arguments[0], builtin) == 0)
  {
    if (chdir(arguments[1]) != 0)
    {
      perror("mush");
    }
  }
  else
  {
    process_id = fork(); // start up a new process
    if (process_id == 0) // child processes are 0
    {
      if (execvp(arguments[0], arguments) == -1)
      {
        perror("mush");
      }
      exit(1);
    }
    else
    {
      wait(NULL);
    }
  }
  return 1;
}

void shell_loop()
{
  int shell_state = 1;
  char *prompt = "> ";
  char *input;
  char **command_arguments;

  while (shell_state)
  {
    printf("%s", prompt);
    input = read_input();
    command_arguments = parse_input(input);
    printf("%s\n", command_arguments[0]);
    shell_state = start_process(command_arguments);
  }
}

int main()
{
  initialize_shell();
  shell_loop();
}
