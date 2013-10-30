#ifndef _STATIC_HASH_H
#define _STATIC_HASH_H

#define __USE_GNU
#include <search.h>
#include <stdarg.h>

typedef struct dnsurl_s {
    char          urlstr[64];
    uisigned int  ip[4]; 
    char          flag;    /*0:自定义解析  1：禁止访问*/
}dnsurl;

#define hash_entry ENTRY
typedef struct hsearch_data hash_table;

hash_table *hash_create(int num);
void hash_destory(hash_table *tbl);

hash_entry *hash_find(hash_table *tbl, char *key);
void *hash_find_value(hash_table *tbl, char *key);
void *hash_find_value_r(hash_table *tbl, char *key, int len);
void *hash_find_value_space(hash_table *tbl, char *key);

hash_entry *hash_insert(hash_table *tbl, char *key, void *value);
int hash_insert_pairs(hash_table *tbl, int num, ...);

#endif
