// I am become OS
//
// Gaetan Ingrassia
// 2019-05-26
//
// smallsh
//
// This is a less useful, custom shell

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


const int MAX_ARGS = 513;
struct sigaction act_SIGINT = {{0}}, act_SIGTSTP = {{0}};
sigset_t justTSTP;
int noBack = -1;


// SIGNINT handler
void catchSIGINT(int signo)
{
  char* errText = "terminated by signal 2\n";
  write(1, errText, 23);
  fflush(stdout);
}

// SIGTSTP handler
void catchSIGTSTP(int signo)
{
  noBack *= -1;

  if (noBack == 1)
  {
    char* errText = "\nEntering foreground-only mode (& is now ignored)\n";
    write(1, errText, 50);
    fflush(stdout);
  }
  else
  {
    char* errText = "\nExiting foreground-only mode\n";
    write(1, errText, 30);
    fflush(stdout);
  }

  write(1, ": ", 2);
  fflush(stdout);
}


//////////////////////////////////////////////
///////////// BUILT IN COMMANDS //////////////
//////////////////////////////////////////////

//////////////////////////////////////////////
// The exit commnand
void exitShell(pid_t* processes, int numPids)
{
  int temp = 0;

  int i = 0;
  while (processes[i] != -1)
  {
    printf("killing process %i...\n", processes[i]);
    fflush(stdout);
    kill(processes[i], SIGTERM);
    waitpid(processes[i], &temp, 0);
    i++;
  }
  exit(0);
}

//////////////////////////////////////////////
// The change directory command
void changeDir(char* dir)
{
  if (dir == NULL)
  {
    // Change to whatever is in home environment variable
    chdir(getenv("HOME"));
  }
  else
  {
    chdir(dir);
  }

}

//////////////////////////////////////////////
// The status command
int statusOf(int status)
{
  if (WIFEXITED(status))
  {
    status = WEXITSTATUS(status);
    printf("exit value %i\n", status);
    fflush(stdout);
  }
  else
  {
    printf("terminated by signal %i\n", status);
    fflush(stdout);
  }
  return status;
}

//////////////////////////////////////////////
/////////// FUNCTION EXECUTION ///////////////
//////////////////////////////////////////////

//////////////////////////////////////////////
// Execute the given function
int callFunc(char* args[MAX_ARGS], int numArgs, pid_t* pids, int* numPids)
{
  // BLOCK SIGTSTP
  sigprocmask(SIG_BLOCK, &justTSTP, NULL);

  pid_t funcID;
  int childExitStatus = -5;
  int isBack = 0;
  
  
  funcID = fork();

  // Background processes not allowed
  if (noBack == 1 && strcmp(args[numArgs-1], "&") == 0)
  {
    // Remove ampersand
    args[numArgs-1] = NULL;

    // Child
    if (funcID == 0)
    {
      // Check for redirection
      // Check for write file
      int wrt = isWrite(args);
      if (wrt != -1)
      {
        int writeTo = open(args[wrt], O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (writeTo < 0)
	{
	  printf("Failed to open file\n");
	  fflush(stdout);
          exit(1);
	}
	else
	{
	  args[wrt] = NULL;
	  fcntl(writeTo, F_SETFD, FD_CLOEXEC);
	  dup2(writeTo, 1);
	}
      }
      // Check for read file
      int rd = isRead(args);
      if (rd != -1)
      {
        int readFrom = open(args[rd], O_RDONLY, 0600);
	if (readFrom < 0)
	{
	  printf("Failed to open file\n");
	  fflush(stdout);
          exit(1);
	}
	else
	{
	  args[rd] = NULL;
	  fcntl(readFrom, F_SETFD, FD_CLOEXEC);
	  dup2(readFrom, 0);
	}
      }

      // Activate SIGINT
      act_SIGINT.sa_handler = SIG_DFL;
      sigaction(SIGINT, &act_SIGINT, NULL);
      // Ignore SIGTSTP
      act_SIGTSTP.sa_handler = SIG_IGN;
      sigaction(SIGTSTP, &act_SIGTSTP, NULL);
      // Execute program
      if(0 > execvp(args[0], args))
      {
        printf("Error: failed execution\n");
	fflush(stdout);
	exit(1);
      }
      kill(getpid(), SIGTERM);
      exit(0);
    }
  }
  // If background call and allowed
  else if (strcmp(args[numArgs-1], "&") == 0) 
  {
    isBack = 1;
    args[numArgs-1] = NULL;
    
    if (funcID > 0) // Parent
    {
      // Print PID
      printf("background pid %i has started\n", funcID);
      fflush(stdout);
      // Save PID
      pids[*numPids] = funcID;
      *numPids = *numPids + 1;
    }
    else if (funcID == 0) // Child
    {
      // Check for redirection
      // Check for write file
      int wrt = isWrite(args);
      if (wrt != -1)
      {
        int writeTo = open(args[wrt], O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (writeTo < 0)
	{
	  printf("Failed to open file\n");
	  fflush(stdout);
          exit(1);
	}
	else
	{
	  args[wrt] = NULL;
	  fcntl(writeTo, F_SETFD, FD_CLOEXEC);
	  dup2(writeTo, 1);
	}
      }
      else
      {
        // Otherwise direct output to /dev/null
        int devNull = open("/dev/null", O_WRONLY);
        dup2(devNull, 1);
      }
      // Check for read file
      int rd = isRead(args);
      if (rd != -1)
      {
        int readFrom = open(args[rd], O_RDONLY, 0600);
	if (readFrom < 0)
	{
	  printf("Failed to open file\n");
	  fflush(stdout);
          exit(1);
	}
	else
	{
	  args[rd] = NULL;
	  fcntl(readFrom, F_SETFD, FD_CLOEXEC);
	  dup2(readFrom, 0);
	}
      }
      else
      {
        // Otherwise direct input to take from /dev/null
        int devNullIn = open("/dev/null", O_RDONLY);
        dup2(devNullIn, 0);
      }

      // Ignore SIGINT
      act_SIGINT.sa_handler = SIG_IGN;
      sigaction(SIGINT, &act_SIGINT, NULL);
      // Ignore SIGTSTP
      act_SIGTSTP.sa_handler = SIG_IGN;
      sigaction(SIGINT, &act_SIGTSTP, NULL);
      // Execute program
      if(0 > execvp(args[0], args))
      {
        printf("Error: failed execution\n");
	fflush(stdout);
	exit(1);
      }
      kill(getpid(), SIGTERM);
      exit(0);
    }
  }
  else
  {
    // Child process
    if (funcID == 0)
    {
      // Check for redirection
      // Check for write file
      int wrt = isWrite(args);
      if (wrt != -1)
      {
        int writeTo = open(args[wrt], O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (writeTo < 0)
	{
	  printf("Failed to open file\n");
	  fflush(stdout);
          exit(1);
	}
	else
	{
	  args[wrt] = NULL;
	  fcntl(writeTo, F_SETFD, FD_CLOEXEC);
	  dup2(writeTo, 1);
	}
      }
      // Check for read file
      int rd = isRead(args);
      if (rd != -1)
      {
        int readFrom = open(args[rd], O_RDONLY, 0600);
	if (readFrom < 0)
	{
	  printf("Failed to open file\n");
	  fflush(stdout);
          exit(1);
	}
	else
	{
	  args[rd] = NULL;
	  fcntl(readFrom, F_SETFD, FD_CLOEXEC);
	  dup2(readFrom, 0);
	}
      }

      // Activate SIGINT handler with Default
      act_SIGINT.sa_handler = SIG_DFL;
      sigaction(SIGINT, &act_SIGINT, NULL);
      // Ignore SIGTSTP
      act_SIGTSTP.sa_handler = SIG_IGN;
      sigaction(SIGTSTP, &act_SIGTSTP, NULL);
      // Execute program
      if(0 > execvp(args[0], args))
      {
        printf("Error: failed execution\n");
	fflush(stdout);
	exit(1);
      }
      kill(getpid(), SIGTERM);
      exit(0);
    }
  }

  // Parent process for foreground child
  if (isBack == 0 && funcID > 0)
  {
    waitpid(funcID, &childExitStatus, 0);
    if (childExitStatus == 2)
    {
      printf("\nterminated by signal 2\n");
      fflush(stdout);
    }
  }

  // UNBLOCK SIGTSTP
  sigprocmask(SIG_UNBLOCK, &justTSTP, NULL);
  
  return childExitStatus;
}

//////////////////////////////////////////////
// Tokenize command line arguments
int getArgs(char* cLine, char* args[MAX_ARGS])
{
  char* temp = NULL;
  const char space[3] = " \n\0";

  int idx = 0;
  temp = strtok(cLine, space);
  while (temp != NULL)
  {
    args[idx] = temp;
    idx++;

    if (idx == MAX_ARGS - 1)
    {
      args[idx] = NULL;
      break;
    }

    temp = strtok(NULL, space);
  }

  args[idx] = NULL;
  return idx;
}

///////////////////////////////////////////////
// Check background processes statuses
void checkBack(pid_t* pids)
{
  int status = 5;
  int i = 0;
  while (pids[i] != -1)
  {
    if (waitpid(pids[i], &status, WNOHANG) != 0)
    {
      printf("background pid %i is done: ", pids[i]);
      fflush(stdout);
      statusOf(status);
      pids[i] = -1;
    }
    i++;
  }

}

////////////////////////////////////////////
// Check for write file, remove redirection operator
int isWrite(char* args[MAX_ARGS])
{
  int i = 0;
  while (args[i] != NULL)
  {
    if (strcmp(args[i], ">") == 0)
    {
      args[i] = args[i+1];
      return i;
    }
    i++;
  }

  return -1;
}

////////////////////////////////////////////
// Check for read file, remove redirection operator 
int isRead(char* args[MAX_ARGS])
{
  int i = 0;
  while (args[i] != NULL)
  {
    if (strcmp(args[i], "<") == 0)
    {
      args[i] = args[i+1];
      return i;
    }
    i++;
  }

  return -1;
}

///////////////////////////////////////////////
// Expand any '$$' in input to display the PID
void expandDollar(char lineIn[])
{
  char pid[20];
  memset(pid, '\0', 20);
  sprintf(pid, "%i", getpid());
  int pidLen = strlen(pid);

  // Search and replace '$$'
  char* loc = strstr(lineIn, "$$");
  while (loc != NULL)
  {
    strcpy(loc + pidLen, loc + 2);
    strncpy(loc, pid, pidLen);
    loc = strstr(lineIn, "$$");
  }
}


int main()
{
  // Initialize variable
  size_t buffSize = 2048;
  char* cLine = NULL;
  int numArgs = 0;
  int exitStatus = 0;
  char inputBuff[3000];
  memset(inputBuff, '\0', 3000);
  char* args[MAX_ARGS];

  int j;
  for (j = 0; j < MAX_ARGS; j++)
    args[j] = NULL;

  pid_t pids[1000];
  memset(pids, -1, sizeof(pids));
  int numPids = 0;

  // Signal Handlers
  // Create set with only SIGTSTP in it
  sigemptyset(&justTSTP);
  sigaddset(&justTSTP, SIGTSTP);

  // Initialize catch for SIGINT
  act_SIGINT.sa_handler = SIG_IGN; //catchSIGINT;
  sigfillset(&act_SIGINT.sa_mask);
  act_SIGINT.sa_flags = SA_RESTART;

  // Inititalize catch for SIGTSTP
  act_SIGTSTP.sa_handler = catchSIGTSTP;
  sigfillset(&act_SIGTSTP.sa_mask);
  act_SIGTSTP.sa_flags = SA_RESTART;

  // Activate handlers
  sigaction(SIGINT, &act_SIGINT, NULL);
  sigaction(SIGTSTP, &act_SIGTSTP, NULL);

  while (1)
  {
    // Check for completed backgroung processes
    checkBack(pids);
    
    // Command prompt
    write(1, ": ", 2);
    fflush(stdout);
    getline(&cLine, &buffSize, stdin);

    // Expand any '$$' to the PID
    strcpy(inputBuff, cLine);
    expandDollar(inputBuff);
    strcpy(cLine, inputBuff);

    // Tokenize input
    cLine[strcspn(cLine, "\n")] = '\0';
    numArgs = getArgs(cLine, args);

    // Handle empty lines, comments, built-ins, and functions
    if (args[0] == NULL || args[0][0] == '#'){}
    else if (strcmp(args[0], "exit") == 0)
      exitShell(pids, numPids);
    else if (strcmp(args[0], "cd") == 0)
      changeDir(args[1]);
    else if (strcmp(args[0], "status") == 0)
      statusOf(exitStatus);
    else
      exitStatus = callFunc(args, numArgs, pids, &numPids);

    // Clear out argument list
    int i = 0;
    while (args[i] != NULL)
    {
      args[i] = NULL;
      i++;
    }  

    free(cLine);
    cLine = NULL;
  }
}
