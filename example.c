// gcc example.c -Wl,-Bstatic -ljson-c-parser -Wl,-Bdynamic -ljson-c

#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <json-c-parser.h>

static void do_exit(struct json_object *j, void *arg)
{
	(void) arg;
	if (j) {
		printf("got these params: %s\n", json_object_to_json_string(j));
		return;
	}

	exit(EXIT_FAILURE);
}

static json_parser_t command_parser[] = {
	{ "exit", json_type_null, do_exit, NULL },
	{ "exit", json_type_object, do_exit, NULL },
};
json_parser_table(command_parser);

void main(void)
{
	struct json_object *j = json_tokener_parse("{ \"commands\":{\"exit\":{},\"ignored\":{}}, \"id\": 1, \"retry\":\"true\" }");
	printf("parsing this: %s\n", json_object_to_json_string(j));
	int64_t id = 0; bool retry = false; const char *comment = NULL;
	json_parser_t public_parser[] = {
		{ "commands", json_nest(&command_parser_table) },
		{ "id",       json_extract_int64(&id) },
		// the string will live as long as the json_object
		{ "comment",  json_extract_string(&comment) },
		 // retry won't be parsed because it's not a boolean
		{ "retry",    json_extract_boolean(&retry) },
	};
	json_parser_table(public_parser);
	json_parser_nest(j, &public_parser_table);
	printf("id %ld retry %d comment >%s<\n", id, (int) retry, comment);
	json_object_put(j);
}