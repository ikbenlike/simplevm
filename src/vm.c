#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stddef.h>
#include "vm.h"
#include "vm_utils.h"

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
                vm->stack[s_sptr].type = svm_integer;
                break;
            }
            case ISUB: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].integer = a - b;
                vm->stack[s_sptr].type = svm_integer;
                break;
            }
            case IDIV: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].integer = a / b;
                vm->stack[s_sptr].type = svm_integer;
                break;
            }
            case IMUL: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].integer = a * b;
                vm->stack[s_sptr].type = svm_integer;
                break;
            }
            case IMOD: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].integer = a % b;
                vm->stack[s_sptr].type = svm_integer;
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
                vm->stack[s_sptr].type = svm_float;
                break;
            }
            case FSUB: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].floating = a - b;
                vm->stack[s_sptr].type = svm_float;
                break;
            }
            case FDIV: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].floating = a / b;
                vm->stack[s_sptr].type = svm_float;
                break;
            }
            case FMUL: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].floating = a * b;
                vm->stack[s_sptr].type = svm_float;
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
                vm->stack[s_sptr].type = svm_string;
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
                vm->stack[s_sptr].type = svm_string;
                break;
            }
            case CADD: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a + b;
                vm->stack[s_sptr].type = svm_character;
                break;
            }
            case CSUB: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a - b;
                vm->stack[s_sptr].type = svm_character;
                break;
            }
            case CDIV: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a / b;
                vm->stack[s_sptr].type = svm_character;
                break;
            }
            case CMUL: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a * b;
                vm->stack[s_sptr].type = svm_character;
                break;
            }
            case CMOD: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].character = a % b;
                vm->stack[s_sptr].type = svm_character;
                break;
            }
            case IEQ: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].boolean = a == b;
                vm->stack[s_sptr].type = svm_boolean;
                break;
            }
            case INEQ: {
                int b = vm->stack[s_sptr--].integer;
                int a = vm->stack[s_sptr--].integer;
                vm->stack[++s_sptr].boolean = a != b;
                vm->stack[s_sptr].type = svm_boolean;
                break;
            }
            case FEQ: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].boolean = a == b;
                vm->stack[s_sptr].type = svm_boolean;
                break;
            }
            case FNEQ: {
                float b = vm->stack[s_sptr--].floating;
                float a = vm->stack[s_sptr--].floating;
                vm->stack[++s_sptr].boolean = a != b;
                vm->stack[s_sptr].type = svm_boolean;
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
                vm->stack[s_sptr].type = svm_boolean;
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
                vm->stack[s_sptr].type = svm_boolean;
                break;
            }
            case CEQ: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].boolean = a == b;
                vm->stack[s_sptr].type = svm_boolean;
                break;
            }
            case CNEQ: {
                char b = vm->stack[s_sptr--].character;
                char a = vm->stack[s_sptr--].character;
                vm->stack[++s_sptr].boolean = a != b;
                vm->stack[s_sptr].type = svm_boolean;
                break;
            }
            case CMPB: {
                svm_stack_item_t *b = &vm->stack[s_sptr--];
                svm_stack_item_t *a = &vm->stack[s_sptr--];
                bool equality = memcmp(a, b, sizeof(*a));
                vm->stack[++s_sptr].boolean = equality;
                vm->stack[s_sptr].type = svm_boolean;
                break;
            }
            case CMPD: {
                svm_stack_item_t *b = &vm->stack[s_sptr--];
                svm_stack_item_t *a = &vm->stack[s_sptr--];
                if(a->type != b->type){
                    vm->stack[++s_sptr].boolean = false;
                }
                else {
                    switch(a->type){
                        case svm_integer:
                            vm->stack[++s_sptr].boolean = a->integer == b->integer;
                            break;
                        case svm_boolean:
                            vm->stack[++s_sptr].boolean = a->boolean == b->boolean;
                            break;
                        case svm_character:
                            vm->stack[++s_sptr].boolean = a->character == b->character;
                            break;
                        case svm_string:
                            if(a->string->len < b->string->len){
                                vm->stack[++s_sptr].boolean = !strncmp(a->string->str, b->string->str, a->string->len);
                            }
                            else {
                                vm->stack[++s_sptr].boolean = !strncmp(a->string->str, b->string->str, b->string->len);
                            }
                            break;
                        case svm_float:
                            vm->stack[++s_sptr].boolean = a->floating == b->floating;
                            break;
                        default:
                            vm->stack[++s_sptr].boolean = false;
                    }
                }
                vm->stack[s_sptr].type = svm_boolean;
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
            case RET: {
                s_iptr = vm->cstack[s_csptr].ret + 1;
                s_csptr--;
                break;
            }
            case RJMP: {
                int val = vm->stack[s_sptr--].integer;
                s_iptr += (ssize_t)val;
                break;
            }
            case RJMPIT: {
                int b = vm->stack[s_sptr--].integer;
                bool a = vm->stack[s_sptr--].boolean;
                if(a){
                    s_iptr += (ssize_t)b;
                }
                break;
            }
            case RJMPIF: {
                int b = vm->stack[s_sptr--].integer;
                bool a = vm->stack[s_sptr--].boolean;
                if(!a){
                    s_iptr += (ssize_t)b;
                }
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
