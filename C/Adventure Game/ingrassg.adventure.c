#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>


pthread_mutex_t vault = PTHREAD_MUTEX_INITIALIZER;


// Room structure
struct Room
{
  char name[10];
  char type[15];
  int roomNum;
  int totalC;
  char conns[6][10];
};

// Generates a file with a current time stamp
void* giveTime(void* arg)
{
  pthread_mutex_lock(&vault);

  // Time structures
  char timeStr[50];
  memset(timeStr, '\0', 50);
  time_t theNow = time(NULL);
  struct tm* timeDate = localtime(&theNow);
  strftime(timeStr, 49, "%l:%M%P, %A, %B %e, %Y%n", timeDate);
  
  // Create time file
  FILE* timeFile = fopen("currentTime.txt", "w");
  fprintf(timeFile, "%s", timeStr);
  fclose(timeFile);

  pthread_mutex_unlock(&vault);
 
  return NULL;
}


void main()
{
  int i, j;
  struct Room rooms[7];

  // Create thread, pause it with mutex
  pthread_mutex_lock(&vault);
  pthread_t timeThread;
  int resultpThread;
  resultpThread = pthread_create(&timeThread, NULL, &giveTime, NULL);

///////////////////////////////
//// Find Newest Directory ////
///////////////////////////////

  int newestDirTime = -1;			// Initialize time stamp
  char targetDirPre[50] = "ingrassg.rooms.";	// Directory Prefix
  char newestDirName[260];			// Directory name
  memset(newestDirName, '\0', 260);

  DIR* curDir;				// Current directory

  struct dirent* fileInDir;		// Holds the current directory
  struct stat dirAttr;			// Holds the stats for the directory

  curDir = opendir(".");		// Opens the current directory

  // Check current directory was opened
  if (curDir > 0) 
  {
    // Check each entry in directory
    while ((fileInDir = readdir(curDir)) != NULL) 
    {
      // Check entry for prefix
      if (strstr(fileInDir->d_name, targetDirPre) != NULL) 
      {
        stat(fileInDir->d_name, &dirAttr); //Get attributes of the entry
        
	// If this directory is newer
        if ((int)dirAttr.st_mtime > newestDirTime) 
	{
	  newestDirTime = (int)dirAttr.st_mtime;
          memset(newestDirName, '\0', sizeof(newestDirName));
	  strcpy(newestDirName, fileInDir->d_name);
	}
      }
    }
  }

  closedir(curDir);

//////////////////////////////
///// Reading Room Files /////
//////////////////////////////

  // Array of file names
  char fNames[7][260];
  memset(fNames, '\0', 1820);

  // Open the found directory
  DIR* roomDir = opendir(newestDirName);
  struct dirent* roomsInDir;

  // Load Room Files
  if (roomDir > 0) 
  {
    i = 0;
    // Check each entry in directory
    while ((roomsInDir = readdir(roomDir)) != NULL) 
    {
      if (strcmp(roomsInDir->d_name,".") != 0 && 
          strcmp(roomsInDir->d_name, "..") != 0)
      {
        sprintf(fNames[i],"%s/%s", newestDirName, roomsInDir->d_name);
        i++;
      }
      if (i > 6)
        break;
    }
  }

  // Open and parse files to great game graph
  FILE* rFile;
  char temp[50];
  char testC;
  memset(temp, '\0', 50);

  for (i = 0; i < 7; i++)
  {
    rFile = fopen(fNames[i], "r");
    rooms[i].roomNum = i;
    fseek(rFile, 11, SEEK_CUR);
    memset(rooms[i].name, '\0', 10);
    fgets(rooms[i].name, 10, rFile);
    rooms[i].name[strlen(rooms[i].name)-1] = '\0';
    memset(rooms[i].conns, '\0', 60);
    rooms[i].totalC = 0;
    // Parse connections
    while(!feof(rFile))
    {
      if (fgetc(rFile) == 'C')
      {
	fseek(rFile, 13, SEEK_CUR);
        fgets(rooms[i].conns[rooms[i].totalC], 10, rFile);
        rooms[i].conns[rooms[i].totalC][strlen(rooms[i].conns[rooms[i].totalC])-1] = '\0';
        rooms[i].totalC = rooms[i].totalC + 1;
      }
      memset(temp, '\0', 50);
    }
    // Parse room type
    fseek(rFile, -7, SEEK_END);
    if (fgetc(rFile) == 'T')
    {
      fseek(rFile, -11, SEEK_END);
      fgets(rooms[i].type, 15, rFile);
      rooms[i].type[strlen(rooms[i].type)-1] = '\0';
    }
    else
    {
      fseek(rFile, -9, SEEK_END);
      fgets(rooms[i].type, 15, rFile);
      rooms[i].type[strlen(rooms[i].type)-1] = '\0';
    }

    fclose(rFile);
  }

  closedir(roomDir);


/////////////////////////////////////////
///////////// GAME PLAY /////////////////
////////////////////////////////////////
  
  // Get Start Room
  struct Room* startRoom;
  for (i = 0; i < 7; i++)
  {
    if (strcmp(rooms[i].type, "START_ROOM") == 0)
      {
      startRoom = &rooms[i];
      break;
      }
  }

  // Initialize current room pointer and user input string
  struct Room* curRoom = startRoom;
  char userInput[15];
  memset(userInput, '\0', 15);
  int steps = 0;
  char path[255][10];
  memset(path, '\0', 2550);

  while(1)
  {
    printf("CURRENT LOCATION: %s\nPOSSIBLE CONNECTIONS: ", curRoom->name);
    for (i = 0; i < curRoom->totalC; i++)
    {
      printf("%s", curRoom->conns[i]);
      if (i != (curRoom->totalC-1))
        printf(", ");
      else
        printf(".\n");
    }
    printf("WHERE TO? >");
    fgets(userInput, 14, stdin);
    userInput[strlen(userInput)-1] = '\0';
    printf("\n");

    // Time Check
    while (strcmp(userInput, "time") == 0)
    {      
      // Unleash the second thread
      pthread_mutex_unlock(&vault); 

      // Join threads, for divided we fall
      pthread_join(timeThread, NULL);

      // Create waiting thread again
      pthread_mutex_lock(&vault);
      resultpThread = pthread_create(&timeThread, NULL, &giveTime, NULL);

      // Read Time File
      FILE* timeFile = fopen("currentTime.txt", "r");
      char c = fgetc(timeFile);
      while(c != '\n') 
      {
        printf("%c", c);
        c = fgetc(timeFile);
      }
      fclose(timeFile);

      // Continue to receive input
      printf("\n\nWHERE TO? >");
      fgets(userInput, 14, stdin);
      userInput[strlen(userInput)-1] = '\0';
      printf("\n");
    }
    
    // Traverse the rooms
    for (i = 0; i < curRoom->totalC; i++)
    {
      if (strcmp(userInput, curRoom->conns[i]) == 0)
      {
        for (j = 0; j < 7; j++)
        {
          if (strcmp(rooms[j].name, userInput) == 0)
          {
	    strcpy(path[steps], userInput);
	    steps++;
            curRoom = &rooms[j];
            break;
          }
        }
	break;
      }
      if (i == curRoom->totalC - 1)
        printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
    }

    // End room has been reached, game is finished
    if (strcmp(curRoom->type, "END_ROOM") == 0)
    {
      printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
      printf("YOU TOOK %i STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
      for (i = 0; i < steps; i++)
        printf("%s\n", path[i]);

      // Destroy thread - Your threads, give them to me
      pthread_mutex_destroy(&vault);

      exit(0);
    }
    memset(userInput, '\0', 15);
  }
  
}
