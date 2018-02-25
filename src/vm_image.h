#ifndef __VM_IMAGE_H__
    #define __VM_IMAGE_H__
    #include <stdio.h>
    #include "vm.h"

    size_t svm_write_stack_item(svm_stack_item_t *item, FILE *out);
    svm_stack_item_t *svm_read_stack_item(FILE *in);

    size_t svm_save(svm_t *vm, char *path);
    svm_t *svm_read(char *path);
#endif