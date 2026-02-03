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

bool net4RequestInitialized = false;
bool net4ServerInitialized = false;
bool net4socketInitialized = false;

#define Net4extType_REQ 1
#define Net4extType_RSP 2
#define Net4extType_Socket 3

#define Net4extFlag_REQUEST 1
#define Net4extFlag_SERVE 2
#define Net4extFlag_SOCKET 3

typedef struct Net4extReq Net4extReq;
typedef struct Net4extRsp Net4extRsp;
typedef int (*Net4Server_Callback)(Net4extReq*, Net4extRsp*);

typedef struct Net4extSocket Net4extSocket;
typedef void (*Net4Socket_Accepter)(Net4extSocket *);
typedef void (*Net4Socket_Handler)(Net4extSocket *, void *);


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

struct Net4extSocket {
    struct __Net4extObj;
    int status;
    char url[1024];
    int port;
    dyad_Stream *stream;
    Net4Socket_Handler onConnect;
    Net4Socket_Handler onData;
    Net4Socket_Handler onClose;
    Net4Socket_Handler onError;
    Net4Socket_Handler onTimeout;
};

#define Net4Socket_STATUS_NONE 1
#define Net4Socket_STATUS_CONNECTING 2
#define Net4Socket_STATUS_CONNECTED 3
#define Net4Socket_STATUS_CLOSING 4
#define Net4Socket_STATUS_CLOSED 5

Net4extReq __net4serverReq;
Net4extRsp __net4serverRsp;

Net4extSocket* __net4ext_newSocket(dyad_Stream *);

#endif
