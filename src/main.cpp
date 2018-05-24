#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Logger.h"

#define EPOLLEVENTS 100
#define MAXSIZE     1024
#define FDSIZE      1024
#define LISTENQ     5

int socket_bind(const char* ip, uint16_t port) {
    int listenfd;
    sockaddr_in server_addr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket error:");
        exit(1);
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_addr.sin_addr);
    server_addr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind error: ");
        exit(1);
    }
    return listenfd;
}

void add_event(int epollfd, int fd, int state) {
    epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void delete_event(int epollfd, int fd, int state) {
    epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void modify_event(int epollfd, int fd, int state) {
    epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void handle_accept(int epollfd, int listenfd) {
    sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    int clifd = accept(listenfd, (sockaddr*)&cliaddr, &cliaddrlen);
    if (clifd == -1) {
        perror("accpet error:");
    } else {
        Logger::info("accept a new client") << inet_ntoa(cliaddr.sin_addr) << cliaddr.sin_port;
        add_event(epollfd, clifd, EPOLLIN);
    }
}

void do_read(int epollfd, int fd, char* buff) {
    int nread;
    nread = read(fd, buff, MAXSIZE);
    if (nread == -1) {
        Logger::error("read error:");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    } else {
        Logger::info("read message is :") << buff;
        modify_event(epollfd, fd, EPOLLOUT);
    }
}

void do_write(int epollfd, int fd, char* buff) {
    int nwrite = write(fd, buff, strlen(buff));
    if (nwrite == -1) {
        Logger::error("write error:");
        close(fd);
        delete_event(epollfd, fd, EPOLLOUT);
    } else {
        modify_event(epollfd, fd, EPOLLIN);
    }
    memset(buff, 0, MAXSIZE);
}

void handle_events(int epollfd, epoll_event* events, int num, int listenfd, char* buff) {
    int fd;
    for (int i = 0; i < num; i++) {
        fd = events[i].data.fd;

        if((fd == listenfd) && (events[i].events & EPOLLIN)) {
            handle_accept(epollfd, listenfd);
        } else if (events[i].events & EPOLLIN) {
            do_read(epollfd, fd, buff);
        } else if(events[i].events & EPOLLOUT) {
            do_write(epollfd, fd, buff);
        }
    }
}

void do_epoll (int listenfd) {
    int epollfd;
    epoll_event events[EPOLLEVENTS];
    int ret;
    char buff[MAXSIZE];
    memset(buff, 0, MAXSIZE);
    //create a fd
    epollfd = epoll_create(FDSIZE);
    //add listen fd event
    add_event(epollfd, listenfd, EPOLLIN);

    for (;;) {
        ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
        handle_events(epollfd, events, ret, listenfd, buff);
    }
    close(epollfd);
}

int main() {
    int listenfd;
    Logger::init();
    Logger::info("start service") << "127.0.0.1:" << 8088;
    listenfd = socket_bind("127.0.0.1", 8888);
    listen(listenfd, LISTENQ);
    do_epoll(listenfd);
    return 0;
}