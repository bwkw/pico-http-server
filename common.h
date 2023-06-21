#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_HEADER_SIZE 8192

// HTTPリクエストヘッダーの読み取り
int read_http_request_header(int sock_fd, char* request_header, int* total_received_length);

// レスポンスの返却
void send_response(const int sock_fd, const char* status_line, char* response_header, const char* response_body, const int response_body_size);

// メソッドが有効化のチェック
int is_valid_request_method(char* method);

#endif // COMMON_H
