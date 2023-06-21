#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <arpa/inet.h>

#define MAX_HEADER_SIZE 8192

// HTTPサーバー起動
int run_http_server(const char *ip_address, int port, const char *root_dir);

// HTTPリクエストに対する処理
void handle_http_request(int sock_fd, const char* root_dir);

#endif // HTTP_SERVER_H
