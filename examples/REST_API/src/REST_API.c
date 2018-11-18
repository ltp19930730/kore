#include <kore/kore.h>
#include <kore/http.h>

#include <json-c/json.h>

int		api(struct http_request *);

#define JSON_PRINT_FLAGS JSON_C_TO_STRING_PRETTY|JSON_C_TO_STRING_SPACED
#define JSON_FILE "game.json"
#define API_PATH "/api/"

const char *api_allow[] = {
	"Version",
	"Domain",
	"Status",
	"Game",
};
#define API_ALLOWED_NUM (int)sizeof(api_allow)/sizeof(api_allow[0])
#define MAX_SUBURL 4

int
api(struct http_request *req)
{
	const char *body;
	char *sub_url[MAX_SUBURL];
	struct json_object *games = NULL, *res = NULL;
	int i, j, url_num;

	if (req->method != HTTP_METHOD_POST &&
			req->method != HTTP_METHOD_GET) {
		http_response_header(req, "allow", "POST, GET");
		http_response(req, HTTP_STATUS_METHOD_NOT_ALLOWED, NULL, 0);
		return (KORE_RESULT_OK);
	}

	url_num = kore_split_string(req->path, "/", sub_url, MAX_SUBURL);
	
	/* generate a test json oject */
	games = json_object_from_file(JSON_FILE);
	
	res = games;
	for (i = 0; i < url_num; i++) {
		if (i == 0) {
			if (!strcmp("api", sub_url[i])) {
					continue;
			} else {
					break;
			}
		}
		for (j = 0; j < API_ALLOWED_NUM; j++) {
			if (!strcmp(sub_url[i], api_allow[j])) {
				res = json_object_object_get(res, api_allow[j]);
			}
		}
	}

	body = json_object_to_json_string_ext(res, JSON_PRINT_FLAGS);
	
	http_response_header(req, "content-type", "application/json");
	http_response(req, 200, body, strlen(body));
	
	json_object_put(games);

	return KORE_RESULT_OK;
}
