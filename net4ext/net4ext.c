

#ifndef NET_EXT_SOURCE
#define NET_EXT_SOURCE

#include "./net4ext.h"

////////////////////////////////////

void net4ext_init() {
    if(!net4RequestInitialized) {
        net4RequestInitialized = true;
        naettInit(NULL);
    }
    if(!net4socketInitialized) {
        net4socketInitialized = true;
        dyad_init();
    }
}

void net4ext_update() {
    if(net4socketInitialized) {
        if (dyad_getStreamCount() > 0) {
            dyad_update();
        }
    }
}

void net4ext_release() {
    if(net4socketInitialized) dyad_shutdown();
}

////////////////////////////////////

void __net4ext_initObj(void *_this, int type)
{
    _Net4extObj *this = _this;
    this->type = type;
}

bool __net4ext_validPair(Net4StringPair *from) {
    if (from == NULL) return false;
    bool a = from->key != NULL && from->key[0] != '\0';
    bool b = from->val != NULL && from->val[0] != '\0';
    return a && b;
}

Net4extReq* __net4ext_newReq(Net4extReq *target) {
    if (target == NULL) {
        target = malloc(sizeof(Net4extReq));
    }
    __net4ext_initObj(target, Net4extType_REQ);
    target->headers = NULL;
    strcpy(target->url, "");
    target->post = false;
    target->timeout = 30;
    strcpy(target->body, "");
    return target;
}

Net4extReq* net4ext_newGet(char *url) {
    Net4extReq* req = __net4ext_newReq(NULL);
    strcpy(req->url, url);
    req->post = false;
    strcpy(req->body, "");
    return req;
}

Net4extReq* net4ext_newPost(char *url, char *body) {
    Net4extReq* req = __net4ext_newReq(NULL);
    strcpy(req->url, url);
    req->post = true;
    strcpy(req->body, body);
    return req;
}

Net4extRsp* __net4ext_newRsp(Net4extRsp *target) {
    if (target == NULL) {
        target = malloc(sizeof(Net4extRsp));
    }
    __net4ext_initObj(target, Net4extType_RSP);
    memset(target->headers, 0, sizeof(target->headers));
    target->status = -1;
    target->content = "";
    target->length = 0;
    target->flag = -1;
    target->extra = NULL;
    return target;
}

void net4ext_print(char *msg)
{
    printf("net4ext print: %s\n", msg);
}

void net4ext_error(char *msg)
{
    fprintf(stderr, "net4ext error: %s\n", msg);
}

void net4ext_assert(bool val, char *msg)
{
    if (val) read;
    fprintf(stderr, "net4ext assert: %s\n", msg);
}


void net4ext_freeObject(void *target) {
    
    // if (target->__type == Net4extFlag_REQUEST)
    // {
    //     naettClose(target->extra);
    //     naettFree(target->__target);
    // }
    // else if (target->__type == Net4extFlag_SERVE)
    // {
    //     sb_close_server(target->__target);
    // }
    free(target);
}

char* net4ext_getHeader(void *any, char *key)
{
    // if (target->__type == Net4extFlag_REQUEST)
    // {
    //     naettReq* res = target->extra;
    //     net4ext_assert(res != NULL, "invalid request res");
    //     return naettGetHeader(res, key);
    // }
    // else if (target->__type == Net4extFlag_SERVE)
    // {
    //     sb_Event *event = target->extra;
    //     net4ext_assert(event != NULL, "invalid serve event");
    //     char val[128];
    //     sb_get_header(event->stream, key, val, 128);
    //     return val;
    // }
}

Net4extRsp* net4ext_request(Net4extReq *req)
{
    // 
    int optionCount = 0;
    naettOption *optionList[NET4EXT_PAIRS_COUNT];
    optionList[optionCount++] = naettMethod(req->post ? "POST" : "GET");
    optionList[optionCount++] = naettTimeout(req->timeout);
    //
    if (req->post) {
        optionList[optionCount++] = naettBody(req->body, strlen(req->body));
    }
    //
    for (size_t i = 0; i < NET4EXT_PAIRS_COUNT; i++) {
        if (req->headers == NULL) break;
        Net4StringPair header = (*req->headers)[i];
        if (!__net4ext_validPair(&header)) continue;
        char *_key = header.key;
        char *_val = header.val;
        optionList[optionCount++] = naettHeader(_key, _val);
    }
    //
    net4ext_init();
    naettReq* _req = naettRequestWithOptions(req->url, optionCount, optionList);
    naettRes* _res = naettMake(_req);
    while (!naettComplete(_res)) {
        usleep(100 * 1000);
    }
    //
    Net4extRsp *rsp = __net4ext_newRsp(NULL);
    rsp->status = naettGetStatus(_res);
    rsp->content = naettGetBody(_res, &rsp->length);
    return rsp;
}

static int __net4server_onRequest(sb_Event *event) {
    if (event->type == SB_EV_REQUEST) {
        //
        Net4extReq *req = __net4ext_newReq(&__net4serverReq);
        Net4extRsp *rsp = __net4ext_newRsp(&__net4serverRsp);
        //
        strcpy(req->url, event->address);
        strcat(req->url, event->path);
        req->post = strcmp(event->method, "GET") != 0;
        rsp->flag = Net4extFlag_SERVE;
        rsp->extra = event->stream;
        //
        Net4Server_Callback *callback = event->udata;
        if (callback != NULL) {
            (*callback)(req, rsp);
        }
        sb_send_status(event->stream, rsp->status, "No Content");
        for (size_t i = 0; i < NET4EXT_PAIRS_COUNT; i++) {
            Net4StringPair header = rsp->headers[i];
            if (header.key == NULL || header.val == NULL) continue;
            sb_send_header(event->stream, header.key, header.val);
        }
        sb_writef(event->stream, rsp->content);
        // 
    }
    return SB_RES_OK;
}

void net4ext_server(char *port, Net4Server_Callback* callback) {
    //
    sb_Options *opt = malloc(sizeof(sb_Options));
    opt->port = port;
    opt->handler = &__net4server_onRequest;
    opt->udata = &callback;
    //
    sb_Server *server = sb_new_server(opt);
    if (!server) {
        net4ext_error("failed to initialize server");
        exit(EXIT_FAILURE);
    }
    //
    char msg[256];
    sprintf(msg, "Server running at http://localhost:%s\n", port);
    net4ext_print(msg);
    while (true) {
        sb_poll_server(server, 1000);
    }
}

void __net4server_onSocket(dyad_Event *e)
{
    Net4Socket_Accepter func = e->udata;
    dyad_Stream *stream = e->remote;
    Net4extSocket *sock = __net4ext_newSocket(stream);
    strcpy(sock->url, dyad_getAddress(stream));
    sock->port = dyad_getPort(stream);
    if (func != NULL) {
        (func)(sock);
    }
}

void net4ext_listen(int port, Net4Socket_Accepter onClient) {
    net4ext_init();
    dyad_Stream *stream = dyad_newStream();
    dyad_addListener(stream, DYAD_EVENT_ACCEPT, __net4server_onSocket, onClient);
    dyad_listen(stream, port);
    char msg[256];
    sprintf(msg, "socket listening at http://localhost:%i\n", port);
    net4ext_print(msg);
    while (dyad_getStreamCount() > 0) {
        dyad_update();
    }
}

static void __net4socket_onConnect(dyad_Event *e)
{
    Net4extSocket *sock = e->udata;
    sock->status = Net4Socket_STATUS_CONNECTED;
    Net4Socket_Handler callback = sock->onConnect;
    if (callback != NULL) {
        callback(sock, e->msg);
    }
}

static void __net4socket_onData(dyad_Event *e) {
    Net4extSocket *sock = e->udata;
    Net4Socket_Handler callback = sock->onData;
    if (callback != NULL) {
        callback(sock, e->data);
    }
}

static void __net4socket_onClose(dyad_Event *e) {
    Net4extSocket *sock = e->udata;
    sock->status = Net4Socket_STATUS_CLOSED;
    Net4Socket_Handler callback = sock->onClose;
    if (callback != NULL) {
        callback(sock, e->msg);
    }
}

static void __net4socket_onError(dyad_Event *e) {
    Net4extSocket *sock = e->udata;
    Net4Socket_Handler callback = sock->onError;
    if (callback != NULL) {
        callback(sock, e->msg);
    }
}

static void __net4socket_onTimeout(dyad_Event *e) {
    Net4extSocket *sock = e->udata;
    Net4Socket_Handler callback = sock->onTimeout;
    if (callback != NULL) {
        callback(sock, e->msg);
    }
}

Net4extSocket* __net4ext_newSocket(dyad_Stream *stream) {
    Net4extSocket *sock = malloc(sizeof(Net4extSocket));
    __net4ext_initObj(sock, Net4extType_Socket);
    strcpy(sock->url, "");
    sock->port = 0;
    sock->status = Net4Socket_STATUS_NONE;
    sock->onData = NULL;
    sock->onClose = NULL;
    sock->onError = NULL;
    sock->onTimeout = NULL;
    dyad_addListener(stream, DYAD_EVENT_CONNECT, __net4socket_onConnect, sock);
    dyad_addListener(stream, DYAD_EVENT_DATA, __net4socket_onData, sock);
    dyad_addListener(stream, DYAD_EVENT_CLOSE, __net4socket_onClose, sock);
    dyad_addListener(stream, DYAD_EVENT_ERROR, __net4socket_onError, sock);
    dyad_addListener(stream, DYAD_EVENT_TIMEOUT, __net4socket_onTimeout, sock);
    sock->stream = stream;
    return sock;
}

Net4extSocket* net4ext_newSocket(char *url, int port) {
    net4ext_init();
    dyad_Stream *stream = dyad_newStream();
    Net4extSocket *sock = __net4ext_newSocket(stream);
    strcpy(sock->url, url);
    sock->port = port;
    return sock;
}

void net4socket_connect(Net4extSocket *sock) {
    if (sock->status == Net4Socket_STATUS_CONNECTED) return;
    sock->status = Net4Socket_STATUS_CONNECTING;
    dyad_connect(sock->stream, sock->url, sock->port);

}

void net4socket_close(Net4extSocket* sock) {
    if (sock->status == Net4Socket_STATUS_CLOSED) return;
    sock->status = Net4Socket_STATUS_CLOSING;
    dyad_close(sock->stream);
}

bool net4socket_send(Net4extSocket* sock, char *data) {
    if (sock->status == Net4Socket_STATUS_CONNECTED) {
        dyad_writef(sock->stream, data);
        return true;
    } else {
        return false;
    }
}


////////////////////////////////////

void test_onRequest(Net4extReq *req, Net4extRsp *rsp) {
    rsp->headers[0] = (Net4StringPair){"Content-Type", "text/plain"};
    rsp->headers[1] = (Net4StringPair){"rspKey", "rspVal"};
    rsp->status = 202;
    // target->content = "hello...";
    // rsp->content = "{a:123}";
    rsp->content = req->url;
}

void test_onConnect(Net4extSocket *sock, char *msg) {
    printf("connected!!!! %s \n", msg);
}

void test_onData(Net4extSocket *sock, char *data)
{
    printf("onoData %s \n", data);
}

void test_onClient(Net4extSocket *client)
{
    printf("onClient\n");
    printf("sock %p \n", client);
    client->onData = &test_onData;
    printf("onClient:%p\n", client->stream);
}

void net4ext_test() {
    net4ext_print("test:");
    //
    char getUrl[] = "https://httpbin.org/get";
    char postUrl[] = "https://httpbin.org/post";
    // Net4extReq *req = net4ext_newGet(getUrl);
    // Net4extReq *req = net4ext_newPost(postUrl, "{a:123}");
    // req->headers = &(Net4StringPairs){
    //     {"accept", "*/*"},
    //     {"SOMEKEY", "SOMEVAL"},
    //     {"ANOTHERRRR", "ANOTHERSTU"},
    //     {"My-test-key-header", "My-test-val-header"},
    // };
    // Net4extRsp *rsp = net4ext_request(req);
    // printf("status %d length %d:\n", rsp->status, rsp->length);
    // printf("content: [%s]\n", rsp->content);
    // // 
    // net4ext_server("5678", &test_handler);
    // 
    // net4ext_listen(5555, &test_onClient);
    // 
    // Net4extSocket *sock = net4ext_newSocket("localhost", 5555);
    // sock->onConnect = &test_onConnect;
    // sock->onData = &test_onData;
    // net4socket_connect(sock);
    // while (true)
    // {
    //     net4ext_update();
    // }
    
    //
    // printf("test: %d [%s]\n", headCount, (*(&headers))[1][1]);
    net4ext_print("complete!!!");
    // net4ext_release(NULL);
}

#endif
