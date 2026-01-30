

#ifndef NET_EXT_SOURCE
#define NET_EXT_SOURCE

#include "./net4ext.h"

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


void net4ext_release(void *target) {
    
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
    if (!naettInitialized) naettInit(NULL);
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

static int __net4ext_server(sb_Event *event) {
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

void net4ext_serve(char *port, Net4Server_Callback* callback) {
    //
    sb_Options *opt = malloc(sizeof(sb_Options));
    opt->port = port;
    opt->handler = &__net4ext_server;
    opt->udata = &callback;
    //
    sb_Server *server = sb_new_server(opt);
    if (!server) {
        net4ext_error("failed to initialize server");
        exit(EXIT_FAILURE);
    }
    //
    char msg[256];
    sprintf(msg, "Server running at http://localhost:%s", port);
    net4ext_print(msg);
    for (;;) {
        sb_poll_server(server, 1000);
    }
}

////////////////////////////////////

void test_handler(Net4extReq *req, Net4extRsp *rsp) {
    rsp->headers[0] = (Net4StringPair){"Content-Type", "text/plain"};
    rsp->headers[1] = (Net4StringPair){"rspKey", "rspVal"};
    rsp->status = 202;
    // target->content = "hello...";
    // rsp->content = "{a:123}";
    rsp->content = req->url;
}

void net4ext_test() {
    //
    char getUrl[] = "https://httpbin.org/get";
    char postUrl[] = "https://httpbin.org/post";
    Net4extReq *req = net4ext_newGet(getUrl);
    // Net4extReq *req = net4ext_newPost(postUrl, "{a:123}");
    req->headers = &(Net4StringPairs){
        {"accept", "*/*"},
        {"SOMEKEY", "SOMEVAL"},
        {"ANOTHERRRR", "ANOTHERSTU"},
        {"My-test-key-header", "My-test-val-header"},
    };
    Net4extRsp *rsp = net4ext_request(req);
    printf("status %d length %d:\n", rsp->status, rsp->length);
    printf("content: [%s]\n", rsp->content);
    // // 
    // net4ext_serve("5678", &test_handler);
    // //
    // printf("test: %d [%s]\n", headCount, (*(&headers))[1][1]);
    net4ext_print("complete!!!");
    // net4ext_release(NULL);
}

#endif
