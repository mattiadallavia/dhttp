#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define DHTTP_HEADERS_MAX 100
#define DHTTP_BUF_MAX 10000

struct dhttp_connection {
	int socket;
	char* host;
};

struct dhttp_header {
	char* name;
	char* value;
};

struct dhttp_request {
	char* version;
	struct dhttp_header headers[DHTTP_HEADERS_MAX];
	size_t headers_num;
	char* method;
	char* path;
	char buf[DHTTP_BUF_MAX];
	size_t buf_len;
	size_t sent;
	char packed;
};

struct dhttp_response {
	char* version;
	struct dhttp_header headers[DHTTP_HEADERS_MAX];
	size_t headers_num;
	char buf[DHTTP_BUF_MAX];
};

int dhttp_connect(struct dhttp_connection* conn, char* address);
int dhttp_close(struct dhttp_connection* conn);

int dhttp_request(struct dhttp_connection* conn, struct dhttp_request* req, char* method, char* path);
int dhttp_request_pack(struct dhttp_request* req);
int dhttp_send(struct dhttp_connection* conn, struct dhttp_request* req);

int dhttp_receive(struct dhttp_connection* conn);

char* dhttp_header(void* r, char* name, char* val);