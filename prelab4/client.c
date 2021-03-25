// The echo client client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAX          256
#define SERVER_HOST "Nick Keister"
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT  2

// Define variables
struct sockaddr_in  server_addr; 
int sock, r;

// clinet initialization code
int client_init()
{
  printf("======= clinet init ==========\n");
  
  printf("1 : create a TCP socket\n");
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock<0){
     printf("socket call failed\n");
     exit(1);
  }

  printf("2 : fill server_addr with server's IP and PORT#\n");
  server_addr.sin_family = AF_INET;
  //server_addr.sin_addr.s_addr = htonl((127<<24) + 1);
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);

  // Connect to server
  printf("3 : connecting to server ....\n");
  r = connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r== 0){
     printf("connect failed\n");
     exit(3);
  }
  printf("4 : connected OK to\n"); 
  printf("---------------------------------------------------------\n");
  printf("hostname=%s PORT=%d\n", SERVER_HOST, SERVER_PORT);
  printf("---------------------------------------------------------\n");
  printf("========= init done ==========\n");
}

int main(int argc, char *argv[ ])
{
  int n;
  char line[MAX], ans[MAX];

  client_init();

  printf("********  processing loop  *********\n");
  while (1){
    printf("input a line : ");

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% prelab4: part 1 %%%%%%%%%%%%%%%%%%%%5%%%%%%%
  int one, two;
  int arr[2] = {one, two};//array of two integers
  int sum;

  scanf("%d %d", &one, &two);//recieve input of two integers

  for(int i = 0; i< 2; i++)
  {
    n = write(arr[i], line, MAX);//write user input into _fd
    printf("client: wrote n=%d bytes; line=(%s)\n", arr[i], line);

    // Read a line from sock and show it
    n = read(arr[i], ans, MAX);//read user input into _fd
    printf("client: read  n=%d bytes; echo=(%s)\n", arr[i], ans);

    sum += arr[i];//add sum of two integers inputed
  }
    printf("sum of two integers: %d", sum);//print sum of integers
    printf("\n");
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    bzero(line, MAX);                // zero out line[ ]
    fgets(line, MAX, stdin);         // get a line (end with \n) from stdin
    line[strlen(line)-1] = 0;        // kill \n at end
    if (line[0]==0)                  // exit if NULL line
       exit(0);
   }

  return 0;
}


