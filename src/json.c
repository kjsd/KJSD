/**
 * @file json.c
 *
 * @version $Id:$
 *
 * @brief
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 ***********************************************************************/
#include "../include/kjsd/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERROR                      0
#define SUCCESS                    1
#define STARTING_CAPACITY         15
#define ARRAY_MAX_CAPACITY    122880 /* 15*(2^13) */
#define OBJECT_MAX_CAPACITY      960 /* 15*(2^6)  */
#define MAX_NESTING               19
#define sizeof_token(a)       (sizeof(a) - 1)
#define skip_char(str)        ((*str)++)
#define skip_whitespaces(str) while (isspace(**string)) { skip_char(string); }
#define MAX(a, b)             ((a) > (b) ? (a) : (b))

#define json_malloc(a)     malloc(a)
#define json_free(a)       free((void*)a)
#define json_realloc(a, b) realloc(a, b)

/* Type definitions */
typedef union
{
    const char *string;
    double number;
    KJSD_JSON_Object *object;
    KJSD_JSON_Array *array;
    int bool;
    int null;
} json_value_value;

struct KJSD_JSON_ValueT
{
    KJSD_JSON_Type type;
    json_value_value value;
};

struct KJSD_JSON_ObjectT
{
    const char **names;
    KJSD_JSON_Value **values;
    size_t count;
    size_t capacity;
};

struct KJSD_JSON_ArrayT
{
    KJSD_JSON_Value **items;
    size_t count;
    size_t capacity;
};

typedef enum
{
    JSON_DEV_FILE,
    JSON_DEV_MEM,

    JSON_DEV_NULL
} json_accessor_type;

typedef struct
{
    char* buf;
    size_t size;
} json_accessor_str;

typedef union
{
    FILE* file;
    json_accessor_str str;
} json_accessor_obj;

typedef struct
{
    json_accessor_type sym;
    json_accessor_obj obj;
} json_accessor;

/* Various */
static int try_realloc(void **ptr, size_t new_size);
static char* json_strndup(const char *string, size_t n);
static int is_utf(const unsigned char *string);
static int is_decimal(const char *string, size_t length);
static const char* json_escape_string(const char* src);
static size_t json_strchg(const char* src,
                          const char* old_str, const char* new_str,
                          char* dst, size_t dst_sz);

/* JSON Object */
static KJSD_JSON_Object* json_object_init(void);
static int json_object_add(KJSD_JSON_Object *object,
                           const char *name, KJSD_JSON_Value *value);
static int json_object_resize(KJSD_JSON_Object *object,
                              size_t capacity);
static KJSD_JSON_Value* json_object_nget_value(
    const KJSD_JSON_Object *object, const char *name, size_t n);
static void json_object_free(KJSD_JSON_Object *object);

/* JSON Array */
static KJSD_JSON_Array* json_array_init(void);
static int json_array_add(KJSD_JSON_Array *array,
                          KJSD_JSON_Value *value);
static int json_array_resize(KJSD_JSON_Array *array,
                             size_t capacity);
static void json_array_free(KJSD_JSON_Array *array);

/* JSON Value */
static KJSD_JSON_Value* json_value_init_object(void);
static KJSD_JSON_Value* json_value_init_array(void);
static KJSD_JSON_Value* json_value_init_string(const char *string);
static KJSD_JSON_Value* json_value_init_number(double number);
static KJSD_JSON_Value* json_value_init_bool(int bool);
static KJSD_JSON_Value* json_value_init_null(void);

/* Parser */
static void skip_quotes(const char **string);
static const char* get_processed_string(const char **string);
static KJSD_JSON_Value* parse_object_value(const char **string,
                                           size_t nesting);
static KJSD_JSON_Value* parse_array_value(const char **string,
                                          size_t nesting);
static KJSD_JSON_Value* parse_string_value(const char **string);
static KJSD_JSON_Value* parse_bool_value(const char **string);
static KJSD_JSON_Value* parse_number_value(const char **string);
static KJSD_JSON_Value* parse_null_value(const char **string);
static KJSD_JSON_Value* parse_value(const char **string,
                                    size_t nesting);

/* Serializer */
static size_t json_serialize(const KJSD_JSON_Value* value,
                             json_accessor* out);
static size_t json_serialize_object(KJSD_JSON_Object* value,
                                    json_accessor* out);
static size_t json_serialize_array(KJSD_JSON_Array* value,
                                   json_accessor* out);
static size_t json_serialize_string(const char* value,
                                    json_accessor* out);
static size_t json_serialize_number(double value,
                                    json_accessor* out);
static size_t json_serialize_bool(int value,
                                  json_accessor* out);
static size_t json_serialize_vanilla(const char* value,
                                     json_accessor* out);


/* Various */
static int try_realloc(void **ptr, size_t new_size)
{
    void *reallocated_ptr = json_realloc(*ptr, new_size);
    if (!reallocated_ptr) return ERROR;

    *ptr = reallocated_ptr;
    return SUCCESS;
}

static char* json_strndup(const char *string, size_t n)
{
    char *output_string = (char*)json_malloc(n + 1);
    if (!output_string) return NULL;

    output_string[n] = '\0';
    strncpy(output_string, string, n);
    return output_string;
}

static int is_utf(const unsigned char *s)
{
    return isxdigit(s[0]) && isxdigit(s[1])
        && isxdigit(s[2]) && isxdigit(s[3]);
}

static int is_decimal(const char *string, size_t length)
{
    if (length > 1 && string[0] == '0' && string[1] != '.') return 0;
    if (length > 2 && !strncmp(string, "-0", 2) && string[2] != '.')
        return 0;

    while (length--)
    {
        if (strchr("xX", string[length])) return 0;
    }
    return 1;
}

/* JSON Object */
static KJSD_JSON_Object * json_object_init(void)
{
    KJSD_JSON_Object *new_obj =
        (KJSD_JSON_Object*)json_malloc(sizeof(KJSD_JSON_Object));
    if (!new_obj) return NULL;

    new_obj->names = (const char**)NULL;
    new_obj->values = (KJSD_JSON_Value**)NULL;
    new_obj->capacity = 0;
    new_obj->count = 0;
    return new_obj;
}

static int json_object_add(KJSD_JSON_Object *object,
                           const char *name, KJSD_JSON_Value *value)
{
    size_t index;

    if (object->count >= object->capacity)
    {
        size_t new_capacity = MAX(object->capacity * 2, STARTING_CAPACITY);
        if (new_capacity > OBJECT_MAX_CAPACITY) return ERROR;
        if (json_object_resize(object, new_capacity) == ERROR) 
            return ERROR;
    }
    if (KJSD_JSON_objectGetValue(object, name) != NULL) 
        return ERROR;

    index = object->count;
    object->names[index] = json_strndup(name, strlen(name));
    if (!object->names[index]) return ERROR;

    object->values[index] = value;
    object->count++;
    return SUCCESS;
}

static int json_object_resize(KJSD_JSON_Object *object,
                              size_t capacity)
{
    if (try_realloc((void**)&object->names,
                    capacity * sizeof(char*)) == ERROR) return ERROR;
    if (try_realloc((void**)&object->values,
                    capacity * sizeof(KJSD_JSON_Value*)) == ERROR)
        return ERROR;

    object->capacity = capacity;
    return SUCCESS;
}

static KJSD_JSON_Value* json_object_nget_value(
    const KJSD_JSON_Object *object, const char *name, size_t n)
{
    size_t i, name_length;

    for (i = 0; i < KJSD_JSON_objectGetCount(object); i++)
    {
        name_length = strlen(object->names[i]);
        if (name_length != n) continue;
        if (strncmp(object->names[i], name, n) == 0) 
        {
            return object->values[i];
        }
    }

    return NULL;
}

static void json_object_free(KJSD_JSON_Object *object)
{
    while (object->count--)
    {
        json_free(object->names[object->count]);
        KJSD_JSON_free(object->values[object->count]);
    }
    json_free(object->names);
    json_free(object->values);
    json_free(object);
}

/* JSON Array */
static KJSD_JSON_Array * json_array_init(void) 
{
    KJSD_JSON_Array *new_array =
        (KJSD_JSON_Array*)json_malloc(sizeof(KJSD_JSON_Array));
    if (!new_array) return NULL;

    new_array->items = (KJSD_JSON_Value**)NULL;
    new_array->capacity = 0;
    new_array->count = 0;
    return new_array;
}

static int json_array_add(KJSD_JSON_Array *array,
                          KJSD_JSON_Value *value)
{
    if (array->count >= array->capacity)
    {
        size_t new_capacity = MAX(array->capacity * 2, STARTING_CAPACITY);
        if (new_capacity > ARRAY_MAX_CAPACITY) return ERROR;
        if (!json_array_resize(array, new_capacity)) return ERROR;
    }

    array->items[array->count] = value;
    array->count++;
    return SUCCESS;
}

static int json_array_resize(KJSD_JSON_Array *array, size_t capacity)
{
    if (try_realloc((void**)&array->items,
                    capacity * sizeof(KJSD_JSON_Value*)) == ERROR)
        return ERROR;

    array->capacity = capacity;
    return SUCCESS;
}

static void json_array_free(KJSD_JSON_Array *array)
{
    while (array->count--)
    {
        KJSD_JSON_free(array->items[array->count]);
    }
    json_free(array->items);
    json_free(array);
}

/* JSON Value */
static KJSD_JSON_Value* json_value_init_object(void)
{
    KJSD_JSON_Value* new_value =
        (KJSD_JSON_Value*)json_malloc(sizeof(KJSD_JSON_Value));
    if (!new_value) return NULL;

    new_value->type = KJSD_JSON_OBJECT;
    new_value->value.object = json_object_init();

    if (!new_value->value.object)
    {
        json_free(new_value);
        return NULL;
    }
    return new_value;
}

static KJSD_JSON_Value * json_value_init_array(void)
{
    KJSD_JSON_Value *new_value =
        (KJSD_JSON_Value*)json_malloc(sizeof(KJSD_JSON_Value));
    if (!new_value) return NULL;

    new_value->type = KJSD_JSON_ARRAY;
    new_value->value.array = json_array_init();

    if (!new_value->value.array)
    {
        json_free(new_value);
        return NULL;
    }
    return new_value;
}

static KJSD_JSON_Value * json_value_init_string(const char *string)
{
    KJSD_JSON_Value *new_value =
        (KJSD_JSON_Value*)json_malloc(sizeof(KJSD_JSON_Value));
    if (!new_value) return NULL;

    new_value->type = KJSD_JSON_STRING;
    new_value->value.string = string;
    return new_value;
}

static KJSD_JSON_Value * json_value_init_number(double number)
{
    KJSD_JSON_Value *new_value =
        (KJSD_JSON_Value*)json_malloc(sizeof(KJSD_JSON_Value));
    if (!new_value) return NULL;

    new_value->type = KJSD_JSON_NUMBER;
    new_value->value.number = number;
    return new_value;
}

static KJSD_JSON_Value * json_value_init_bool(int bool)
{
    KJSD_JSON_Value *new_value =
        (KJSD_JSON_Value*)json_malloc(sizeof(KJSD_JSON_Value));
    if (!new_value) return NULL;

    new_value->type = KJSD_JSON_BOOL;
    new_value->value.bool = bool;
    return new_value;
}

static KJSD_JSON_Value * json_value_init_null(void)
{
    KJSD_JSON_Value *new_value =
        (KJSD_JSON_Value*)json_malloc(sizeof(KJSD_JSON_Value));
    if (!new_value) return NULL;

    new_value->type = KJSD_JSON_NULL;
    return new_value;
}

/* Parser */
static void skip_quotes(const char **string)
{
    skip_char(string);
    while (**string != '\"')
    {
        if (**string == '\0') return;
        if (**string == '\\')
        {
            skip_char(string);
            if (**string == '\0') return;
        }
        skip_char(string);
    }
    skip_char(string);
}

/* Returns contents of a string inside double quotes and parses escaped
   characters inside.
   Example: "\u006Corem ipsum" -> lorem ipsum */
static const char * get_processed_string(const char **string)
{
    const char *string_start = *string;
    char *output, *processed_ptr, *unprocessed_ptr, current_char;
    unsigned int utf_val;

    skip_quotes(string);
    if (**string == '\0') return NULL;

    output = json_strndup(string_start + 1, *string  - string_start - 2);
    if (!output) return NULL;

    processed_ptr = unprocessed_ptr = output;
    while (*unprocessed_ptr)
    {
        current_char = *unprocessed_ptr;
        if (current_char == '\\')
        {
            unprocessed_ptr++;
            current_char = *unprocessed_ptr;
            switch (current_char)
            {
            case '\"': case '\\': case '/': break;
            case 'b': current_char = '\b'; break;
            case 'f': current_char = '\f'; break;
            case 'n': current_char = '\n'; break;
            case 'r': current_char = '\r'; break;
            case 't': current_char = '\t'; break;
            case 'u':
                unprocessed_ptr++;
                if (!is_utf((const unsigned char*)unprocessed_ptr) ||
                    sscanf(unprocessed_ptr, "%4x", &utf_val) == EOF)
                {
                    json_free(output);
                    return NULL;
                }
                if (utf_val < 0x80)
                {
                    current_char = utf_val;
                }
                else if (utf_val < 0x800)
                {
                    *processed_ptr++ = (utf_val >> 6) | 0xC0;
                    current_char = ((utf_val | 0x80) & 0xBF);
                }
                else
                {
                    *processed_ptr++ = (utf_val >> 12) | 0xE0;
                    *processed_ptr++ = (((utf_val >> 6) | 0x80) & 0xBF);
                    current_char = ((utf_val | 0x80) & 0xBF);
                }
                unprocessed_ptr += 3;
                break;
            default:
                json_free(output);
                return NULL;
                break;
            }
        }
        else if ((unsigned char)current_char < 0x20)
        {
            /* 0x00-0x19 are invalid characters for json string
               (http://www.ietf.org/rfc/rfc4627.txt) */
            json_free(output);
            return NULL;
        }
        *processed_ptr = current_char;
        processed_ptr++;
        unprocessed_ptr++;
    }
    *processed_ptr = '\0';
    if (try_realloc((void**)&output, strlen(output) + 1) == ERROR)
        return NULL;

    return output;
}

static KJSD_JSON_Value * parse_value(const char **string,
                                     size_t nesting)
{
    if (nesting > MAX_NESTING) return NULL;

    skip_whitespaces(string);
    switch (**string)
    {
    case '{':
        return parse_object_value(string, nesting + 1);
    case '[':
        return parse_array_value(string, nesting + 1);
    case '\"':
        return parse_string_value(string);
    case 'f': case 't':
        return parse_bool_value(string);
    case '-':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return parse_number_value(string);
    case 'n':
        return parse_null_value(string);
    default:
        return NULL;
    }
}

static KJSD_JSON_Value * parse_object_value(const char **string,
                                            size_t nesting)
{
    KJSD_JSON_Value* output_value = json_value_init_object();
    KJSD_JSON_Value* new_value = NULL;
    KJSD_JSON_Object* output_object =
        KJSD_JSON_valueGetObject(output_value);
    const char *new_key = NULL;

    if (!output_value) return NULL;

    skip_char(string);
    skip_whitespaces(string);

    if (**string == '}')
    {
        /* empty object */
        skip_char(string);
        return output_value;
    }
    while (**string != '\0')
    {
        new_key = get_processed_string(string);
        skip_whitespaces(string);
        if (!new_key || **string != ':')
        {
            KJSD_JSON_free(output_value);
            return NULL;
        }
        skip_char(string);
        new_value = parse_value(string, nesting);
        if (!new_value)
        {
            json_free(new_key);
            KJSD_JSON_free(output_value);
            return NULL;
        }
        if (!json_object_add(output_object, new_key, new_value))
        {
            json_free(new_key);
            json_free(new_value);
            KJSD_JSON_free(output_value);
            return NULL;
        }
        json_free(new_key);
        skip_whitespaces(string);
        if (**string != ',') break;

        skip_char(string);
        skip_whitespaces(string);
    }
    skip_whitespaces(string);
    if ((**string != '}') ||
        json_object_resize(output_object,
                           KJSD_JSON_objectGetCount(output_object))
        == ERROR)
    {
        /* Trim object after parsing is over */
        KJSD_JSON_free(output_value);
        return NULL;
    }

    skip_char(string);
    return output_value;
}

static KJSD_JSON_Value * parse_array_value(const char **string,
                                           size_t nesting)
{
    KJSD_JSON_Value* output_value = json_value_init_array();
    KJSD_JSON_Value* new_array_value = NULL;
    KJSD_JSON_Array* output_array =
        KJSD_JSON_valueGetArray(output_value);
    if (!output_value) return NULL;

    skip_char(string);
    skip_whitespaces(string);
    if (**string == ']')
    {
        /* empty array */
        skip_char(string);
        return output_value;
    }
    while (**string != '\0')
    {
        new_array_value = parse_value(string, nesting);
        if (!new_array_value)
        {
            KJSD_JSON_free(output_value);
            return NULL;
        }
        if (json_array_add(output_array, new_array_value) == ERROR)
        {
            json_free(new_array_value);
            KJSD_JSON_free(output_value);
            return NULL;
        }
        skip_whitespaces(string);
        if (**string != ',') break;

        skip_char(string);
        skip_whitespaces(string);
    }
    skip_whitespaces(string);
    if ((**string != ']') ||
        json_array_resize(output_array,
                          KJSD_JSON_arrayGetCount(output_array))
        == ERROR)
    {
        /* Trim array after parsing is over */
        KJSD_JSON_free(output_value);
        return NULL;
    }

    skip_char(string);
    return output_value;
}

static KJSD_JSON_Value * parse_string_value(const char **string)
{
    const char *new_string = get_processed_string(string);
    if (!new_string) return NULL;

    return json_value_init_string(new_string);
}

static KJSD_JSON_Value * parse_bool_value(const char **string)
{
    size_t true_token_size = sizeof_token("true");
    size_t false_token_size = sizeof_token("false");

    if (strncmp("true", *string, true_token_size) == 0)
    {
        *string += true_token_size;
        return json_value_init_bool(1);
    }
    else if (strncmp("false", *string, false_token_size) == 0)
    {
        *string += false_token_size;
        return json_value_init_bool(0);
    }
    return NULL;
}

static KJSD_JSON_Value * parse_number_value(const char **string)
{
    char *end;
    double number = strtod(*string, &end);
    KJSD_JSON_Value *output_value;

    if (is_decimal(*string, end - *string))
    {
        *string = end;
        output_value = json_value_init_number(number);
    }
    else
    {
        output_value = NULL;
    }
    return output_value;
}

static KJSD_JSON_Value * parse_null_value(const char **string)
{
    size_t token_size = sizeof_token("null");
    if (strncmp("null", *string, token_size) == 0)
    {
        *string += token_size;
        return json_value_init_null();
    }
    return NULL;
}

/* Parser API */
KJSD_JSON_Value* KJSD_JSON_deserialize(FILE* in)
{
    size_t file_size;
    char *file_contents;
    KJSD_JSON_Value *output_value;

    if (!in) return NULL;

    fseek(in, 0L, SEEK_END);
    file_size = ftell(in);
    rewind(in);
    file_contents = (char*)json_malloc(sizeof(char) * (file_size + 1));
    if (!file_contents)
    {
        return NULL;
    }

    fread(file_contents, file_size, 1, in);
    file_contents[file_size] = '\0';
    output_value = KJSD_JSON_deserializeS(file_contents);
    json_free(file_contents);

    return output_value;
}

KJSD_JSON_Value* KJSD_JSON_deserializeS(const char *string)
{
    if (!string || (*string != '{' && *string != '[')) return NULL;

    return parse_value((const char**)&string, 0);
}

/* JSON Object API */
KJSD_JSON_Value* KJSD_JSON_objectGetValue(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return NULL;

    return json_object_nget_value(object, name, strlen(name));
}

const char* KJSD_JSON_objectGetString(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return NULL;

    return KJSD_JSON_valueGetString(
        KJSD_JSON_objectGetValue(object, name));
}

double KJSD_JSON_objectGetNumber(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return 0;

    return KJSD_JSON_valueGetNumber(
        KJSD_JSON_objectGetValue(object, name));
}

KJSD_JSON_Object* KJSD_JSON_objectGetObject(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return NULL;

    return KJSD_JSON_valueGetObject(
        KJSD_JSON_objectGetValue(object, name));
}

KJSD_JSON_Array* KJSD_JSON_objectGetArray(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return NULL;

    return KJSD_JSON_valueGetArray(
        KJSD_JSON_objectGetValue(object, name));
}

int KJSD_JSON_objectGetBool(const KJSD_JSON_Object *object,
                            const char *name)
{
    if ((object == NULL) || (name == NULL)) return -1;

    return KJSD_JSON_valueGetBool(
        KJSD_JSON_objectGetValue(object, name));
}

KJSD_JSON_Value * KJSD_JSON_objectDotGetValue(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return NULL;

    const char *dot_position = strchr(name, '.');
    if (!dot_position)
    {
        return KJSD_JSON_objectGetValue(object, name);
    }

    object = KJSD_JSON_valueGetObject(
        json_object_nget_value(object, name, dot_position - name));
    return KJSD_JSON_objectDotGetValue(object, dot_position + 1);
}

const char* KJSD_JSON_objectDotGetString(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return NULL;

    return KJSD_JSON_valueGetString(
        KJSD_JSON_objectDotGetValue(object, name));
}

double KJSD_JSON_objectDotGetNumber(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return 0;

    return KJSD_JSON_valueGetNumber(
        KJSD_JSON_objectDotGetValue(object, name));
}

KJSD_JSON_Object* KJSD_JSON_objectDotGetObject(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return NULL;

    return KJSD_JSON_valueGetObject(
        KJSD_JSON_objectDotGetValue(object, name));
}

KJSD_JSON_Array* KJSD_JSON_objectDotGetArray(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return NULL;

    return KJSD_JSON_valueGetArray(
        KJSD_JSON_objectDotGetValue(object, name));
}

int KJSD_JSON_objectDotGetBool(
    const KJSD_JSON_Object *object, const char *name)
{
    if ((object == NULL) || (name == NULL)) return -1;

    return KJSD_JSON_valueGetBool(
        KJSD_JSON_objectDotGetValue(object, name));
}

size_t KJSD_JSON_objectGetCount(const KJSD_JSON_Object *object)
{
    return object ? object->count : 0;
}

const char* KJSD_JSON_objectGetName(
    const KJSD_JSON_Object *object, size_t index)
{
    if (index >= KJSD_JSON_objectGetCount(object)) return NULL;
    return object->names[index];
}

/* JSON Array API */
KJSD_JSON_Value* KJSD_JSON_arrayGetValue(
    const KJSD_JSON_Array *array, size_t index)
{
    if (index >= KJSD_JSON_arrayGetCount(array)) return NULL;
    return array->items[index];
}

const char* KJSD_JSON_arrayGetString(
    const KJSD_JSON_Array *array, size_t index)
{
    return KJSD_JSON_valueGetString(
        KJSD_JSON_arrayGetValue(array, index));
}

double KJSD_JSON_arrayGetNumber(const KJSD_JSON_Array *array,
                                size_t index)
{
    return KJSD_JSON_valueGetNumber(
        KJSD_JSON_arrayGetValue(array, index));
}

KJSD_JSON_Object* KJSD_JSON_arrayGetObject(
    const KJSD_JSON_Array *array, size_t index)
{
    return KJSD_JSON_valueGetObject(
        KJSD_JSON_arrayGetValue(array, index));
}

KJSD_JSON_Array* KJSD_JSON_arrayGetArray(
    const KJSD_JSON_Array *array, size_t index)
{
    return KJSD_JSON_valueGetArray(
        KJSD_JSON_arrayGetValue(array, index));
}

int KJSD_JSON_arrayGetBool(const KJSD_JSON_Array *array,
                           size_t index)
{
    return KJSD_JSON_valueGetBool(
        KJSD_JSON_arrayGetValue(array, index));
}

size_t KJSD_JSON_arrayGetCount(const KJSD_JSON_Array *array)
{
    return array ? array->count : 0;
}

/* JSON Value API */
KJSD_JSON_Type KJSD_JSON_valueGetType(
    const KJSD_JSON_Value *value)
{
    return value ? value->type : KJSD_JSON_ERROR;
}

KJSD_JSON_Object* KJSD_JSON_valueGetObject(
    const KJSD_JSON_Value *value)
{
    return KJSD_JSON_valueGetType(value) == KJSD_JSON_OBJECT ?
        value->value.object : NULL;
}

KJSD_JSON_Array* KJSD_JSON_valueGetArray(
    const KJSD_JSON_Value *value)
{
    return KJSD_JSON_valueGetType(value) == KJSD_JSON_ARRAY ?
        value->value.array : NULL;
}

const char* KJSD_JSON_valueGetString(
    const KJSD_JSON_Value *value)
{
    return KJSD_JSON_valueGetType(value) == KJSD_JSON_STRING ?
        value->value.string : NULL;
}

double KJSD_JSON_valueGetNumber(const KJSD_JSON_Value *value)
{
    return KJSD_JSON_valueGetType(value) == KJSD_JSON_NUMBER ?
        value->value.number : 0;
}

int KJSD_JSON_valueGetBool(const KJSD_JSON_Value *value)
{
    return KJSD_JSON_valueGetType(value) == KJSD_JSON_BOOL ?
        value->value.bool : -1;
}

void KJSD_JSON_free(KJSD_JSON_Value *value)
{
    switch (KJSD_JSON_valueGetType(value))
    {
    case KJSD_JSON_OBJECT:
        json_object_free(value->value.object);
        break;
    case KJSD_JSON_STRING:
        if (value->value.string)
        {
            json_free(value->value.string);
        }
        break;
    case KJSD_JSON_ARRAY:
        json_array_free(value->value.array);
        break;
    default:
        break;
    }
    json_free(value);
}

KJSD_JSON_Value* KJSD_JSON_createRoot(void)
{
    return json_value_init_object();
}

KJSD_JSON_Object* KJSD_JSON_objectAddObject(
    KJSD_JSON_Object* base,
    const char* name)
{
    KJSD_JSON_Value* new_val = NULL;

    if ((base == NULL) || (name == NULL)) return ERROR;

    new_val = json_value_init_object();
    if (json_object_add(base, name, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        return NULL;
    }
    return KJSD_JSON_valueGetObject(new_val);
}

KJSD_JSON_Array* KJSD_JSON_objectAddArray(
    KJSD_JSON_Object* base, const char* name)
{
    KJSD_JSON_Value* new_val = NULL;

    if ((base == NULL) || (name == NULL)) return ERROR;

    new_val = json_value_init_array();
    if (json_object_add(base, name, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        new_val = NULL;
    }
    return KJSD_JSON_valueGetArray(new_val);
}

int KJSD_JSON_objectAddString(KJSD_JSON_Object* base,
                              const char* name,
                              const char* value)
{
    KJSD_JSON_Value* new_val = NULL;
    const char *new_string = NULL;

    if ((base == NULL) || (name == NULL)) return ERROR;

    if (value != NULL)
    {
        new_string = json_strndup(value, strlen(value));
    }

    new_val = json_value_init_string(new_string);
    if (json_object_add(base, name, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        return ERROR;
    }
    return SUCCESS;
}

int KJSD_JSON_objectAddNumber(KJSD_JSON_Object* base,
                              const char* name,
                              double value)
{
    KJSD_JSON_Value* new_val = NULL;

    if ((base == NULL) || (name == NULL)) return ERROR;

    new_val = json_value_init_number(value);
    if (json_object_add(base, name, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        return ERROR;
    }
    return SUCCESS;
}

int KJSD_JSON_objectAddBool(KJSD_JSON_Object* base,
                            const char* name, int value)
{
    KJSD_JSON_Value* new_val = NULL;

    if ((base == NULL) || (name == NULL)) return ERROR;

    new_val = json_value_init_bool(value);
    if (json_object_add(base, name, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        return ERROR;
    }
    return SUCCESS;
}

KJSD_JSON_Object* KJSD_JSON_arrayAddObject(
    KJSD_JSON_Array* base)
{
    KJSD_JSON_Value* new_val = NULL;

    if (base == NULL) return ERROR;

    new_val = json_value_init_object();
    if (json_array_add(base, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        return NULL;
    }
    return KJSD_JSON_valueGetObject(new_val);
}

KJSD_JSON_Array* KJSD_JSON_arrayAddArray(
    KJSD_JSON_Array* base)
{
    KJSD_JSON_Value* new_val = NULL;

    if (base == NULL) return ERROR;

    new_val = json_value_init_array();
    if (json_array_add(base, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        return NULL;
    }
    return KJSD_JSON_valueGetArray(new_val);
}

int KJSD_JSON_arrayAddString(KJSD_JSON_Array* base,
                             const char* value)
{
    KJSD_JSON_Value* new_val = NULL;
    const char *new_string = NULL;

    if (base == NULL) return ERROR;

    if (value != NULL)
    {
        new_string = json_strndup(value, strlen(value));
    }

    new_val = json_value_init_string(new_string);
    if (json_array_add(base, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        return ERROR;
    }
    return SUCCESS;
}

int KJSD_JSON_arrayAddNumber(KJSD_JSON_Array* base,
                             double value)
{
    KJSD_JSON_Value* new_val = NULL;

    if (base == NULL) return ERROR;

    new_val = json_value_init_number(value);
    if (json_array_add(base, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        return ERROR;
    }
    return SUCCESS;
}

int KJSD_JSON_arrayAddBool(KJSD_JSON_Array* base, int value)
{
    KJSD_JSON_Value* new_val = NULL;

    if (base == NULL) return ERROR;

    new_val = json_value_init_bool(value);
    if (json_array_add(base, new_val) == ERROR)
    {
        KJSD_JSON_free(new_val);
        return ERROR;
    }
    return SUCCESS;
}

size_t KJSD_JSON_serialize(const KJSD_JSON_Value* value, FILE* out)
{
    json_accessor outer = {
        JSON_DEV_FILE, { NULL }
    };
    outer.obj.file = out;

    if ((value == NULL) || (outer.obj.file == NULL)) return 0;

    return json_serialize(value, &outer);
}

size_t KJSD_JSON_serializeS(const KJSD_JSON_Value* value,
                            char* out, size_t size)
{
    json_accessor outer = {
        JSON_DEV_MEM, { NULL }
    };
    outer.obj.str.buf = out;
    outer.obj.str.size = size;

    if ((value == NULL) || (outer.obj.str.buf == NULL)) return 0;
    
    return json_serialize(value, &outer);
}

size_t KJSD_JSON_sizeof(const KJSD_JSON_Value* value)
{
    json_accessor outer = {
        JSON_DEV_NULL, { NULL }
    };

    if (value == NULL) return 0;
    
    return json_serialize(value, &outer);
}

size_t json_serialize(const KJSD_JSON_Value* value, json_accessor* out)
{
    switch (KJSD_JSON_valueGetType(value))
    {
    case KJSD_JSON_OBJECT:
        return json_serialize_object(KJSD_JSON_valueGetObject(value),
                                     out);
    case KJSD_JSON_ARRAY:
        return json_serialize_array(KJSD_JSON_valueGetArray(value),
                                    out);
    case KJSD_JSON_STRING:
        return json_serialize_string(KJSD_JSON_valueGetString(value),
                                     out);
    case KJSD_JSON_NUMBER:
        return json_serialize_number(KJSD_JSON_valueGetNumber(value),
                                     out);
    case KJSD_JSON_BOOL:
        return json_serialize_bool(KJSD_JSON_valueGetBool(value),
                                   out);
    default:
        break;
    }

    return 0;
}

size_t json_serialize_object(KJSD_JSON_Object* value, json_accessor* out)
{
    size_t i;
    size_t num = KJSD_JSON_objectGetCount(value);
    size_t sz = 0;

    sz += json_serialize_vanilla("{", out);
    for (i = 0; i < num; i++)
    {
        const char* name = KJSD_JSON_objectGetName(value, i);

        sz += json_serialize_string(name, out);
        sz += json_serialize_vanilla(":", out);

        sz += json_serialize(KJSD_JSON_objectGetValue(value, name), out);
        if (i < (num - 1))
        {
            sz += json_serialize_vanilla(",", out);
        }
    }
    sz += json_serialize_vanilla("}", out);

    return sz;
}

size_t json_serialize_array(KJSD_JSON_Array* value, json_accessor* out)
{
    size_t i;
    size_t num = KJSD_JSON_arrayGetCount(value);
    size_t sz = 0;

    sz += json_serialize_vanilla("[", out);
    for (i = 0; i < num; i++)
    {
        sz += json_serialize(KJSD_JSON_arrayGetValue(value, i), out);
        if (i < (num - 1))
        {
            sz += json_serialize_vanilla(",", out);
        }
    }
    sz += json_serialize_vanilla("]", out);

    return sz;
}

size_t json_serialize_string(const char* value, json_accessor* out)
{
    const char* escaped = json_escape_string(value);
    const char* fmt = (escaped == NULL ? "null": "\"%s\"");
    size_t sz = 0;

    switch (out->sym)
    {
    case JSON_DEV_FILE:
        sz = fprintf(out->obj.file, fmt, escaped);
        break;
    case JSON_DEV_MEM:
        sz = snprintf(out->obj.str.buf, out->obj.str.size, fmt, escaped);
        if (sz >= out->obj.str.size)
        {
            sz = (out->obj.str.size > 0) ? out->obj.str.size - 1: 0;
        }
        out->obj.str.size -= sz;
        out->obj.str.buf += sz;
        break;
    case JSON_DEV_NULL:
        sz = snprintf(NULL, 0, fmt, escaped);
        break;
    default:
        break;
    }

    json_free(escaped);
    return sz;
}

size_t json_serialize_number(double value, json_accessor* out)
{
    const char* fmt = ((value - (int)value) == 0) ? "%.0f": "%f";
    size_t sz = 0;

    switch (out->sym)
    {
    case JSON_DEV_FILE:
        sz = fprintf(out->obj.file, fmt, value);
        break;
    case JSON_DEV_MEM:
        sz = snprintf(out->obj.str.buf, out->obj.str.size, fmt, value);
        if (sz >= out->obj.str.size)
        {
            sz = (out->obj.str.size > 0) ? out->obj.str.size - 1: 0;
        }
        out->obj.str.size -= sz;
        out->obj.str.buf += sz;
        break;
    case JSON_DEV_NULL:
        sz = snprintf(NULL, 0, fmt, value);
        break;
    default:
        break;
    }
    return sz;
}

size_t json_serialize_bool(int value, json_accessor* out)
{
    return json_serialize_vanilla((value ? "true": "false"), out);
}

size_t json_serialize_vanilla(const char* value, json_accessor* out)
{
    size_t sz = 0;

    switch (out->sym)
    {
    case JSON_DEV_FILE:
        sz = fprintf(out->obj.file, "%s", value);
        break;
    case JSON_DEV_MEM:
        sz = snprintf(out->obj.str.buf, out->obj.str.size, "%s", value);
        if (sz >= out->obj.str.size)
        {
            sz = (out->obj.str.size > 0) ? out->obj.str.size - 1: 0;
        }
        out->obj.str.size -= sz;
        out->obj.str.buf += sz;
        break;
    case JSON_DEV_NULL:
        sz = snprintf(NULL, 0, "%s", value);
        break;
    default:
        break;
    }

    return sz;
}

size_t json_strchg(const char* src,
                   const char* old_str, const char* new_str,
                   char* dst, size_t dst_sz)
{
    const char* src_p = src;
    char* dst_p = dst;
    size_t cpy_sz = 0;
    const size_t old_sz = strlen(old_str);
    const size_t new_sz = strlen(new_str);
    size_t sz;
    size_t count;

    if ((src == NULL)
        || (old_str == NULL)
        || (new_str == NULL)
        || (dst == NULL)
        || (dst_sz == 0))
    {
        return 0;
    }

    for (count = 0; cpy_sz < dst_sz; count++)
    {
        char* p;

        p = strstr(src_p, old_str);
        if (p == NULL) break;
            
        sz = ((cpy_sz + (p - src_p)) < dst_sz) ?
            p - src_p: dst_sz - cpy_sz - 1;
        strncpy(dst_p, src_p, sz);
        dst_p += sz;
        cpy_sz += sz;

        sz = ((cpy_sz + new_sz) < dst_sz) ? new_sz: dst_sz - cpy_sz - 1;
        strncpy(dst_p, new_str, sz);

        src_p = p + old_sz;
        dst_p += sz;
        cpy_sz += sz;
    }
    sz = ((cpy_sz + strlen(src_p)) < dst_sz) ?
        strlen(src_p): dst_sz - cpy_sz - 1;
    strncpy(dst_p, src_p, sz);
    dst[cpy_sz + sz] = '\0';

    return count;
}

const char* json_escape_string(const char* src)
{
    const char* p = src;
    char* out = NULL;
    char* buf = NULL;
    size_t out_sz = 0;

    if (src == NULL) return NULL;

    out_sz = strlen(src) + 1;
    for (p = strchr(src, '\\'); p != NULL; p = strchr(++p, '\\')) out_sz++;
    for (p = strchr(src, '"'); p != NULL; p = strchr(++p, '"')) out_sz++;
    for (p = strchr(src, '/'); p != NULL; p = strchr(++p, '/')) out_sz++;

    out = (char*)json_malloc(out_sz);
    if (out == NULL) return NULL;
    buf = (char*)json_malloc(out_sz);
    if (buf == NULL)
    {
        json_free(out);
        return NULL;
    }

    json_strchg(src, "\\", "\\\\", buf, out_sz);
    memcpy(out, buf, out_sz);
    json_strchg(out, "\"", "\\\"", buf, out_sz);
    memcpy(out, buf, out_sz);
    json_strchg(out, "/", "\\/", buf, out_sz);
    memcpy(out, buf, out_sz);

    json_free(buf);
    return out;
}
