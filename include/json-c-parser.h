#ifndef JSON_C_PARSER_H
#define JSON_C_PARSER_H

#include <limits.h>
#include <stdbool.h>

#include <json-c/json.h>

typedef void (*json_parser_func)(struct json_object *j, void *ctx);

typedef struct {
	json_parser_func f;
	void *ctx;
} json_parser_cb_t;

typedef struct {
	const char *name;
	enum json_type type;
	json_parser_func func;
	void *ctx;
} json_parser_t;

typedef struct {
	json_parser_t *parsers;
	size_t cnt;
} json_parser_table_t;

#define json_type_any INT_MIN

#define json_parser_table(h) json_parser_table_t h##_table = {\
  h, sizeof(h) / sizeof(h[0]),\
}

void json_parser_nest(struct json_object *j, json_parser_table_t *t);
#define json_nest(t) json_type_object, (json_parser_func) json_parser_nest, t

void json_parser_extract_object(struct json_object *j, struct json_object **into);
#define json_extract_object(i) json_type_object, (json_parser_func) json_parser_extract_object, i

void json_parser_extract_array(struct json_object *j, struct array_list **into);
#define json_extract_array(i) json_type_array, (json_parser_func) json_parser_extract_array, i

void json_parser_extract_array_len(struct json_object *j, size_t *l);
#define json_extract_array_len(l) json_type_array, (json_parser_func) json_parser_extract_array_len, l

void json_parser_extract_string(struct json_object *j, const char **s);
#define json_extract_string(i) json_type_string, (json_parser_func) json_parser_extract_string, i

void json_parser_extract_boolean(struct json_object *j, bool *b);
#define json_extract_boolean(i) json_type_boolean, (json_parser_func) json_parser_extract_boolean, i

void json_parser_extract_double(struct json_object *j, double *d);
#define json_extract_double(d) json_type_double, (json_parser_func) json_parser_extract_double, d

void json_parser_exists(struct json_object *j, bool *b);
#define json_exists_any(b) json_type_any, (json_parser_func) json_parser_exists, b
#define json_exists(t, b) t, (json_parser_func) json_parser_exists, b

void json_parser_extract_int64(struct json_object *j, int64_t *i);
#define json_extract_int64(i) json_type_int, (json_parser_func) json_parser_extract_int64, i

void json_parser_iterate(struct json_object *j, json_parser_func f);
#define json_iterate(f) json_type_array, (json_parser_func) json_parser_iterate, f
void json_parser_iterate_cb(struct json_object *j, json_parser_cb_t *cb);
#define json_iterate_cb(cb) json_type_array, (json_parser_func) json_parser_iterate_cb, cb

#endif
