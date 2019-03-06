// Chris Fietkiewicz. Solution to Problem #2 (server program).
// Usage: server port
// Example: server 8000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

// Helper function to conveniently print to stderr AND exit (terminate)
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
     // Check for proper number of commandline arguments
     // Expect program name in argv[0], port # in argv[1]
     if (argc < 2) { 
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
	 
     const char *question = "What is the capitol of Greece?";
     const char *answer = "Athens";

     // Setup phase
     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     struct sockaddr_in serv_addr; // Server address struct
     bzero((char *) &serv_addr, sizeof(serv_addr));
     int portno = atoi(argv[1]); // Port number is commandline argument
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
          error("ERROR on binding");
     listen(sockfd, 5);

     // Service phase
     struct sockaddr_in cli_addr;
     socklen_t clilen = sizeof(cli_addr); // Address struct length
     int newsockfd = accept(sockfd, 
                     (struct sockaddr *) &cli_addr, 
                     &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
	
	// Play the game
	int n; // number of bytes that were written/read
	n = write(newsockfd, question, strlen(question));
	if (n < 0) 
		error("ERROR writing to socket");
	printf("Sent question.\n");
	char buffer[256];
	bzero(buffer, sizeof(buffer));
	n = read(newsockfd, buffer, sizeof(buffer));
	if (n < 0) 
		error("ERROR reading from socket");
	printf("Answer received: %s\n", buffer);
	if (!strcmp(answer, buffer))
		sprintf(buffer, "Correct!");
	else
		sprintf(buffer, "Wrong");
	n = write(newsockfd, buffer, sizeof(buffer));
	if (n < 0) 
		error("ERROR writing to socket");
	close(newsockfd);
	close(sockfd);

	return 0;
}