#include "http_post_request.h"
#include "http_status.h"
#include "common.h"

// POSTメソッドに対する処理
void handle_http_post_request(int sock_fd, const char* root_dir, const char* url, const char* request_header, int total_received_length, int request_header_length, char* status_line, char* response_header) {
    // content_lengthの取得と、request_bodyの動的確保
    char* content_length_marker = strstr(request_header, "Content-Length: ");
    if (!content_length_marker) {
        perror("Content-Length not found in request_header.");
        return;
    }
    int content_length = atoi(content_length_marker + 16);  // "Content-Length: "分スキップ
    char* request_body = malloc(content_length + 1);
    if (!request_body) {
        perror("Failed to allocate memory for request body.");
        return;
    }

    // request_bodyに詰める
    int received_body_length = total_received_length - request_header_length;
    memcpy(request_body, request_header + request_header_length, received_body_length);
    while (received_body_length < content_length) {
        int received = read(sock_fd, request_body + received_body_length, content_length - received_body_length);
        if (received <= 0) {
            perror("Failed to receive bytes from client.");
            return;
        }
        received_body_length += received;
    }
    request_body[received_body_length] = '\0';

    // ファイルのアップロード
    char file_path[2048];
    sprintf(file_path, "%s%s", root_dir, url);
    save_uploaded_file(file_path, request_body, received_body_length);

    // レスポンス
    HttpStatus status = get_status_by_code(201);
    sprintf(status_line + strlen(status_line), "%d %s\r\n", status.code, status.description);
    char response_body[128];
    sprintf(response_body, "<!DOCTYPE html><html><body><h1>%d %s</h1></body></html>", status.code, status.description);
    send_response(sock_fd, status_line, response_header, response_body, strlen(response_body));

    free(request_body);
}

// アップロードされたファイルの保存
void save_uploaded_file(char* filename, char* file_data, int file_data_size) {
    int file_fd = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (file_fd < 0) {
        perror("Could not open file to save uploaded data");
        return;
    }
    if (write(file_fd, file_data, file_data_size) < 0) {
        perror("Could not write uploaded data to file");
    }
    close(file_fd);
}
