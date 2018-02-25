#include <stdio.h>
#include "vm.h"
#include "vm_utils.h"
#include "vm_image.h"

int main(int argc, char **argv){
    /*svm_t *vm = svm_init(100, 100, 100, 0);
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
    //svm_append_opcode(vm->code, sptr++, SPRINT);*/
    /*svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_string(vm->code, sptr++, svm_string_from_cstr("Hello, Function World!"));
    svm_append_opcode(vm->code, sptr++, SPRINT);
    svm_append_opcode(vm->code, sptr++, RET);

    svm_append_opcode(vm->code, sptr++, LOAD);
    svm_append_integer(vm->code, sptr++, 0);
    svm_append_opcode(vm->code, sptr++, LOAD);
    svm_append_integer(vm->code, sptr++, 1);
    svm_append_opcode(vm->code, sptr++, IMUL);
    svm_append_opcode(vm->code, sptr++, RET);

    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_float(vm->code, sptr++, 10.0);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_float(vm->code, sptr++, 10.0);
    svm_append_opcode(vm->code, sptr++, FADD);
    svm_append_opcode(vm->code, sptr++, FPRINT);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_string(vm->code, sptr++, svm_string_from_cstr("Hello, World!"));
    svm_append_opcode(vm->code, sptr++, SPRINT);
    svm_append_opcode(vm->code, sptr++, CALL);
    svm_append_function(vm->code, sptr++, svm_generate_function(0, 1, 0));
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_integer(vm->code, sptr++, 4);
    svm_append_opcode(vm->code, sptr++, PUSH);
    svm_append_integer(vm->code, sptr++, 5);
    svm_append_opcode(vm->code, sptr++, CALL);
    svm_append_function(vm->code, sptr++, svm_generate_function(2, 0, 4));
    svm_append_opcode(vm->code, sptr++, IPRINT);
    svm_append_opcode(vm->code, sptr++, SIN);
    svm_append_opcode(vm->code, sptr++, SPRINT);
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

    svm_save(vm, "./svm-save-test");

    svm_t *vm2 = svm_init(100, 100, 100, 0);
    svm_read(vm2, "./svm-save-test");
    svm_exec(vm2);*/

    if(argc == 2){
        svm_t *vm = svm_read(argv[1]);
        svm_exec(vm);
    }
    else {
        printf("SimpleVM: please provide a file to execute");
    }

    return 0;
}
