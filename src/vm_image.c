#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"
#include "vm_utils.h"
#include "vm_image.h"

typedef struct {
    FILE *fd;
    size_t len;
    char *contents;
} svm_image_t;

size_t svm_write_int(svm_stack_item_t *item, FILE *out){
    size_t count = fwrite(&item->type, sizeof(item->type), 1, out);
    if(!count) return 0;
    count += fwrite(&item->integer, sizeof(item->integer), 1, out);
    return count;
}

size_t svm_write_char(svm_stack_item_t *item, FILE *out){
    size_t count = fwrite(&item->type, sizeof(item->type), 1, out);
    if(!count) return 0;
    count += fwrite(&item->character, sizeof(item->character), 1, out);
    return count;
}

size_t svm_write_str(svm_stack_item_t *item, FILE *out){
    size_t count = fwrite(&item->type, sizeof(item->type), 1, out);
    if(!count) return 0;
    size_t count2 = fwrite(&item->string->len, sizeof(item->string->len), 1, out);
    if(!count2) return 0;
    count += fwrite(&item->string->str, sizeof(char), item->string->len, out);
    return count + count2;
}

size_t svm_write_bool(svm_stack_item_t *item, FILE *out){
    size_t count = fwrite(&item->type, sizeof(item->type), 1, out);
    if(!count) return 0;
    count += fwrite(&item->boolean, sizeof(item->boolean), 1, out);
    return count;
}

size_t svm_write_float(svm_stack_item_t *item, FILE *out){
    size_t count = fwrite(&item->type, sizeof(item->type), 1, out);
    if(!count) return 0;
    count += fwrite(&item->floating, sizeof(item->floating), 1, out);
    return count;
}

size_t svm_write_opcode(svm_stack_item_t *item, FILE *out){
    size_t count = fwrite(&item->type, sizeof(item->type), 1, out);
    if(!count) return 0;
    count += fwrite(&item->opcode, sizeof(item->opcode), 1, out);
    return count;
}

size_t svm_write_function(svm_stack_item_t *item, FILE *out){
    size_t count = fwrite(&item->type, sizeof(item->type), 1, out);
    if(!count) return 0;
    count += fwrite(&item->function, sizeof(item->function), 1, out);
    return 0;
}

size_t svm_write_stack_item(svm_stack_item_t *item, FILE *out){
    switch(item->type){
        case svm_integer:
            return svm_write_int(item, out);
            break;
        case svm_character:
            return svm_write_char(item, out);
            break;
        case svm_string:
            return svm_write_str(item, out);
            break;
        case svm_boolean:
            return svm_write_bool(item, out);
            break;
        case svm_float:
            return svm_write_float(item, out);
            break;
        case svm_opcode:
            return svm_write_opcode(item, out);
            break;
        case svm_function:
            return svm_write_function(item, out);
            break;
        default:
            fprintf(stderr, "SimpleVM: invalid type: %i\n", item->type);
            break;
    }
    return 0;
}

size_t svm_save(svm_t *vm, char *path){
    FILE *out = fopen(path, "wb");
    fwrite(&vm->baseiptr, sizeof(vm->baseiptr), 1, out);
    fwrite(&vm->code_size, sizeof(vm->code_size), 1, out);
    fwrite(&vm->stack_size, sizeof(vm->stack_size), 1, out);

    size_t total = 0;
    for(size_t i = 0; i < vm->code_size; i++){
        size_t tmp = svm_write_stack_item(&vm->code[i], out);
        if(!tmp){
            break;
        }
        total += tmp;
    }
    fclose(out);
    return total;
}

enum svm_types svm_read_type(FILE *in){
    enum svm_types type;
    fread(&type, sizeof(type), 1, in);
    return type;
}

svm_stack_item_t *svm_read_int(FILE *in){
    svm_stack_item_t *item = calloc(1, sizeof(svm_stack_item_t));
    item->type = svm_integer;
    fread(&item->integer, sizeof(item->integer), 1, in);
    return item;
}

svm_stack_item_t *svm_read_char(FILE *in){
    svm_stack_item_t *item = calloc(1, sizeof(svm_stack_item_t));
    item->type = svm_character;
    fread(&item->character, sizeof(item->character), 1, in);
    return item;
}

svm_stack_item_t *svm_read_str(FILE *in){
    svm_stack_item_t *item = calloc(1, sizeof(svm_stack_item_t));
    item->type = svm_string;
    item->string = calloc(1, sizeof(svm_string_t));
    fread(&item->string->len, sizeof(item->string->len), 1, in);
    item->string->str = calloc(item->string->len, sizeof(char));
    fread(item->string->str, sizeof(char), item->string->len, in);
    return item;
}

svm_stack_item_t *svm_read_bool(FILE *in){
    svm_stack_item_t *item = calloc(1, sizeof(svm_stack_item_t));
    item->type = svm_boolean;
    fread(&item->boolean, sizeof(item->boolean), 1, in);
    return item;
}

svm_stack_item_t *svm_read_float(FILE *in){
    svm_stack_item_t *item = calloc(1, sizeof(svm_stack_item_t));
    item->type = svm_float;
    fread(&item->floating, sizeof(item->floating), 1, in);
    return item;
}

svm_stack_item_t *svm_read_opcode(FILE *in){
    svm_stack_item_t *item = calloc(1, sizeof(svm_stack_item_t));
    item->type = svm_opcode;
    fread(&item->floating, sizeof(item->floating), 1, in);
    return item;
}

svm_stack_item_t *svm_read_function(FILE *in){
    svm_stack_item_t *item = calloc(1, sizeof(svm_stack_item_t));
    item->type = svm_function;
    fread(&item->function, sizeof(item->function), 1, in);
    return item;
}

svm_stack_item_t *svm_read_stack_item(FILE *in){
    enum svm_types type = svm_read_type(in);
    switch(type){
        case svm_integer:
            return svm_read_int(in);
            break;
        case svm_character:
            return svm_read_char(in);
            break;
        case svm_string:
            return svm_read_str(in);
            break;
        case svm_boolean:
            return svm_read_bool(in);
            break;
        case svm_float:
            return svm_read_float(in);
            break;
        case svm_opcode:
            return svm_read_opcode(in);
            break;
        case svm_function:
            return svm_read_function(in);
            break;
        default:
            fprintf(stderr, "SimpleVM: invalid type: %i\n", in);
            break;
    }
    return NULL;
}

svm_t *svm_read(svm_t *vm, char *path){
    FILE *in = fopen(path, "rb");
    fread(&vm->baseiptr, sizeof(vm->baseiptr), 1, in);
    fread(&vm->code_size, sizeof(vm->code_size), 1, in);
    fread(&vm->stack_size, sizeof(vm->stack_size), 1, in);

    vm->stack = calloc(vm->stack_size, sizeof(svm_stack_item_t));
    vm->code = calloc(vm->code_size, sizeof(svm_stack_item_t));

    for(size_t i = 0; i < vm->code_size; i++){
        svm_stack_item_t *item = svm_read_stack_item(in);
        if(!item){
            printf("%p", i);
            return NULL;
        }
        memcpy(vm->code + i, item, sizeof(svm_stack_item_t));
        free(item);
    }

    fclose(in);

    return vm;
}