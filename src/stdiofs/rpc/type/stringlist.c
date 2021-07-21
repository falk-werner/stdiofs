#include "stdiofs/rpc/type/stringlist.h"
#include "stdiofs/rpc/types.h"
#include "stdiofs/rpc/buffer.h"
#include "stdiofs/rpc/arg.h"
#include "stdiofs/rpc/status.h"

#include <stdlib.h>
#include <string.h>

#define RPC_STRINGLIST_INITIAL_CAPACITY 16

void
rpc_stringlist_init(
    struct rpc_stringlist * list)
{
    list->entries = malloc(sizeof(char*) * RPC_STRINGLIST_INITIAL_CAPACITY);
    list->capacity = RPC_STRINGLIST_INITIAL_CAPACITY;
    list->size = 0;
}

void
rpc_stringlist_cleanup(
    struct rpc_stringlist * list)
{
    for(size_t i = 0; i < list->size; i++)
    {
        free(list->entries[i]);
    }
    free(list->entries);
}

int
rpc_stringlist_add(
    void * buffer,
    char const * name)
{
    struct rpc_stringlist * list = buffer;
    if (list->size >= list->capacity)
    {
        size_t new_capacity = list->capacity * 2;
        list->entries = realloc(list->entries, sizeof(char *) * new_capacity);
        list->capacity = new_capacity;
    }

    list->entries[list->size] = strdup(name);
    list->size++;

    return 0;
}

int
rpc_serialize_stringlist(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    struct rpc_stringlist * list = arg->value;

    rpc_serialize_raw_u32(buffer, (uint32_t) list->size);
    for(size_t i = 0; i < list->size; i++)
    {
        rpc_serialize_raw_string(buffer, list->entries[i]);
    }

    return 0;
}

int
rpc_deserialize_stringlist(
    struct rpc_buffer * buffer,
    struct rpc_arg const * arg)
{
    void * list = arg->value;
    rpc_stringlist_add_fn * add = arg->extra;

    uint32_t count;
    int result = rpc_deserialize_raw_u32(buffer, &count);
    for(uint32_t i = 0; (0 == result) && (i < count); i++)
    {
        char * value;
        result = rpc_deserialize_raw_string(buffer, &value);
        if (0 == result)
        {
            add(list, value);
        }
    }

    return result;
}