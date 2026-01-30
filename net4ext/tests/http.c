
#include "../net4ext.c"

#include "../naett/naett.c"

void run_naett() {
    printf("\naett.start:\n");
    //
    naettInit(NULL);
    naettReq* req = naettRequest("http://ip.jsontest.com/", naettMethod("GET"), naettHeader("accept", "*/*"));
    naettRes* res = naettMake(req);
    //
    while (!naettComplete(res)) {
        usleep(100 * 1000);
    }
    if (naettGetStatus(res) < 0) {
        printf("naett.failed!\n");
        return;
    }
    int bodyLength = 0;
    const char* body = naettGetBody(res, &bodyLength);
    //
    printf("naett.Header-> '%s'\n", naettGetHeader(res, "Content-Type"));
    printf("naett.Length-> %d bytes\n", bodyLength);
    printf("naett.body:%s\n", body);
    printf("naett.end!\n");
}


int main(int argc, char* argv[]) {
    run_naett();
    return 0;
}
