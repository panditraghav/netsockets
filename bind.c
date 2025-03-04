#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>

int main() {
  struct addrinfo hints = {0}, *res;
  int status;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(0, "3000", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    fprintf(stderr, "bind error: %d\n", errno);
    return 2;
  }

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    fprintf(stderr, "bind error: %d\n", errno);
    return 2;
  }

  printf("Bind successful!\n");

  return 0;
}
