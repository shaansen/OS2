// Name : Shantanu Sengupta
// SERVER

// Header Files
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>
#include <time.h> 
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


// MAXIMUM NUMBER OF CLIENTS ACCEPTED BY THE SOCKET
#define MAX_CLIENTS 5
// MAXIMUM NUMBER OF ACCOUNTS THAT CAN BE HANDLED BY THE SOCKET
#define MAX_ACCOUNTS 100000


// BUFFER DEFINITION
char buffer[256];
int local_time[MAX_CLIENTS+1];
int local_time_index = 0;

// MUTEX DEFINITION
pthread_mutex_t mtx_1;


// -------------------------------------------------------------------------------------------------------------
//
// FUNCTION : 
//    5. threadHandler() : 
//                      A threadhandler() is created for every Client Socket that tries to communicate 
//                      with the Server Socket. 
//                      On connecting with a Client, this handler sends an acknowldgement of the connection 
//                      being established. And then it reads every transaction sent by the client and performs
//                      modifications in the Account Balance of the Account.
//                      On completion of every modification, it sends an acknowledgement to the Client.
//                      
// -------------------------------------------------------------------------------------------------------------

void *threadHandler(void *arg)
{
    // Read Connection ID from the Argument 
    int *parameter = (int *)arg;
    int connFd = *parameter;
    cout << "Thread No: " << pthread_self() << endl;
    int x = pthread_self();
       // Read Transactions from Client
        bzero(buffer,256);
        int n = read(connFd,buffer,255);
        if (n < 0) {
          perror("ERROR reading from socket\n");
          exit(EXIT_FAILURE);
        }

        string temp(buffer);
        cout << "Buffer : " << temp << endl;

        local_time[local_time_index] = stoi(temp);
        cout << "After stoi " << local_time[local_time_index]  <<
        local_time_index++;
     
    // Close Connection
    close(connFd);
}

// -------------------------------------------------------------------------------------------------------------
//
// FUNCTION : 
//    5. main() : 
//                The main function creates a socket for the Server and binds an address for it.
//                For every connection accepted by the Server, it spawns a thread to enable multithreading
//                connection for all the clients.
//                      
// -------------------------------------------------------------------------------------------------------------

int calculate_time_average(int local_time[])
{
	int sum = 0;
	for (int i = 0; i <= local_time_index; i++)
	{
		sum += local_time[i];
	}
	cout << "Sum : " << sum << endl;
	cout << "local_time : " << local_time_index << endl;
	return sum/local_time_index;
}

int main(int argc, char* argv[])
{

  int time_daemon_indicator = atoi(argv[4]);
  if(time_daemon_indicator==0)
  {
  cout << "Time Daemon Indicator : " << time_daemon_indicator << endl;
// Initialize Variables to create Socket
  int port_number, sockFD, connFd;
  int *temp_sock;
  socklen_t len; 
  struct sockaddr_in server_address, client_address;
  pthread_t thread_clients[MAX_CLIENTS];
  port_number = atoi(argv[1]);
 
  // Create Socket, bind it with an address and Listed for Connections
  sockFD = socket(AF_INET, SOCK_STREAM, 0);
  if(sockFD < 0)
  {
      cerr << "Cannot open socket" << endl;
      return 0;
  }
  
  bzero((char*) &server_address, sizeof(server_address));
  
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port_number);

  //Bind socket
  if(bind(sockFD, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
  {
      cerr << "Cannot bind" << endl;
      return 0;
  }
  
  listen(sockFD, MAX_CLIENTS);
  len = sizeof(client_address);
  int number_of_threads = 0;

  while (number_of_threads < MAX_CLIENTS)
  {
      // Accept Connections
      connFd = accept(sockFD, (struct sockaddr *)&client_address, &len);

      if (connFd < 0)
      {
          cerr << "Cannot accept connection" << endl;
          return 0;
      }
      else
      {
        cout << "\nCONNECTION ESTABLISHED BETWEEN CLIENT AND SERVER \n";
      }
      temp_sock = (int *)malloc(1);
      *temp_sock = connFd;

      //cout << "Creating Thread with Connector Descriptor : " << connFd << endl;

      // Creates a thread for every Transaction
      pthread_create(&thread_clients[number_of_threads], NULL, threadHandler, (void *)temp_sock); 
      number_of_threads++;
  }
  
  // Closes all the Threads
  for(int i = 0; i < MAX_CLIENTS; i++)
  {
      pthread_join(thread_clients[i], NULL);
  }

  local_time[local_time_index] = atoi(argv[3]);
  cout << "Server Local Time : " << local_time[local_time_index] << endl;
  cout << "Average Calculated Time : " << calculate_time_average(local_time) << endl;

  
  return 0; 
}
else
{
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
  cout << portno << " " << server << endl;
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
	cout << message << endl;
	cout << "*********************************************" << endl;
	message.append("\0");

	//const char *cstr = message.c_str();
	n = write(sockfd,message.data(),message.size());
      cout << "Completed Writing to the socket" << endl;
      if (n < 0)
        { 
          perror("ERROR writing to socket");
        }

      // Waits for an acknowledgement from the Server about the status of the Transaction sent.
        bzero(buffer,256);
}

}