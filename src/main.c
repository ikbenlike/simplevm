#include <stdio.h>
#include "vm.h"
#include "vm_utils.h"

int main(int argc, char **argv){
    svm_t *vm = svm_init(100, 100, 0);
    svm_append_opcode(vm->code, 0, PUSH);
    svm_append_float(vm->code, 1, 10.0);
    svm_append_opcode(vm->code, 2, PUSH);
    svm_append_float(vm->code, 3, 10.0);
    svm_append_opcode(vm->code, 4, FADD);
    svm_append_opcode(vm->code, 5, FPRINT);
    svm_append_opcode(vm->code, 6, HALT);
    svm_exec(vm);
    return 0;
}
