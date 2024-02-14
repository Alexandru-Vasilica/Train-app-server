#include <iostream>
#include "pugixml.hpp"
#include <cstdlib>
#include <cerrno>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include "requests.h"

#include "TrainData.h"
#include "CommandQueue.h"
#include "ThreadPool.h"

using namespace std;

#define PORT 3000
#define RESPONSE_LINE_BUFFER_SIZE 150


int main() {

    CommandQueue commandQueue;
//    data.get_routes("Micula Hm.");
//    commandQueue.add_command(new RoutesCommand(1,data,"Micula Hm."));

    pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
    ThreadPool::mlock=&mlock;
    ThreadPool::condition=&condition;
    ThreadPool::commands=&commandQueue;

    ThreadPool pool(20);

    struct sockaddr_in server;
    struct sockaddr_in from;
    fd_set readfds,actfds;

    int sd, client;
    int optval = 1;
    int fd;
    int nfds;
    socklen_t len;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server] Eroare la socket().\n");
        return errno;
    }

    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server] Eroare la bind().\n");
        return errno;
    }
    pool.start();
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(ROUTE_PATH);
    if (!result){
        perror("Eroare deschidere fisier xml\n");
        exit(1);
    }
    auto data=new TrainData(doc);
    // data->print();

    if (listen(sd, 10) == -1)
    {
        perror("[server] Eroare la listen().\n");
        return errno;
    }

    FD_ZERO(&actfds);
    FD_SET(sd, &actfds);

    nfds = sd;

    printf("[server] Asteptam la portul %d...\n", PORT);
    fflush(stdout);

    while (true)
    {

        bcopy((char *)&actfds, (char *)&readfds, sizeof(readfds));

        if (select(nfds + 1, &readfds, NULL, NULL, NULL) < 0)
        {
            perror("[server] Eroare la select().\n");
            return errno;
        }

        if (FD_ISSET(sd, &readfds))
        {

            len = sizeof(from);
            bzero(&from, sizeof(from));

            client = accept(sd, (struct sockaddr *)&from, &len);

            if (client < 0)
            {
                perror("[server] Eroare la accept().\n");
                continue;
            }

            if (nfds < client)
                nfds = client;

            FD_SET(client, &actfds);

            printf("[server] S-a conectat clientul cu descriptorul %d, de la adresa.\n", client);
            fflush(stdout);
        }

        for (fd = 0; fd <= nfds; fd++)
        {

            if (fd != sd && FD_ISSET(fd, &readfds))
            {
                struct request req;
                int bytes=read(fd, &req, sizeof(struct request));
                if(bytes) {
                    req.fd=fd;
                    commandQueue.add_command(get_request(req,*data,actfds));
//                    commandQueue.add_command(new RoutesCommand(fd,data,req.location));
                    printf("Added request from descriptor %d to queue\n", fd);
                    pthread_cond_signal(&condition);
                } else{
                    FD_CLR(fd,&actfds);
                }
            }
        }
    }


}
