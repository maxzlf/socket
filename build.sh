
gcc -g          \
    utils/*.c   \
    tcp/util.c  \
    tcp/client.c\
    -o tcp_client -Wall

gcc -g          \
    utils/*.c   \
    tcp/util.c  \
    tcp/server.c\
    -o tcp_server -Wall

gcc -g          \
    utils/*.c   \
    udp/client.c\
    -o udp_client -Wall

gcc -g          \
    utils/*.c   \
    udp/server.c\
    -o udp_server -Wall

gcc -g          \
    utils/*.c   \
    ipc/client.c\
    -o ipc_client -Wall

gcc -g          \
    utils/*.c   \
    ipc/server.c\
    -o ipc_server -Wall

sudo rm -r *dSYM
