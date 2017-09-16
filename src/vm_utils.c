#include <stddef.h>
#include <stdlib.h>
#include "vm.h"
#include "vm_utils.h"

svm_t *svm_init(size_t code_size, size_t stack_size, size_t baseiptr){
    svm_t *vm = calloc(1, sizeof(svm_t));
    vm->baseiptr = baseiptr;
    vm->code_size = code_size;
    vm->stack_size = stack_size;
    vm->code = calloc(code_size, sizeof(svm_stack_item_t));
    vm->stack = calloc(stack_size, sizeof(svm_stack_item_t));
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
