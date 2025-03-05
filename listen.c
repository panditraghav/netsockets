#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>

int BACKLOG = 10;

int main() {
  struct addrinfo hints = {0}, *res;

  hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // Use localhost as IP

  int addrinfo_response;
  if ((addrinfo_response = getaddrinfo(0, "3000", &hints, &res)) != 0) {
    fprintf(stderr, "[ERROR] getaddrinfo: %s\n",
            gai_strerror(addrinfo_response));
    return 2;
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd < 0) {
    fprintf(stderr, "[ERROR] socket: %d\n", errno);
    return 2;
  }

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    fprintf(stderr, "[ERROR] bind: %d\n", errno);
    return 2;
  }
  listen(sockfd, BACKLOG);
  // then use accept() to accept new connections

  return 0;
}
