#include <string.h>

#include <json-c-parser.h>

void json_parser_extract_object(struct json_object *j, struct json_object **into)
{
	*into = j;
}

void json_parser_extract_array(struct json_object *j, struct array_list **into)
{
	*into = json_object_get_array(j);
}

void json_parser_extract_string(struct json_object *j, const char **s)
{
	*s = json_object_get_string(j);
}

void json_parser_extract_boolean(struct json_object *j, bool *b)
{
	*b = json_object_get_boolean(j);
}

void json_parser_extract_int64(struct json_object *j, int64_t *i)
{
	*i = json_object_get_int64(j);
}

void json_parser_exists(struct json_object *j, bool *b)
{
	*b = true;
}

void json_parser_nest(struct json_object *j, json_parser_table_t *t)
{
	json_parser_t *h;

  json_object_object_foreach(j, key, val) {
		for (h = t->parsers; h < &(t->parsers[t->cnt]); h++) {
		  if (h->type != json_type_any && !json_object_is_type(val, h->type))
		    continue;
			if (strcmp(key, h->name) != 0)
				continue;

			h->func(val, h->ctx);
		}
  }
}

void json_parser_iterate(struct json_object *j, json_parser_func f)
{
	json_parser_cb_t cb = { f, NULL };
	json_parser_iterate_cb(j, &cb);
}

void json_parser_iterate_cb(struct json_object *j, json_parser_cb_t *cb)
{
	size_t len = json_object_array_length(j);
	for (size_t i = 0; i < len; i++) {
		cb->f(json_object_array_get_idx(j, i), cb->ctx);
	}
}