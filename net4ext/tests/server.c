
// #include "../net4ext.c"

#include "../sandbird/sandbird.c"

static int sandbird_handler(sb_Event *e) {
    if (e->type == SB_EV_REQUEST) {
        printf("request:addr:[%s],method:[%s],path:[%s]\n", e->address, e->method, e->path);
        sb_send_status(e->stream, 200, "OK");
        sb_send_header(e->stream, "Content-Type", "text/plain");
        sb_writef(e->stream, "Hello world!");
    }
    return SB_RES_OK;
}
void run_sandbird() {
    printf("\nsandbird.start:\n");
    sb_Options opt;
    memset(&opt, 0, sizeof(opt));
    opt.port = "8000";
    opt.handler = sandbird_handler;
    sb_Server *server = sb_new_server(&opt);
    printf("sandbird.addr: http://localhost:%s\n", opt.port);
    while (1) sb_poll_server(server, 1000);
    sb_close_server(server);
    printf("sandbird.end!\n");
}

int main(int argc, char* argv[]) {
    run_sandbird();
    return 0;
}
