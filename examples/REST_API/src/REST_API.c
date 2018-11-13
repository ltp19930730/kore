#include <kore/kore.h>
#include <kore/http.h>

#include <json-c/json.h>

int		api(struct http_request *);

#define JSON_PRINT_FLAGS JSON_C_TO_STRING_PRETTY|JSON_C_TO_STRING_SPACED

int
api(struct http_request *req)
{
	const char *body;
	struct json_object *games = NULL;
	struct json_object *status = NULL;

	if (req->method != HTTP_METHOD_POST &&
			req->method != HTTP_METHOD_GET) {
		http_response_header(req, "allow", "POST, GET");
		http_response(req, HTTP_STATUS_METHOD_NOT_ALLOWED, NULL, 0);
		return (KORE_RESULT_OK);
	}

	/* generate a test json oject */
	games = json_object_new_object();
	if (NULL == games) {
		printf("new games json object failed.\n");
		return (KORE_RESULT_OK);
	}

	status = json_object_new_object();
	if (NULL == status) {
		json_object_put(games);
		printf("new status json object failed.\n");
		return (KORE_RESULT_OK);
	}
	// TODO
	json_object_object_add(status, "Version", json_object_new_string("1.2.3.4"));
	json_object_object_add(status, "Domain", json_object_new_string("USA"));

	// TODO
	json_object_object_add(games, "status", status);
	json_object_object_add(games, "game", json_object_new_string("DOTA2"));

	body = json_object_to_json_string_ext(games, JSON_PRINT_FLAGS);

	http_response_header(req, "content-type", "application/json");


	http_response(req, 200, body, strlen(body));
	json_object_put(games);

	return KORE_RESULT_OK;
}
