#include <stdio.h>
#include "../vm.h"
#include "../vm_utils.h"
#include "../vm_image.h"

int main(int argc, char **argv){
    svm_t *vm = svm_init(17, 100, 100, 0);
    size_t sptr = 0;

    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_bool(vm->code, sptr++, true);
    svm_append_opcode(vm->code, sptr++, BPRINT);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_integer(vm->code, sptr++, 10);
    svm_append_opcode(vm->code, sptr++, IPRINT);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_float(vm->code, sptr++, 10.345);
    svm_append_opcode(vm->code, sptr++, FPRINT);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_char(vm->code, sptr++, 'd');
    svm_append_opcode(vm->code, sptr++, CPRINT);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_string(vm->code, sptr++, svm_string_from_cstr("Hello, Image World!"));
    svm_append_opcode(vm->code, sptr++, SPRINT);
    svm_append_opcode(vm->code, sptr++, SIN);
    svm_append_opcode(vm->code, sptr++, SPRINT);
    svm_append_opcode(vm->code, sptr++, HALT);

    svm_exec(vm);
    svm_save(vm, "./svm-prints.svmi");

    return 0;
}