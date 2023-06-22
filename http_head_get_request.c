#include "http_head_get_request.h"
#include "http_status.h"
#include "common.h"

// HEAD, GETメソッドに対する処理
void handle_http_head_get_request(int sock_fd, const char* root_dir, const char* url, const char* method, char* status_line, char* response_header) {
    HttpStatus status;
    char file_path[2048];
    sprintf(file_path, "%s%s", root_dir, url);

    // リクエストファイルを開く
    int file_fd = open(file_path, O_RDONLY);  // 読み取り専用
    if (file_fd < 0) {
        if (errno == EACCES) {
            status = get_status_by_code(403);
        } else if (errno == ENOENT) {
            status = get_status_by_code(404);
        }
        sprintf(status_line + strlen(status_line), "%d %s\r\n", status.code, status.description);
        char response_body[128];
        sprintf(response_body, "<!DOCTYPE html><html><body><h1>%d %s</h1></body></html>", status.code, status.description);
        send_response(sock_fd, status_line, response_header, response_body, strlen(response_body));

        return;
    }

    // リダイレクト処理
    char* requested_file = strrchr(file_path, '/');
    if (requested_file == NULL) {
        printf("No slash ('/') found in the path.\n");
        requested_file = file_path;
    } else {
        requested_file++;
    }
    FILE *file = fopen("./redirect.cnf", "r");
    if (file == NULL) {
        printf("Cannot open the redirect.cnf \n");
    } else {
        RedirectEntry* entries = malloc(sizeof(RedirectEntry) * MAX_ENTRIES);
        int i = 0;
        while (fscanf(file, "%s %s %s", entries[i].filename, entries[i].redirect_url, entries[i].type) != EOF) {
            i++;
        }
        fclose(file);
        
        for (int j = 0; j < i; j++) {
            if (strcmp(entries[j].filename, requested_file) == 0) {
                if (strcmp(entries[j].type, "PERM") == 0) {
                    status = get_status_by_code(308);
                } else {
                    status = get_status_by_code(307);
                }
                sprintf(status_line + strlen(status_line), "%d %s\r\n", status.code, status.description);
                sprintf(response_header + strlen(response_header), "Location: %s\r\n", entries[j].redirect_url);
                char response_body[128];
                sprintf(response_body, "<!DOCTYPE html><html><body><h1>%d %s</h1></body></html>", status.code, status.description);
                send_response(sock_fd, status_line, response_header, NULL, 0);

                break;
            }
        }
    }

    // ファイルサイズ取得
    struct stat file_stat;  // ファイルの状態管理
    if (fstat(file_fd, &file_stat) < 0) {
        perror("Could not get file status");
        return;
    }
    int file_size = file_stat.st_size;

    // ステータスライン格納
    status = get_status_by_code(200);
    sprintf(status_line + strlen(status_line), "%d %s\r\n", status.code, status.description);

    // キャッシュあった場合レスポンスヘッダーに付与
    char* cache_control = "";
    if (strstr(url, ".css")) {
        cache_control = "Cache-Control: max-age=300";
    } else if (strstr(url, ".js")) {
        cache_control = "Cache-Control: max-age=300";
    }
    if (strlen(cache_control) > 0) {
         sprintf(response_header + strlen(response_header), "%s\r\n", cache_control);
    }

    // HEADの場合は、ファイルデータ読み込まない
    if (strcmp(method, "HEAD") == 0) {
        send_response(sock_fd, status_line, response_header, NULL, file_size);
        return;
    }

    // GETの場合は、ファイルデータ読み込む
    char* file_data = malloc(file_size);
    if (!file_data) {
        perror("Could not allocate memory for file data");
        return;
    }
    if (read(file_fd, file_data, file_size) < 0) {
        perror("Could not read file data");
        return;
    }
    
    send_response(sock_fd, status_line, response_header, file_data, file_size);

    free(file_data);
    close(file_fd);
}
