#include "cweb.h"

struct cweb_client* cweb_client_init()
{
	return (struct cweb_client*) calloc(1, sizeof (struct cweb_client));
}

int cweb_client_destroy(struct cweb_client* client)
{
	if (client->res.body) free(client->res.body);
	free(client);
	return 0;
}

struct cweb_response* cweb_client_get(struct cweb_client* client, char* host, char* uri)
{
	int n;
	char* body_len_str;

	if (cweb_connect(&client->conn, host)) return 0;

	cweb_request(&client->conn, &client->req, "GET", uri);
	client->req.version = "HTTP/1.0";

	cweb_header(&client->req, "Host", client->conn.host);
	cweb_header(&client->req, "Connection", "close");

	cweb_request_pack(&client->req);

	while ((n = cweb_send(&client->conn, &client->req)) > 0);
	if (n < 0) return 0;

	while ((n = cweb_receive_head(&client->conn, &client->res)) > 0);
	if (n < 0) return 0;

	cweb_response_unpack(&client->res);

	body_len_str = cweb_header(&client->res, "Content-Length", 0);

	if (body_len_str) client->res.body_size = atoi(body_len_str) + 1; // tappo
	else client->res.body_size = CWEB_BODY_SIZE_DEF;

	client->res.body = calloc(1, client->res.body_size);

	while ((n = cweb_receive_body(&client->conn, &client->res)) > 0);
	if (n < 0) return 0;

	if (cweb_close(&client->conn)) return 0;

	return &client->res;
}