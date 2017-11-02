/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: yurnero
 *
 * Created on November 2, 2017, 10:25 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h> 
#include <iostream>
#include <fstream>
#include <string.h>
#include <cassert>
#include <cstdlib>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <unistd.h>
using namespace std;


void read_callback(struct bufferevent *bev, void *ctx);
void event_callback(struct bufferevent *bev, short events, void *ctx);


vector<string> host;
vector<string> domain;
#define CHUNK_SIZE 512

void adddata(){
	host.push_back("123.30.134.2");
	domain.push_back("nhaccuatui.com");

	host.push_back("123.30.215.16");
	domain.push_back("tuoitre.vn");

	host.push_back("212.193.33.272");
	domain.push_back("codeforces.com");

	host.push_back("222.255.27.168");
	domain.push_back("genk.vn");

	host.push_back("222.255.27.28");
	domain.push_back("dantri.com.vn");

	host.push_back("222.255.27.9");
	domain.push_back("dantri.com.vn");

	host.push_back("222.255.27.22");
	domain.push_back("dantri.com.vn");

	host.push_back("222.255.27.169");
	domain.push_back("dantri.com.vn");

	host.push_back("125.212.233.133");
	domain.push_back("24h.com.vn");

	host.push_back("125.212.247.3");
	domain.push_back("24h.com.vn");

	host.push_back("123.30.134.2");
	domain.push_back("nhaccuatui.com");

	host.push_back("123.30.215.16");
	domain.push_back("tuoitre.vn");

	host.push_back("212.193.33.27");
	domain.push_back("codeforces.com");

	host.push_back("222.255.27.168");
	domain.push_back("genk.vn");

	host.push_back("222.255.27.28");
	domain.push_back("dantri.com.vn");

	host.push_back("222.255.27.9");
	domain.push_back("dantri.com.vn");

	host.push_back("222.255.27.22");
	domain.push_back("dantri.com.vn");

	host.push_back("222.255.27.169");
	domain.push_back("dantri.com.vn");
}


/*
 * 
 */
int main(int argc, char** argv) {
    //freopen("out2.txt","w",stdout);
    clock_t t;
    int start_s = clock();



    struct sockaddr_in 	addr;
    struct timeval 	timeout;

    adddata();
    for(int ii = 0; ii < 10; ++ii)
    {
        int i = ii % (host.size());
        struct event_base *base;
	struct sockaddr_in sin;
	struct bufferevent *bev;
        base = event_base_new();
	if (!base) {
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}
        memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	inet_aton(host[i].c_str(), &sin.sin_addr);

	memset(sin.sin_zero, 0x00, 0);

	bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
        
	bufferevent_setcb(bev, read_callback, NULL, event_callback, NULL);
	if (bufferevent_socket_connect(bev, 
				(struct sockaddr *)&sin, sizeof(sin)) < 0) {
		fprintf(stderr, "Could not connect!\n");
		bufferevent_free(bev);	
		return -1;
	}
        evutil_socket_t fd = bufferevent_getfd(bev);
        printf("socket fd: %d\n",fd);
	       bufferevent_enable(bev, EV_READ|EV_WRITE);
        string sDomain = domain[i];
        string mess = "GET / HTTP/1.1\r\nHost: "+ sDomain +"\r\nConnection: keep-alive \r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6)\r\nAccept: */*\r\n\r\n";
        
        bufferevent_write(bev, mess.c_str(), mess.length());
        event_base_dispatch(base);
    }
    int stop_s=clock();
    cout << "**********************time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;
    
    return 0;
}

void read_callback(struct bufferevent *bev, void *ctx) {
    char buf[2048] = {0};
//    bufferevent_free(bev);
    // size_t n = bufferevent_read(bev, buf, 2048);
    // if(n > 0) {
    //     //sleep(5);
    //     puts(buf);
    //     bufferevent_free(bev);
    // }
    size_t n;
    int c = 0;
    while (1){
        n = bufferevent_read(bev, buf, 2048);
        if(n > 0) {
            //sleep(5);
            puts(buf);
           
        }
        cout <<"\n++++++" << c <<"+++++++\n";
        if (c == 5)
            {
                bufferevent_free(bev);
                break;
            }
        c++;
    }
    
}

void event_callback(struct bufferevent *bev, short events, void *ctx) {
	if (events & BEV_EVENT_CONNECTED) {
		printf("connected\n");
	} else if (events & BEV_EVENT_EOF) {
		printf("Connection closed.\n");
	} else if (events & BEV_EVENT_ERROR) {
		printf("Got an error on the connection: %s\n",
				strerror(errno));
	}
}