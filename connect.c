#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>

// INFO: Run the program in superuser mode to be able to create socket
int main() {
  struct addrinfo hints = {0}, *res, *p;
  char ipstr[INET6_ADDRSTRLEN];

  hints.ai_family = AF_UNSPEC;
  hints.ai_protocol = SOCK_STREAM;

  int addrinfo_status;
  if ((addrinfo_status = getaddrinfo("www.example.com", 0, &hints, &res)) !=
      0) {
    fprintf(stderr, "ERROR getaddrinfo: %s", gai_strerror(addrinfo_status));
    return 2;
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd < 0) {
    fprintf(stderr, "ERROR socket: %d\n", errno);
    return 2;
  }

  printf("sockfd: %d\n", sockfd);

  if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    fprintf(stderr, "ERROR connect: %d\n", errno);
    return 2;
  }

  printf("Connect success!\n");
  return 0;
}
