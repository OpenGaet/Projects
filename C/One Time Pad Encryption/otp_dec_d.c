#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>


int NUM_CHARS = 27;
int MAX_CHARS = 100000;
pid_t pool[1000];

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

void catchSIGTERM(int sig_no)
{
  int i = 0;
  while(pool[i] != -1)
  {
    kill(pool[i], SIGKILL);
    waitpid(pool[i], NULL, 0);
    i++;
  }
  exit(0);
}

int main (int argc, char* argv[])
{
  int k;
  for (k = 0; k < 1000; k++)
    pool[k] = -1;

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &catchSIGTERM;
  sigaction(SIGTERM, &sa, NULL);

  int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
  socklen_t sizeOfClientInfo;
  char plaintext[MAX_CHARS];
  char key[MAX_CHARS];
  char buffer[256];
  struct sockaddr_in serverAddress, clientAddress;

  // Check number of args and usage
  if (argc < 2)
  {
    fprintf(stderr,"USAGE: %s port\n", argv[0]);
    exit(1);
  }

  // Set up the address struct for this process (the server)
  memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
  portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
  serverAddress.sin_family = AF_INET; // Create a network-capable socket
  serverAddress.sin_port = htons(portNumber); // Store the port number
  serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

  // Set up the socket
  listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket 
  if (listenSocketFD < 0)
    error("ERROR opening socket\n");

  // Enable the socket to begin listening
  // Connect socket to port
  if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    error("ERROR on binding\n");

  // Flip the socket on - it can now receive up to 5 connections
  listen(listenSocketFD, 5);

  int j = 0;

  ////////////////////////////////////////////
  ////////// CONTINUALLY RUN SERVER //////////
  ////////////////////////////////////////////
  while (1)
  {
    // Accept a connection, blocking if one is not available until one connects
    sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for client connection
    // Accept
    establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);

    if (establishedConnectionFD < 0)
      error("ERROR on accept");

    // Create Child process
    pool[j] = fork();
    if (pool[j] == 0)  // Child process do the decrypting
    {
      // Send confirmation that this is encode daemon
      char conf[2];
      conf[0] = 'd';
      conf[1] = '\0';
      int testnum = send(establishedConnectionFD, conf, 2, 0); 
      if (testnum < 0) error("ERROR sending to client");
      
      // Get the number of chars to be sent over
      int totalToRec = 0;
      charsRead = recv(establishedConnectionFD, &totalToRec, sizeof(totalToRec), 0);
      if (charsRead < 0) error("ERROR reading from socket");
      totalToRec = ntohl(totalToRec);
      
      // Get the encrypted text from the client
      memset(plaintext, '\0', MAX_CHARS);
      memset(buffer, '\0', 256);
      int i;
      int totalRecd = 0;
      int numRecd = 0;
      int numToRec = 0;
      // Read it in chunks
      while (totalRecd < totalToRec)
      {
        if (totalToRec - totalRecd > 255)
          numToRec = 255;
        else
          numToRec = totalToRec - totalRecd;
      
        numRecd = recv(establishedConnectionFD, buffer, numToRec, 0);
	if (numRecd < 0) error("SERVER: ERROR writing to socket");
      
        i = totalRecd;
        while (i < totalRecd + numRecd)
        {
          plaintext[i] = buffer[i - totalRecd];
          i++;
        }
        totalRecd += numRecd;
      }

      // Get the number of chars to be sent over
      totalToRec = 0;
      charsRead = recv(establishedConnectionFD, &totalToRec, sizeof(totalToRec), 0);
      if (charsRead < 0) error("ERROR reading from socket");
      totalToRec = ntohl(totalToRec);

      // Get key from client
      memset(key, '\0', MAX_CHARS);
      memset(buffer, '\0', 256);
      totalRecd = 0;
      numRecd = 0;
      numToRec = 0;
      // Read key in chunks
      while (totalRecd < totalToRec)
      {
        if (totalToRec - totalRecd > 255)
        numToRec = 255;
        else
        numToRec = totalToRec - totalRecd;
      
        numRecd = recv(establishedConnectionFD, buffer, numToRec, 0);
	if (numRecd < 0) error("SERVER: ERROR writing to socket");
	
      
        i = totalRecd;
        while (i < totalRecd + numRecd)
        {
          key[i] = buffer[i - totalRecd];
          i++;
        }
        totalRecd += numRecd;
      }
      
      // Deccode
      int size = strlen(plaintext);
      int  og, cryp, new;
      for (i = 0; i < size; i++)
      {
        if (plaintext[i] == ' ')
          og = 26;
        else
          og = plaintext[i] - 'A';
        if (key[i] == ' ')
          cryp = 26;
        else
          cryp = key[i] - 'A';
      
        new = (og - cryp);
        if (new < 0)
          new += NUM_CHARS;
        else
          new = new % NUM_CHARS;
      
        if (new == 26)
          plaintext[i] = ' ';
        else
          plaintext[i] = new + 'A';
      }

      // Send message size to client
      int totalToSend = totalToRec;
      totalToSend = htonl(totalToSend);
      int charsWritten = send(establishedConnectionFD, &totalToSend, sizeof(totalToSend), 0);
      if (charsWritten < 0) error("SERVER: ERROR sending number");

      // Send cipher
      for (i = 0; i < 255; i++)
        buffer[i] = plaintext[i];
      totalToSend = totalToRec;
      int totalSent = 0;
      int numSent = 0;
      int numToSend = 0;
      // Send back to client in chunks
      while (totalSent < totalToSend)
      {
        if (totalToSend - totalSent > 255)
          numToSend = 255;
        else
          numToSend = totalToSend - totalSent;

        numSent = send(establishedConnectionFD, buffer, numToSend, 0);
	if (numSent < 0) error("Server: ERROR writing to socket");
	
        totalSent += numSent;
      
        memset(buffer, '\0', 256);
        for (i = totalSent; i < totalSent + numSent; i++)
          buffer[i-totalSent] = plaintext[i];
      }
      
      // Make sure all data has been sent before closing
      int checker = -5;
      do { ioctl(establishedConnectionFD, TIOCOUTQ, &checker); } while (checker > 0);
      
      exit(0);
      kill(getpid(), SIGKILL);
      //exit(0);

    } // this is the end of the child process

    // Limit the number of processes
    j++;
    if (j == 1000)
      error("ERROR: too many processes\n");
    
    close(establishedConnectionFD); // Close the existing socket which is connected to the client
  }

  close(listenSocketFD); // Close the listening socket
  return 0;
}

