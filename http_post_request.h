#ifndef HTTP_POST_REQUEST_H
#define HTTP_POST_REQUEST_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

// POSTメソッドに対する処理
void handle_http_post_request(int sock_fd, const char* root_dir, const char* url, const char* request_header, int total_received_length, int request_header_length, char* status_line, char* response_header);

// アップロードされたファイルの保存
void save_uploaded_file(char* filename, char* file_data, int file_data_size);

#endif // HTTP_POST_REQUEST_H
