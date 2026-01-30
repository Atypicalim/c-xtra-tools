
gcc http.c -o http.exe -lwinhttp
gcc server.c -o server.exe -lws2_32
gcc listen.c -o listen.exe -lws2_32
gcc connect.c -o connect.exe -lws2_32
