// net ext

#ifndef NET_EXT_HEADER
#define NET_EXT_HEADER

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define __USE_W32_SOCKETS
#include "./naett/naett.c"
#include "./sandbird/sandbird.c"
#include "./dyad/dyad.c"

#define Net4extType_REQ 1
#define Net4extType_RSP 2

#define Net4extFlag_REQUEST 1
#define Net4extFlag_SERVE 2
#define Net4extFlag_LISTEN 3
#define Net4extFlag_CONNECT 4

typedef struct Net4extReq Net4extReq;
typedef struct Net4extRsp Net4extRsp;
typedef int (*Net4Server_Callback)(Net4extReq*, Net4extRsp*);


typedef struct __Net4extObj {
    int type;
} _Net4extObj;

typedef struct {
    char *key;
    char *val;
} Net4StringPair;

#define NET4EXT_PAIRS_COUNT 8
typedef Net4StringPair Net4StringPairs[NET4EXT_PAIRS_COUNT];

struct Net4extReq {
    struct __Net4extObj;
    Net4StringPairs *headers;
    char url[1024];
    bool post;
    int timeout;
    char body[1024*5];
};

struct Net4extRsp {
    struct __Net4extObj;
    Net4StringPairs headers;
    int status;
    int length;
    char *content;
    int flag;
    void *extra;
};

Net4extReq __net4serverReq;
Net4extRsp __net4serverRsp;

#endif
