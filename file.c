#include <stdio.h>
#include <stdlib.h>

#include "file.h"


typedef struct file_struct {
    struct file_struct * prev;
    void * data;
    struct file_struct * next;
}
