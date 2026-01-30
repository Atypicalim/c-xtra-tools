

#ifndef NET4EXT_COMMON
#define NET4EXT_COMMON

#ifdef _WIN32
    #define close(a) closesocket(a)
    #define getsockopt(a,b,c,d,e) getsockopt((a),(b),(c),(char*)(d),(e))
    #define setsockopt(a,b,c,d,e) setsockopt((a),(b),(c),(char*)(d),(e))
    #define select(a,b,c,d,e) select((int)(a),(b),(c),(d),(e))
    #define bind(a,b,c) bind((a),(b),(int)(c))
    #define connect(a,b,c) connect((a),(b),(int)(c))

    #define netGetError WSAGetLastError()

    #undef  EWOULDBLOCK
    #define EWOULDBLOCK WSAEWOULDBLOCK

    const char *inet_ntop(int af, const void *src, char *dst, size_t size) { // socklen_t
        union { struct sockaddr sa; struct sockaddr_in sai;
                struct sockaddr_in6 sai6; } addr;
        int res;
        memset(&addr, 0, sizeof(addr));
        addr.sa.sa_family = af;
        if (af == AF_INET6) {
            memcpy(&addr.sai6.sin6_addr, src, sizeof(addr.sai6.sin6_addr));
        } else {
            memcpy(&addr.sai.sin_addr, src, sizeof(addr.sai.sin_addr));
        }
        res = WSAAddressToStringA(&addr.sa, sizeof(addr), 0, dst, (LPDWORD) &size);
        if (res != 0) return NULL;
        return dst;
    }
#endif

#endif
