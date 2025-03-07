#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT "3490"
#define BACKLOG 10

void sigchld_handler(int s) {
  (void)s; // quiet unused variable warning

  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;

  errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void) {
  int sockfd, newfd;
  struct addrinfo hints = {0}, *servinfo, *p;
  struct sockaddr_storage their_addr;

  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(0, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "[ERROR] getaddrinfo(): %s\n", gai_strerror(rv));
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("[ERROR]: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
      perror("[ERROR]: setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("[ERROR] bind\n");
      continue;
    }
    break;
  }
  freeaddrinfo(servinfo);

  if (p == NULL) {
    fprintf(stderr, "[ERROR]: server faild to bind");
    exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1) {
    perror("[ERROR] listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("[INFO]: waiting for connections...\n");

  while (1) {
    sin_size = sizeof their_addr;
    newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (newfd == -1) {
      perror("[ERROR]: accept\n");
      continue;
    }
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              s, sizeof s);
    printf("[INFO] got connection from %s\n", s);

    if (!fork()) {   // this is the child process
      close(sockfd); // child doesn't need the listener
      if (send(newfd, "Hello, world!", 13, 0) == -1)
        perror("send");
      close(newfd);
      exit(0);
    }
    close(newfd); // parent doesn't need this
  }

  return 0;
}
