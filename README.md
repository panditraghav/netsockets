# `getaddrinfo()`
Given node and service, which identify an  Internet  host and a service, getaddrinfo() returns one or more addrinfo
structures,  each  of  which contains an Internet address that can be specified in a call to bind(2) or connect(2).
The getaddrinfo()  function  combines  the  functionality provided  by  the  gethostbyname(3)  and getservbyname(3)
functions into a single interface, but unlike the  latter functions, getaddrinfo() is reentrant and allows programs
to eliminate IPv4-versus-IPv6 dependencies.

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
int getaddrinfo(const char *restrict node,
                const char *restrict service,
                const struct addrinfo *restrict hints,
                struct addrinfo **restrict res);
void freeaddrinfo(struct addrinfo *res);
const char *gai_strerror(int errcode);

struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};
```
