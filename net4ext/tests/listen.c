
#include "../net4ext.c"

#include "../dyad/dyad.c"

static void onData(dyad_Event *e) {
  printf("onData: %s\n", e->msg);
  dyad_write(e->stream, e->data, e->size);
}

static void onAccept(dyad_Event *e) {
  printf("onAccept: %s\n", e->msg);
  dyad_addListener(e->remote, DYAD_EVENT_DATA, onData, NULL);
  dyad_writef(e->remote, "message from server...");
}

int main(void) {
  dyad_init();
  dyad_Stream *s = dyad_newStream();
  dyad_addListener(s, DYAD_EVENT_ACCEPT, onAccept, NULL);
  dyad_listen(s, 5555);
  while (dyad_getStreamCount() > 0) {
    dyad_update();
  }
  return 0;
}
