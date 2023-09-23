#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "listlib.h"

#define TRAY_INIT_SIZE 8

typedef struct list_struct {
    data_t item;
    struct list_struct *next;
} list_t;

static list_t endlist;
static list_t *headptr = NULL;
static list_t *tailptr = NULL;
static list_t **travptrs = NULL;
static int travptrs_size = 0;


