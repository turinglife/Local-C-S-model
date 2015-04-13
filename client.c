
#include "client.h"

// char *socket_path = "./socket";

/* For communication with itf daemon, Unix-domain(Local) socket will be employed to take on the
 * responsibility of Inter-processes communication on the same machine.
 */

/* Linux has a special feature: if the pathname for a UNIX domain socket begins with a null byte \0, its name is not mapped into the filesystem. Thus it won’t collide with other names in the filesystem. Also, when a server closes its UNIX domain listening socket in the abstract namespace, its file is deleted; with regular UNIX domain sockets, the file persists after the server closes it. */
char *socket_path = "\0hidden";

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    char buf[100];
    int fd, rc, cl, recvbytes;

    // If do not specify socket_path, default socket_path will be employed.
    if (argc > 1) {
        socket_path=argv[1];
    }

    /*
    AF_UNIX: The AF_UNIX (also known as AF_LOCAL) socket family is used to communicate between processes
             on the same machine efficiently.

    SOCK_STREAM: Provides sequenced, reliable, two-way, connection-based byte streams.
                 An out-of-band data transmission mechanism may be sup-ported.
    */
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    // fill out socket address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

    // connect to server(in our example, server is another process running in the same machine.)
    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        exit(-1);
    }

    printf("Connected.\n");

    /*
    NOTE: read() doesn't add '\0' to terminate to make it string (just gives raw buffer).

    */

    while(printf("\nclient>"), fgets(buf, sizeof(buf), stdin), !feof(stdin)) {

        // Get message from console, send it to server.
        if(send(fd, buf, strlen(buf), 0) == -1) {
            perror("send");
            exit(1);
        }

        // After sending message to server, get feedback from server.
        if((recvbytes = recv(fd, buf, sizeof(buf), 0)) > 0) {
            buf[recvbytes] = '\0';
            printf("server> %s", buf);
        }else {
            if(recvbytes < 0)
                perror("recv");
            else
                printf("Server closed connection\n");

            exit(1);
        }
    }


    close(fd);


    return 0;
}
