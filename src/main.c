#include <stdio.h>
#include "vm.h"
#include "vm_utils.h"

int main(int argc, char **argv){
    svm_t *vm = svm_init(100, 100, 100, 0);
    svm_append_opcode(vm->code, 0, PUSH);
    svm_append_float(vm->code, 1, 10.0);
    svm_append_opcode(vm->code, 2, PUSH);
    svm_append_float(vm->code, 3, 10.0);
    svm_append_opcode(vm->code, 4, FADD);
    svm_append_opcode(vm->code, 5, FPRINT);
    svm_append_opcode(vm->code, 6, PUSH);
    svm_append_string(vm->code, 7, svm_string_from_cstr("Hello, World!"));
    svm_append_opcode(vm->code, 8, SPRINT);
    svm_append_opcode(vm->code, 9, CALL);
    svm_append_function(vm->code, 10, svm_generate_function(0, 1, 12));
    svm_append_opcode(vm->code, 11, HALT);
    svm_append_opcode(vm->code, 12, PUSH);
    svm_append_string(vm->code, 13, svm_string_from_cstr("Hello, Function World!"));
    svm_append_opcode(vm->code, 14, SPRINT);
    svm_append_opcode(vm->code, 15, RET);
    svm_exec(vm);
    return 0;
}
