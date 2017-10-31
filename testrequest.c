#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <evhttp.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/bufferevent.h>

void http_request_done(struct evhttp_request *req, void *arg){
    char buf[2048];
    int s = evbuffer_remove(req->input_buffer, &buf, sizeof(buf) - 1);
    buf[s] = '\0';
    printf("con: %s", buf);
    
    // terminate event_base_dispatch()
    event_base_loopbreak((struct event_base *)arg);
}

int main(int argc, char **argv){
    struct event_base *base;
    struct evhttp_connection *conn;
    struct evhttp_request *req;

    base = event_base_new();
    conn = evhttp_connection_base_new(base, NULL, "103.1.236.175", 80);
    req = evhttp_request_new(http_request_done, base);

    evhttp_add_header(req->output_headers, "Host", "Eitguide.net");
    evhttp_add_header(req->output_headers, "Connection", "KeepAlive");
    //evhttp_add_header(req->output_headers, "Connection", "close");

    evhttp_make_request(conn, req, EVHTTP_REQ_GET, "GET / HTTP/1.1");
    evhttp_connection_set_timeout(req->evcon, 600);
    event_base_dispatch(base);

    return 0;
}