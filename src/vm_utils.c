#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "vm.h"
#include "vm_utils.h"
#include "ffi.h"

svm_t *svm_init(size_t code_size, size_t stack_size, size_t cstack_size, size_t baseiptr){
    svm_t *vm = calloc(1, sizeof(svm_t));
    vm->baseiptr = baseiptr;
    vm->code_size = code_size;
    vm->stack_size = stack_size;
    vm->cstack_size = cstack_size;
    vm->code = calloc(code_size, sizeof(svm_stack_item_t));
    vm->stack = calloc(stack_size, sizeof(svm_stack_item_t));
    vm->cstack = calloc(cstack_size, sizeof(svm_context_t));
    return vm;
}

inline void svm_append_opcode(svm_stack_item_t *stack, size_t sptr, enum svm_opcodes opcode){
    stack[sptr].type = svm_opcode;
    stack[sptr].opcode = opcode;
    return;
}

inline void svm_append_integer(svm_stack_item_t *stack, size_t sptr, int value){
    stack[sptr].type = svm_integer;
    stack[sptr].integer = value;
    return;
}

inline void svm_append_float(svm_stack_item_t *stack, size_t sptr, float value){
    stack[sptr].type = svm_float;
    stack[sptr].floating = value;
    return;
}

inline void svm_append_string(svm_stack_item_t *stack, size_t sptr, svm_string_t *value){
    stack[sptr].type = svm_string;
    stack[sptr].string = value;
    return;
}

inline void svm_append_function(svm_stack_item_t *stack, size_t sptr, svm_function_t value){
    stack[sptr].type = svm_function;
    stack[sptr].function = value;
    return;
}

inline void svm_append_ffi(svm_stack_item_t *stack, size_t sptr, svm_ffi_t *value){
    stack[sptr].type = svm_ffi;
    stack[sptr].ffi = value;
    return;
}

inline svm_function_t svm_generate_function(size_t nargs, size_t nlocals, size_t addr){
    svm_function_t function;
    function.nargs = nargs;
    function.nlocals = nlocals;
    function.addr = addr;
    return function;
}

inline svm_string_t *svm_string_cat(svm_string_t *a, svm_string_t *b){
    svm_string_t *dest = calloc(1, sizeof(svm_string_t));
    dest->str = calloc(a->len + b->len, sizeof(char));
    strncpy(dest->str, a->str, a->len);
    strncpy(dest->str + a->len, b->str, b->len);
    dest->len = a->len + b->len;
    return dest;
}

inline svm_string_t *svm_string_from_cstr(char *str){
    svm_string_t *dest = calloc(1, sizeof(svm_string_t));
    dest->len = strlen(str);
    dest->str = calloc(dest->len, sizeof(char));
    strncpy(dest->str, str, dest->len);
    return dest;
}

inline char *svm_char_from_svm_str(svm_string_t *str){
    char *string = calloc(str->len + 1, sizeof(char));
    memcpy(string, str->str, str->len);
    string[str->len] = '\0';
    return string;
}

inline char *svm_get_term_input(FILE *stream){
    size_t len = 512;
    size_t i = 0;
    int c = 0;
    char *buffer = calloc(len, sizeof(char));
    for(c = getc(stream); c != '\n' && c != EOF; c = getc(stream)){
        buffer[i++] = (char)c;
        if(i == len){
            char *tmp = realloc(buffer, len * 2);
            if(tmp){
                buffer = tmp;
                len *= 2;
            }
            else {
                fprintf(stderr, "SimpleVM: %s\n", strerror(errno));
                free(buffer);
                exit(1);
            }
        }
    }
    buffer[i] = '\0';
    return buffer;
}
