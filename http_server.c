#include "http_server.h"
#include "http_status.h"
#include "http_post_request.h"
#include "http_head_get_request.h"
#include "common.h"

// プロトタイプ
void handle_http_request(int sock_fd, const char* root_dir);

// HTTPサーバー起動
int run_http_server(const char* ip_address, const int port, const char* root_dir) {
    int sock_fd;
    struct sockaddr_in server;
    
    if ((sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("Failed to create socket");
        return 1;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    int inet_result = inet_pton(AF_INET, ip_address, &(server.sin_addr));
    if (inet_result == 0) {
        fprintf(stderr, "Invalid IP address: %s\n", ip_address);
        exit(EXIT_FAILURE);
    } else if (inet_result == -1) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    server.sin_port = htons(port);
    int optval = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Failed to bind to the server address");
        return 1;
    }
    listen(sock_fd, 10);

    struct sockaddr_in client;
    int len = sizeof(struct sockaddr_in);
    int connection;

    while ((connection = accept(sock_fd, (struct sockaddr *)&client, (socklen_t*)&len))) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Failed to fork");
            return 1;
        }
        if (pid == 0) {  // 子プロセス
            close(sock_fd);
            handle_http_request(connection, root_dir);
            close(connection);
            exit(EXIT_SUCCESS);
        } else {  // 親プロセス
            close(connection);
        }
    }

    return 0;
}

// HTTPリクエストに対する処理
void handle_http_request(int sock_fd, const char* root_dir) {
    char request_header[MAX_HEADER_SIZE];
    char status_line[128];
    char response_header[MAX_HEADER_SIZE];
    int total_received_length = 0;
    int request_header_length = 0;

    // HTTPリクエストヘッダーの読み取り
    request_header_length = read_http_request_header(sock_fd, request_header, &total_received_length);
    if (request_header_length < 0) {
        return;
    }

    // リクエスト情報のパース -> method, url(request target), http version
    char method[10], url[2048], http_version[10];
    sscanf(request_header, "%s %s %s", method, url, http_version);
    sprintf(status_line, "%s ", http_version);

    // url(request target)を基に、content_type取得
    char* content_type = NULL;
    if (strstr(url, ".html")) {
        content_type = "text/html; charset=UTF-8";
    } else if (strstr(url, ".css")) {
        content_type = "text/css; charset=UTF-8";
    } else if (strstr(url, ".js")) {
        content_type = "text/javascript; charset=UTF-8";
    } else if (strstr(url, ".png")) {
        content_type = "image/png;";
    } else if (strstr(url, ".jpeg") || strstr(url, ".jpg")) {
        content_type = "image/jpeg;";
    }
    sprintf(response_header, "Content-Type: %s\r\n", content_type);

    // リクエストメソッドが予期しているもの以外は弾く
    if (!is_valid_request_method(method)) {
        HttpStatus status = get_status_by_code(405);
        sprintf(status_line + strlen(status_line), "%d %s\r\n", status.code, status.description);
        char response_body[128];
        sprintf(response_body, "<!DOCTYPE html><html><body><h1>%d %s</h1></body></html>", status.code, status.description);
        send_response(sock_fd, status_line, response_header, response_body, strlen(response_body));
        return;
    }

    // HEAD, GETメソッドに対する処理
    if (strcmp(method, "HEAD") == 0 || strcmp(method, "GET") == 0) {
        handle_http_head_get_request(sock_fd, root_dir, url, method, status_line, response_header);
        return;
    }

    // POSTメソッドに対する処理
    if (strcmp(method, "POST") == 0) {
       handle_http_post_request(sock_fd, root_dir, url, request_header, total_received_length, request_header_length, status_line, response_header);
       return;
    }
}
