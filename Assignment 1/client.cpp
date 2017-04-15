/* CLIENT */

// Header Files
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>

using namespace std;

// Maintains a Timer for sending the messages on the basis of their timestamp.
int timer = 0;


// -------------------------------------------------------------------------------------------------------------
//
// FUNCTION : 
//    main() : 
//                The main function creates a socket for every client with the Server.
//                It reads the transacation.txt file and sends each transaction to the server line by line.
//                On sending the transaction line, it waits for an acknowledgment from the server regarding the 
//                modification of the account.
//                      
// -------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // To maintain the time spent on running the client
  clock_t start = clock();

  // Initializing the Socket Variables
  int sockfd, portno, n;
  struct sockaddr_in server_addr;
  struct hostent *server;
  char buffer[256];
  portno = atoi(argv[1]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  if (sockfd < 0)
  { 
    perror("ERROR opening socket");
  } 

  server = gethostbyname(argv[2]);
  
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr,server->h_length);
  server_addr.sin_port = htons(portno);



  if (connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) 
  {
    perror("ERROR connecting");
  }

  		    // string message = "20"; 
  string message = to_string(atoi(argv[3]));
  // srand(1);
  // string message = to_string(rand()%100);
	
	cout << "*********************************************" << endl;
	cout << "Original Time : " << message << endl;
	message.append("\0");

	//const char *cstr = message.c_str();
	n = write(sockfd,message.data(),message.size());
      cout << "Sending Process Time to Time Daemon" << endl;
      if (n < 0)
        { 
          perror("ERROR writing to socket");
        }

      // Waits for an acknowledgement from the Server about the status of the Transaction sent.
      // cout << "ConnFd" << sockfd << endl;
      n = read(sockfd,buffer,255);
      cout << "Receiving Clock Adjustment from Time Daemon" << endl;
      if (n < 0) {
      	cout << errno << endl;
         perror("ERROR reading from socket");

      }
      int o_time = stoi(message);
      int adjust_time = atoi(buffer);
      // cout << "Original Time : " << message << endl;
      printf("\nAdjust original clock by the following amount: %d\n",(adjust_time-o_time));

      cout << "New synchronized time : " << adjust_time << endl;
      bzero(buffer,256);
  }
