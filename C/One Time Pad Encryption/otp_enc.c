#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


int MAX_CHAR = 100000;

// Error function used for reporting issues
void error(const char *msg) { perror(msg); exit(1); }


int main(int argc, char *argv[])
{
  int socketFD, portNumber, charsWritten, charsRead;
  struct sockaddr_in serverAddress;
  struct hostent* serverHostInfo;
    
  // Check argument number and usage
  if (argc < 4){
    fprintf(stderr,"USAGE: %s hostname port\n", argv[0]);
    exit(0);
  }

  char plaintext[MAX_CHAR];
  char key[MAX_CHAR];

  // Get plaintext from file
  FILE* plainFile = fopen(argv[1], "r");
  if (plainFile == NULL)
    fprintf(stderr, "Cannot read file\n");

  memset(plaintext, '\0', MAX_CHAR);

  // Copy over plaintext, check for bad characters
  int plainCount = 0;
  int plainTotal = 0;
  int i = 0;
  while (!feof(plainFile))
  {
    plaintext[i] = fgetc(plainFile);
    
    if (plaintext[i] == '\n')
      break;
    if (!isalpha(plaintext[i]) && plaintext[i] != ' ' && plaintext[i] != EOF)
    {
      fprintf(stderr, "ERROR: plaintext contains bad character: %i at %i\n", plaintext[i], i);
      return 1;
    }

    i++;
    plainTotal++;
  }
  plaintext[strcspn(plaintext, "\n")] = '\0';

  // Get key from file
  FILE* keyFile = fopen(argv[2], "r");
  if (keyFile == NULL)
    fprintf(stderr, "Cannot read file\n");

  memset(key, '\0', MAX_CHAR);

  // Copy over, check for bad characters
  int keyCount = 0;
  int keyTotal = 0;
  i = 0;
  while (!feof(keyFile))
  {
    key[i] = fgetc(keyFile);
    
    if (key[i] == '\n')
      break;
    if (!isalpha(key[i]) && key[i] != ' ' && key[i] != EOF)
    {
      fprintf(stderr, "ERROR: key contains bad characters\n");
      return 1;
    }
    
    i++;
    keyTotal++;
  }
  key[strcspn(key, "\n")] = '\0';

  // Check that key is at least as long as the plaintext
  if (keyTotal < plainTotal)
  {
    fprintf(stderr, "ERROR: Key is not long enough\n");
    return 1;
  }

  // Set up the server address struct
  memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
  portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
  serverAddress.sin_family = AF_INET; // Create a network-capable socket
  serverAddress.sin_port = htons(portNumber); // Store the port number
  serverHostInfo = gethostbyname("localhost");

  if (serverHostInfo == NULL)
  {
    fprintf(stderr, "CLIENT: ERROR, no such host\n");
    exit(0);
  }

  // Copy in the address
  memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); 

  // Set up the socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
  if (socketFD < 0) error("CLIENT: ERROR opening socket");
	
  // Connect to server
  // Connect socket to address
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
  {
    fprintf(stderr, "CLIENT: ERROR connecting to port %i", portNumber);
    return 2;
  }

  // Confirm connected to encode daemon
  char conf[2];
  int testnum = recv(socketFD, conf, 2, 0);
  if (testnum < 0) error("ERROR receiving confirmation");
  if (strcmp(conf, "e\0") != 0)
  {
    fprintf(stderr, "ERROR: attempted to connect to wrong daemon at port %i\n", portNumber);
    return 2;
  }

  char buffer[256];
  memset(buffer, '\0', 256);

  // Send message size to server
  int totalToSend = plainTotal;
  totalToSend = htonl(totalToSend);
  charsWritten = send(socketFD, &totalToSend, sizeof(totalToSend), 0);
  if (charsWritten < 0) error("ERROR sending number");

  // Send plaintext to encoder
  for (i = 0; i < 255; i++)
    buffer[i] = plaintext[i];
  totalToSend = plainTotal;
  int totalSent = 0;
  int numSent = 0;
  int numToSend = 0;
  // Send in chunks, checking for completeness
  while (totalSent < totalToSend)
  {
    if (totalToSend - totalSent > 255)
      numToSend = 255;
    else
      numToSend = totalToSend - totalSent;

    numSent = send(socketFD, buffer, numToSend, 0); // Write to the server
    if (numSent < 0) error("CLIENT: ERROR writing to socket");
    
    totalSent += numSent;
      
    memset(buffer, '\0', 256);
    for (i = totalSent; i < totalSent + numSent; i++)
      buffer[i-totalSent] = plaintext[i];
  }

  // Send message size to server
  numToSend = keyTotal;
  numToSend = htonl(numToSend);
  charsWritten = send(socketFD, &numToSend, sizeof(numToSend), 0);
  if (charsWritten < 0) error("ERROR sending number");

  // Send key to encoder
  memset(buffer, '\0', 256);
  for (i = 0; i < 255; i++)
    buffer[i] = key[i];
  totalToSend = keyTotal;
  totalSent = 0;
  numSent = 0;
  numToSend = 0;
  // Send in chunks, checking for completeness
  while (totalSent < totalToSend)
  {
    if (totalToSend - totalSent > 255)
      numToSend = 255;
    else
      numToSend = totalToSend - totalSent;

    numSent = send(socketFD, buffer, numToSend, 0); // Write to the server
    if (numSent < 0) error("CLIENT: ERROR writing to socket");

    totalSent += numSent;
 
    memset(buffer, '\0', 256);
    for (i = totalSent; i < totalSent + numSent; i++)
      buffer[i-totalSent] = key[i];
  }

  // Get return message from server
  // Get the number of chars to be received
  int totalToRec = 0;
  charsRead = recv(socketFD, &totalToRec, sizeof(totalToRec), 0);
  if (charsRead < 0) error("ERROR reading from socket");
  totalToRec = ntohl(totalToRec);

  char cipher[MAX_CHAR];
  memset(cipher, '\0', MAX_CHAR);
  memset(buffer, '\0', 256);
  // Read encrypted text in chunks, checking for completeness
  int totalRecd = 0;
  int numRecd = 0;
  int numToRec = 0;
  while (totalRecd < totalToRec)
  {
    if (totalToRec - totalRecd > 255)
      numToRec = 255;
    else
      numToRec = totalToRec - totalRecd;

    numRecd = recv(socketFD, buffer, numToRec, 0);
    if (numRecd < 0) error("CLIENT: ERROR reading from socket");

    i = totalRecd;
    while (i < totalRecd + numRecd)
    {
      cipher[i] = buffer[i - totalRecd];
      i++;
    }
    totalRecd += numRecd;
  }
 
  printf("%s\n", cipher);

  fclose(plainFile); // Close files
  fclose(keyFile);
  close(socketFD); // Close the socket
  return 0;
}
