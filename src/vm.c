#include <stdio.h>
#include <sys/types.h>
#include <stddef.h>
#include "vm.h"

svm_stack_item_t *svm_exec(svm_t *vm){
    ssize_t s_iptr = vm->baseiptr;
    ssize_t s_sptr = 0;

    int opcode = vm->code[s_iptr].opcode;

    while(s_iptr < vm->code_size){
        s_iptr++;
        switch(opcode){
            case NOP:
                break;
            case PUSH:
                vm->stack[++s_sptr] = vm->code[s_iptr++];
                break;
            case IADD: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].integer = a + b;
                break;
            }
            case ISUB: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].integer = a - b;
                break;
            }
            case IDIV: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].integer = a / b;
                break;
            }
            case IMUL: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].integer = a * b;
                break;
            }
            case IMOD: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].integer = a % b;
                break;
            }
            case IPRINT: {
                printf("%d\n", vm->stack[s_sptr--].integer);
                break;
            }
            case FADD: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].floating = a + b;
                break;
            }
            case FSUB: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].floating = a - b;
                break;
            }
            case FDIV: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].floating = a / b;
                break;
            }
            case FMUL: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].floating = a * b;
                break;
            }
            case FMOD:
                break;
            case FPRINT: {
                printf("%f\n", vm->stack[s_sptr--].floating);
                break;
            }
            case HALT:
                return NULL;
            default:
                break;
        }
        if(vm->code[s_iptr].type == svm_opcode){
            opcode = vm->code[s_iptr].opcode;
        }
        else {
            fprintf(stderr, "SimpleVM: invalid opcode at instruction pointer %zu\n", s_iptr);
            return NULL;
        }
    }
    return &vm->code[s_iptr];
}
