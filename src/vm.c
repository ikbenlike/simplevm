#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stddef.h>
#include "vm.h"
#include "vm_utils.h"
#include "ffi.h"

svm_stack_item_t *svm_exec(svm_t *vm){
    ssize_t s_iptr = vm->baseiptr;
    ssize_t s_sptr = 0;
    ssize_t s_csptr = 0;

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
            case SADD: {
                svm_string_t *b = vm->stack[s_sptr--].string;
                svm_string_t *a = vm->stack[s_sptr--].string;
                svm_string_t *tmp = svm_string_cat(a, b);
                free(vm->stack[++s_sptr].string->str);
                free(vm->stack[s_sptr].string);
                vm->stack[s_sptr].string = tmp;
                break;
            }
            case SPRINT: {
                svm_string_t *a = vm->stack[s_sptr--].string;
                for(size_t i = 0; i < a->len; putchar(a->str[i++]));
                putchar('\n');
                break;
            }
            case BPRINT: {
                bool a = vm->stack[s_sptr--].boolean;
                puts(a ? "true" : "false");
                break;
            }
            case SIN: {
                char *tmp = svm_get_term_input(stdin);
                vm->stack[++s_sptr].string = svm_string_from_cstr(tmp);
                free(tmp);
                break;
            }
            case CADD: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a + b;
                break;
            }
            case CSUB: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a - b;
                break;
            }
            case CDIV: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a / b;
                break;
            }
            case CMUL: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a * b;
                break;
            }
            case CMOD: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a % b;
                break;
            }
            case IEQ: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].boolean = a == b;
                break;
            }
            case INEQ: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].boolean = a != b;
                break;
            }
            case FEQ: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].boolean = a == b;
                break;
            }
            case FNEQ: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].boolean = a != b;
                break;
            }
            case SEQ: {
                svm_string_t *b = vm->stack[s_sptr--].string;
                svm_string_t *a = vm->stack[s_sptr--].string;
                if(a->len <= b->len){
                    vm->stack[++s_sptr].boolean = strncmp(a->str, b->str, b->len) == 0;
                }
                else {
                    vm->stack[++s_sptr].boolean = strncmp(a->str, b->str, a->len) == 0;
                }
                break;
            }
            case SNEQ: {
                svm_string_t *b = vm->stack[s_sptr--].string;
                svm_string_t *a = vm->stack[s_sptr--].string;
                if(a->len <= b->len){
                    vm->stack[++s_sptr].boolean = strncmp(a->str, b->str, b->len) != 0;
                }
                else {
                    vm->stack[++s_sptr].boolean = strncmp(a->str, b->str, a->len) != 0;
                }
                break;
            }
            case CEQ: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].boolean = a == b;
                break;
            }
            case CNEQ: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].boolean = a != b;
                break;
            }
            case CALL: {
                ssize_t addr = vm->code[s_iptr].function.addr;
                s_csptr++;
                vm->cstack[s_csptr].ret = s_iptr;
                vm->cstack[s_csptr].locals = calloc(vm->code[s_iptr].function.nlocals + vm->code[s_iptr].function.nargs, sizeof(svm_stack_item_t));
                for(size_t i = 0; i < vm->code[s_iptr].function.nargs; i++){
                    vm->cstack[s_csptr].locals[i] = vm->stack[s_sptr - i];
                }
                s_sptr -= vm->code[s_iptr].function.nargs;
                s_iptr = addr;
                break;
            }
            case FFI: {
                puts("FFI opcode");
                svm_ffi_t *n_ffi = vm->code[s_iptr].ffi;
                svm_stack_item_t *n_stack = calloc(n_ffi->nargs, sizeof(svm_stack_item_t));
                for(size_t i = 0; i < n_ffi->nargs; i++){
                    printf("%zu\n", i);
                    n_stack[i] = vm->stack[s_sptr - i];
                }
                svm_ffi_call(n_ffi, n_stack);
                s_iptr++;
                break;
            }
            case RET: {
                s_iptr = vm->cstack[s_csptr].ret + 1;
                s_csptr--;
                break;
            }
            case LOAD: {
                ssize_t offset = vm->code[s_iptr++].integer;
                vm->stack[++s_sptr] = vm->cstack[s_csptr].locals[offset];
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
