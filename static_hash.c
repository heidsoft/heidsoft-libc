/*      ___    __    _  __          ____
 *     /   |  / /__ | |/ /_______  / __ \
 *    / /| | / / _ \|   /_  /_  / / /_/ /
 *   / ___ |/ /  __/   | / /_/ /_/ ____/
 *  /_/  |_/_/\___/_/|_|/___/___/_/
 *
 *  Copyright(c), All rights reserved.
 *  Author:          alexzzp@126.com
 *  Filename:        static_hash.c
 *  Created Time:    2013/07/11  10:41:11
 *  Description:     静态hash表，其表项的大小在建立的时候进行确定
 *                   实际上是glibc的hash的wrapper
 *                   一般用来当做符号表，不支持删除操作
 *  Revision:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "static_hash.h"

/* ----------------------
static hash api ---------------------- 
实例
hash_table *tbl;
hash_entry *new_entry;
tbl = hash_create(maxnum);
new_entry=hash_insert(tbl, "key", data* ); 
//注意data一定要是全局内存空间指针
data = hash_find_value(tbl, "key");
if (data == NULL) {
   DPRINTF(1, "Do not find %s in dict", string);
   return -1;
}
hash_destory(tbl);
*/

/* ---------------------- static hash api ---------------------- */

/*
 * @Desc:   建立一个静态hash表，其最大的容量为num
 * @Return: 若成功则返回新建立的hash表，否则为NULL
 */
hash_table *hash_create(int num)
{
    hash_table *tbl = NULL;
    int flag;

    tbl = malloc(sizeof(hash_table));
    if (tbl < 0)
        return NULL;
    memset(tbl, 0, sizeof(*tbl));
    flag = hcreate_r(num, tbl);
    if (flag == 0) {
        xfree(tbl);
        return NULL;
    }
    return tbl;
}

/*
 * @Desc:   析构一个哈希表
 * @Return: void
 * @Note:   其只是释放内部表项，而每个表项所对应的key以及value并不会被释放
 */
void hash_destory(hash_table *tbl)
{
    hdestroy_r(tbl);
}

/*
 * @Desc:  通过key来搜索对应的表项
 * @Return: 若成功则返回对应hash表中的表项，否则返回NULL
 */
hash_entry *hash_find(hash_table *tbl, char *key)
{
    hash_entry entry;
    hash_entry *find_entry;
    int flag;

    entry.key = key;
    flag = hsearch_r(entry, FIND, &find_entry, tbl);
    if (flag == 0)
        return NULL;

    return find_entry;
}

/*
 * @Desc:   hash_table()的wrapper，直接返回value
 * @Return: 若成功则返回value,否则返回0
 */
void *hash_find_value(hash_table *tbl, char *key)
{
    hash_entry *entry;
    entry = hash_find(tbl, key);
    if (entry != NULL)
        return entry->data;
    return NULL;
}

/*
 * @Desc:   使用长度有限的key进行查找，用户需要确保key的大小应该是
 *          比len要长
 * @Return: 若成功则返回0，否则 < 0
 */
void *hash_find_value_r(hash_table *tbl, char *key, int len)
{
    void *value;
    char *key_r = strdup(key);
    if (!key_r)
        return NULL;

    key_r[len] = 0;
    value = hash_find_value(tbl, key_r);
    xfree(key_r);
    return value;
}

/*
 * @Desc:   去掉key后面的空格，进行搜索，key应该以非空格开始
 * @Return: 若成功则返回0，否则 < 0
 */
void *hash_find_value_space(hash_table *tbl, char *key)
{
    int len = 0;
    char *p = key;

    while ( *p && *p != ' ' ) {
        p++;
        len++;
    }
    return hash_find_value_r(tbl, key, len);
}

/*
 * @Desc:   将key作为键值，将value插入哈希表中
 * @Return: 若成功则新插入的表项，否则返回NULL
 */
hash_entry *hash_insert(hash_table *tbl, char *key, void *value)
{
    hash_entry entry;
    hash_entry *find_entry;
    int flag;

    entry.key = key;
    entry.data = value;
    flag = hsearch_r(entry, ENTER, &find_entry, tbl);
    if (flag == 0)
        return NULL;

    return find_entry;
}

/*
 * @Desc:   在tbl中插入多对键值对，用户需要指定插入的数量
 * @Return: 返回成功插入到hash表中的键值对的个数
 */
int hash_insert_pairs(hash_table *tbl, int num, ...)
{
    hash_entry *entry;
    int i;
    va_list ap;
    int success = 0;

    va_start(ap, num);
    for (i = 0; i < num; i++) {
        char *key;
        void *value;

        key = va_arg(ap, char *);
        value = va_arg(ap, void *);

        entry = hash_insert(tbl, key, value);
        if (entry == NULL)
           continue;
        success++;
    }
    va_end(ap);
    return success;
}

