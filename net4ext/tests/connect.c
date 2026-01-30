
#include "../net4ext.c"

#include <stdio.h>
#include <unistd.h>
#include "../dyad/dyad.h"
#include "../dyad/dyad.c"

static void onData(dyad_Event *e) {
  printf("onData:%s\n", e->data);
  sleep(1);
  dyad_writef(e->stream, "message from client...");
}

static void onConnect(dyad_Event *e) {
  printf("onConnect:%s\n", e->msg);
}

int main(void) {
  dyad_init();
  dyad_Stream *s = dyad_newStream();
  dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
  dyad_addListener(s, DYAD_EVENT_DATA, onData, NULL);
  dyad_connect(s, "localhost", 5555);
  while (dyad_getStreamCount() > 0) {
    dyad_update();
  }
  dyad_shutdown();
  return 0;
}
