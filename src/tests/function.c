#include <stdio.h>
#include "../vm.h"
#include "../vm_utils.h"
#include "../vm_image.h"

int main(int argc, char **argv){
    svm_t *vm = svm_init(19, 100, 100, 10);
    size_t sptr = 0;
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_string(vm->code, sptr++, svm_string_from_cstr("Hello, Function World!"));
    svm_append_opcode(vm->code, sptr++, SPRINT);
    svm_append_opcode(vm->code, sptr++, RET);

    svm_append_opcode(vm->code, sptr++, LOAD);
    svm_append_integer(vm->code, sptr++, 0);
    svm_append_opcode(vm->code, sptr++, LOAD);
    svm_append_integer(vm->code, sptr++, 1);
    svm_append_opcode(vm->code, sptr++, IMUL);
    svm_append_opcode(vm->code, sptr++, RET);

    svm_append_opcode(vm->code, sptr++, CALL);
    svm_append_function(vm->code, sptr++, svm_generate_function(0, 1, 0));
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_integer(vm->code, sptr++, 4);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_integer(vm->code, sptr++, 5);
    svm_append_opcode(vm->code, sptr++, CALL);
    svm_append_function(vm->code, sptr++, svm_generate_function(2, 0, 4));
    svm_append_opcode(vm->code, sptr++, IPRINT);
    svm_append_opcode(vm->code, sptr++, HALT);

    svm_exec(vm);
    svm_save(vm, "./svm-functions.svmi");

    return 0;
}