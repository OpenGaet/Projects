#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


int NUM_ROOMS = 7;
int MAX_CON = 6;


// Room Structure
struct Room
{
  char* name;  
  char* type;  
  int  roomNum;
  int totalC;
  int conns[6];
};

// Returns a random integer between 0 and the argument
int randNum(int lim)
{
  return rand() % lim;
}

// Returns a random Room, does NOT validate if connection can be added
struct Room* GetRandomRoom(struct Room graph[])
{
  int idx = rand() % NUM_ROOMS;
  return &graph[idx];
}

// Returns true(1) if a connection can be added from Room x (< 6 outbound connections), false(0) otherwise
int CanAddConnectionFrom(struct Room x) 
{
  if (x.totalC < MAX_CON)
    return 1;

  return 0;
}

// Returns true if a connection from Room x to Room y already exists, false otherwise
int ConnectionAlreadyExists(struct Room x, struct Room y)
{
  int i;
  // Check every connection x has
  for (i = 0; i < MAX_CON; i++)
  {
    if (x.conns[i] == y.roomNum)
      return 1;
  }
  
  return 0;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct Room* x, struct Room* y) 
{
  int i;
  // Places connection at next available position in connection array
  for (i = 0; i < MAX_CON; i++)
  {
    if (x->conns[i] == -1)
    {
      x->conns[i] = y->roomNum;
      x->totalC++;
      break;
    }
  }
}

// Returns true if Rooms x and y are the same Room, false otherwise
int IsSameRoom(struct Room x, struct Room y)
{
  if (strcmp(x.name, y.name) == 0)
    return 1;

  return 0;
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection(struct Room graph[])  
{
  struct Room* A;
  struct Room* B;

  while(1)
  {
    A = GetRandomRoom(graph);

    if (CanAddConnectionFrom(*A) == 1)
    break;
  }
  // Finds possible valid connections for A
  do
  {
    B = GetRandomRoom(graph);
  }while(CanAddConnectionFrom(*B) == 0 || IsSameRoom(*A, *B) == 1 || ConnectionAlreadyExists(*A, *B) == 1);

  ConnectRoom(A, B);
  ConnectRoom(B, A);

}

// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
int IsGraphFull(struct Room graph[])
{
  int i;
  for (i = 0; i < NUM_ROOMS; i++)
  {
    if (graph[i].totalC < 3)
      return 0;
  }
  return 1;
}


void main()
{
  // Random seed
  time_t timeCur = time(0);
  srand(timeCur);

  // Create Directory with PID
  char dirName[50];
  memset(dirName, '\0', 50);
  sprintf(dirName, "ingrassg.rooms.%d", getpid());
  mkdir(dirName, 0777);

  // Create Names
  char names[10][9];
  memset(names, '\0', 90);
  strcpy(names[0],"Casterly");
  strcpy(names[1],"Meereen");
  strcpy(names[2],"Braavos");
  strcpy(names[3],"Dorne");
  strcpy(names[4],"Quarth");
  strcpy(names[5],"Valyria");
  strcpy(names[6],"Oldtown");
  strcpy(names[7],"Eyrie");
  strcpy(names[8],"Riverrun");
  strcpy(names[9],"IronIsle");

  // Create room graph
  struct Room graph[NUM_ROOMS];
  int i, j;

  // Start Room
  int idxName = randNum(10);
  graph[0].name = names[idxName];
  graph[0].roomNum = 0;
  graph[0].type = "START_ROOM";
  graph[0].totalC = 0;
  for (i = 0; i < 6; i++)
    graph[0].conns[i] = -1;

  // Middle Rooms
  for (i = 1; i < 6; i++)
  {
    idxName++;        
    if (idxName == 10)
      idxName = 0;    
    graph[i].name = names[idxName];
    graph[i].roomNum = i;
    graph[i].type = "MID_ROOM";
    graph[i].totalC = 0;
    for (j = 0; j < 6; j++)
      graph[i].conns[j] = -1;
  }

  // End Room
  idxName++;        
  if (idxName == 10)
    idxName = 0;    
  graph[6].name = names[idxName];
  graph[6].roomNum = 6;
  graph[6].type = "END_ROOM";
  graph[6].totalC = 0;
  for (i = 0; i < 6; i++)
    graph[6].conns[i] = -1;

  // Create all connections in graph
  while (IsGraphFull(graph) == 0)
    AddRandomConnection(graph);

  // Create Room files
  FILE* rfile;
  char rfileName[260];
  memset(rfileName, '\0', 260);

  for (i = 0; i < NUM_ROOMS; i++)
  {
    sprintf(rfileName,"%s/%s", dirName, graph[i].name);
    rfile = fopen(rfileName, "w");

    fprintf(rfile, "ROOM NAME: %s\n", graph[i].name);
    for (j = 0; j < graph[i].totalC; j++)
      fprintf(rfile, "CONNECTION %d: %s\n", j+1, graph[graph[i].conns[j]].name);
    fprintf(rfile,"ROOM TYPE: %s\n", graph[i].type);
    
    fclose(rfile);
    memset(rfileName, '\0', 260);
  }

}
