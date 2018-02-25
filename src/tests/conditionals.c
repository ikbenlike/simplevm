#include <stdio.h>
#include "../vm.h"
#include "../vm_utils.h"
#include "../vm_image.h"

int main(int argc, char **argv){
    svm_t *vm = svm_init(15, 100, 100, 0);
    size_t sptr = 0;

    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_integer(vm->code, sptr++, 10);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_integer(vm->code, sptr++, 10);
    svm_append_opcode(vm->code, sptr++, CMPD);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_integer(vm->code, sptr, 3);
    sptr++;
    svm_append_opcode(vm->code, sptr++, RJMPIF);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_string(vm->code, sptr++, svm_string_from_cstr("Nope!"));
    svm_append_opcode(vm->code, sptr++, SPRINT);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_string(vm->code, sptr++, svm_string_from_cstr("Yeah!"));
    svm_append_opcode(vm->code, sptr++, SPRINT);
    svm_append_opcode(vm->code, sptr++, HALT);

    svm_exec(vm);
    svm_save(vm, "./conditionals.svmi");

    return 0;
}