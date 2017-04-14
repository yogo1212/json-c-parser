#include <string.h>

#include <json-c-parser.h>

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

void json_parser_nest(struct json_object *j, json_parser_table_t *t)
{
	json_parser_t *h;

  json_object_object_foreach(j, key, val) {
		for (h = t->parsers; h < &(t->parsers[t->cnt]); h++) {
		  if (!json_object_is_type(val, h->type))
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
	int len = json_object_array_length(j);
	int i;
	for (i = 0; i < len; i++) {
		cb->f(json_object_array_get_idx(j, i), cb->ctx);
	}
}