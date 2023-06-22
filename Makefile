.PHONY: http-server
http-server:
	gcc -o main.out main.c http_server.c http_head_get_request.c http_post_request.c http_status.c common.c validation.c
