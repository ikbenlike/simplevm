#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include "vm.h"
#include "vm_utils.h"
#include "ffi.h"

#define FFI_NATIVE_PUSH(ITEM) asm volatile ( "push rax;" : : "a" (ITEM) );
#define FFI_NATIVE_RDI(ITEM) asm volatile ( "mov rdi, rax;" : : "a" (ITEM));
#define FFI_NATIVE_RSI(ITEM) asm volatile ( "mov rsi, rax;" : : "a" (ITEM));
#define FFI_NATIVE_RDX(ITEM) asm volatile ( "mov rdx, rax;" : : "a" (ITEM));
#define FFI_NATIVE_RCX(ITEM) asm volatile ( "mov rcx, rax;" : : "a" (ITEM));
#define FFI_NATIVE_R8(ITEM) asm volatile ( "mov r8, rax;" : : "a" (ITEM));
#define FFI_NATIVE_R9(ITEM) asm volatile ( "mov r9, rax;" : : "a" (ITEM));

#define FFI_NATIVE_XMM0(ITEM) asm volatile ( "movaps xmm0, %0" : : "r" (ITEM));
#define FFI_NATIVE_XMM1(ITEM) asm volatile ( "movaps xmm1, %0" : : "r" (ITEM));
#define FFI_NATIVE_XMM2(ITEM) asm volatile ( "movaps xmm2, %0" : : "r" (ITEM));
#define FFI_NATIVE_XMM3(ITEM) asm volatile ( "movaps xmm3, %0" : : "r" (ITEM));
#define FFI_NATIVE_XMM4(ITEM) asm volatile ( "movaps xmm4, %0" : : "r" (ITEM));
#define FFI_NATIVE_XMM5(ITEM) asm volatile ( "movaps xmm5, %0" : : "r" (ITEM));
#define FFI_NATIVE_XMM6(ITEM) asm volatile ( "movaps xmm6, %0" : : "r" (ITEM));
#define FFI_NATIVE_XMM7(ITEM) asm volatile ( "movaps xmm7, %0" : : "r" (ITEM));


svm_ffi_t *svm_load_lib(char *path){
    svm_ffi_t *ffi = calloc(1, sizeof(svm_ffi_t));
    ffi->handle = dlopen(path, RTLD_NOW);
    if(ffi->handle){
        return ffi;
    }
    fprintf(stderr, "SimpleVM: FFI: %s\n", dlerror());
    return NULL;
}

svm_ffi_t *svm_get_function(svm_ffi_t *ffi){
    ffi->func = dlsym(ffi->handle, ffi->f_name);
    if(ffi->func){
        return ffi;
    }
    fprintf(stderr, "SimpleVM: FFI: %s\n", dlerror());
    return NULL;
}

svm_ffi_t *svm_init_ffi(char *path, char *name, size_t nargs){
    svm_ffi_t *ffi = svm_load_lib(path);
    ffi->f_name = name;
    ffi = svm_get_function(ffi);
    ffi->nargs = nargs;
    return ffi;
}

/*void svm_ffi_call(svm_ffi_t *ffi, svm_stack_item_t *args){
    char **strarray = calloc(ffi->nargs, sizeof(char*));
    size_t strarrayi = 0;
    size_t end = 0;
    svm_ffi_t *i_ffi = ffi;
    for(end = 0; end < 6 && end < i_ffi->nargs; end++){
        switch(args[end].type){
            case svm_integer: {
                if(end == 0){
                    FFI_NATIVE_RDI(args[end].integer);
                }
                else if(end == 1){
                    FFI_NATIVE_RSI(args[end].integer);
                }
                else if(end == 2){
                    FFI_NATIVE_RDX(args[end].integer);
                }
                else if(end == 3){
                    FFI_NATIVE_RCX(args[end].integer);
                }
                else if(end == 4){
                    FFI_NATIVE_R8(args[end].integer);
                }
                else if(end == 5){
                    FFI_NATIVE_R9(args[end].integer);
                }
                break;
            }
            case svm_boolean: {
                if(end == 0){
                    FFI_NATIVE_RDI(args[end].boolean);
                }
                else if(end == 1){
                    FFI_NATIVE_RSI(args[end].boolean);
                }
                else if(end == 2){
                    FFI_NATIVE_RDX(args[end].boolean);
                }
                else if(end == 3){
                    FFI_NATIVE_RCX(args[end].boolean);
                }
                else if(end == 4){
                    FFI_NATIVE_R8(args[end].boolean);
                }
                else if(end == 5){
                    FFI_NATIVE_R9(args[end].boolean);
                }
                break;
            }
            case svm_character: {
                if(end == 0){
                    FFI_NATIVE_RDI(args[end].character);
                }
                else if(end == 1){
                    FFI_NATIVE_RSI(args[end].character);
                }
                else if(end == 2){
                    FFI_NATIVE_RDX(args[end].character);
                }
                else if(end == 3){
                    FFI_NATIVE_RCX(args[end].character);
                }
                else if(end == 4){
                    FFI_NATIVE_R8(args[end].character);
                }
                else if(end == 5){
                    FFI_NATIVE_R9(args[end].character);
                }
                break;
            }
            case svm_string: {
                if(end == 0){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    FFI_NATIVE_RDI(strarray[strarrayi - 1]);
                }
                else if(end == 1){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    FFI_NATIVE_RSI(strarray[strarrayi - 1]);
                }
                else if(end == 2){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    FFI_NATIVE_RDX(strarray[strarrayi - 1]);
                }
                else if(end == 3){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    FFI_NATIVE_RCX(strarray[strarrayi - 1]);
                }
                else if(end == 4){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    FFI_NATIVE_R8(strarray[strarrayi - 1]);
                }
                else if(end == 5){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    FFI_NATIVE_R9(strarray[strarrayi - 1]);
                }
                break;
            }
            case svm_float: {
                break;
            }
            default: 
                puts("type not supported yet");
                break;
        }
    }
    for(size_t i = end; i < ffi->nargs; i++){
        switch(args[i].type){
            case svm_float: {
                if(i == 0){
                    FFI_NATIVE_XMM0(args[i].floating);
                }
                else if(i == 1){
                    FFI_NATIVE_XMM1(args[i].floating);
                }
                else if(i == 2){
                    FFI_NATIVE_XMM2(args[i].floating);
                }
                else if(i == 3){
                    FFI_NATIVE_XMM3(args[i].floating);
                }
                else if(i == 4){
                    FFI_NATIVE_XMM4(args[i].floating);
                }
                else if(i == 5){
                    FFI_NATIVE_XMM5(args[i].floating);
                }
                else if(i == 6){
                    //FFI_NATIVE_XMM6(args[i].floating);
                }
                else if(i == 7){
                    //FFI_NATIVE_XMM7(args[i].floating);
                }
            }
            default: {
                break;
            }
        }
    }
    for(size_t i = 0; i_ffi->nargs - i > end; i++){
        switch(args[ffi->nargs - i].type){
            case svm_integer: {
                FFI_NATIVE_PUSH(args[ffi->nargs - i].integer);
                break;
            }
            case svm_float: {
                FFI_NATIVE_PUSH(args[ffi->nargs - i].floating);
            }
            case svm_boolean: {
                FFI_NATIVE_PUSH(args[ffi->nargs - i].floating);
            }
            case svm_character: {
                FFI_NATIVE_PUSH(args[ffi->nargs - i].floating);
            }
            case svm_string: {
                strarray[strarrayi++] = svm_char_from_svm_str(args[ffi->nargs - i].string);
                FFI_NATIVE_PUSH(strarray[strarrayi - 1]);
            }
            default: 
                fprintf(stderr, "SimpleVM: FFI: invalid type on line #%d\n", __LINE__);
        }
    }
    asm volatile (
        "call %0;"
        :
        : "r" (i_ffi->func)
    );
}*/


void svm_ffi_call(svm_ffi_t *ffi, svm_stack_item_t *args){

    enum svm_ffi_type {
        svm_ffi_int,
        svm_ffi_bool,
        svm_ffi_char,
        svm_ffi_str,
        svm_ffi_float,
    };
    typedef struct {
        bool set;
        void *data;
        enum svm_ffi_type type;
    } svm_ffi_reg_t;

    char **strarray = calloc(ffi->nargs, sizeof(char*));
    size_t strarrayi = 0;
    size_t end = 0;
    size_t end_f = 0;
    svm_ffi_reg_t rdi = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t rsi = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t rdx = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t rcx = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t r8 = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t r9 = {.data = NULL, .type = 0, .set = false};

    svm_ffi_reg_t xmm0 = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t xmm1 = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t xmm2 = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t xmm3 = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t xmm4 = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t xmm5 = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t xmm6 = {.data = NULL, .type = 0, .set = false};
    svm_ffi_reg_t xmm7 = {.data = NULL, .type = 0, .set = false};

    for(end = 0; end < 6 && end < ffi->nargs; end++){
        switch(args[end].type){
            case svm_integer: {
                if(end == 0){
                    rdi.data = &args[end].integer;
                    rdi.type = svm_ffi_int;
                    rdi.set = true;
                    //rdi = {.data = &args[end].integer, .type = svm_ffi_int};
                }
                else if(end == 1){
                    rsi.data = &args[end].integer;
                    rsi.type = svm_ffi_int;
                    rsi.set = true;
                    //rsi = {.data = &args[end].integer, .type = svm_ffi_int};
                }
                else if(end == 2){
                    rdx.data = &args[end].integer;
                    rdx.type = svm_ffi_int;
                    rdx.set = true;
                    //rdx = {.data = &args[end].integer, .type = svm_ffi_int};
                }
                else if(end == 3){
                    rcx.data = &args[end].integer;
                    rcx.type = svm_ffi_int;
                    rcx.set = true;
                    //rcx = {.data = &args[end].integer, .type = svm_ffi_int};
                }
                else if(end == 4){
                    r8.data = &args[end].integer;
                    r8.type = svm_ffi_int;
                    r8.set = true;
                    //r8 = {.data = &args[end].integer, .type = svm_ffi_int};
                }
                else if(end == 5){
                    r9.data = &args[end].integer;
                    r9.type = svm_ffi_int;
                    r9.set = true;
                    //r9 = {.data = &args[end].integer, .type = svm_ffi_int};
                }
                break;
            }
            case svm_boolean: {
                if(end == 0){
                    rdi.data = &args[end].boolean;
                    rdi.type = svm_ffi_bool;
                    rdi.set = true;
                    //rdi = (int*)&args[end].boolean;
                }
                else if(end == 1){
                    rsi.data = &args[end].boolean;
                    rsi.type = svm_ffi_bool;
                    rsi.set = true;
                    //rsi = (int*)&args[end].boolean;
                }
                else if(end == 2){
                    rdx.data = &args[end].boolean;
                    rdx.type = svm_ffi_bool;
                    rdx.set = true;
                    //rdx = (int*)&args[end].boolean;
                }
                else if(end == 3){
                    rcx.data = &args[end].boolean;
                    rcx.type = svm_ffi_bool;
                    rcx.set = true;
                    //rcx = (int*)&args[end].boolean;
                }
                else if(end == 4){
                    r8.data = &args[end].boolean;
                    r8.type = svm_ffi_bool;
                    r8.set = true;
                    //r8 = (int*)&args[end].boolean;
                }
                else if(end == 5){
                    r9.data = &args[end].boolean;
                    r9.type = svm_ffi_bool;
                    r9.set = true;
                    //r9 = (int*)&args[end].boolean;
                }
                break;
            }
            case svm_character: {
                if(end == 0){
                    rdi.data = &args[end].character;
                    rdi.type = svm_ffi_char;
                    rdi.set = true;
                    //rdi = (int*)&args[end].character;
                }
                else if(end == 1){
                    rdi.data = &args[end].character;
                    rdi.type = svm_ffi_char;
                    rsi.set = true;
                    //rsi = (int*)&args[end].character;
                }
                else if(end == 2){
                    rdx.data = &args[end].character;
                    rdx.type = svm_ffi_char;
                    rdx.set = true;
                    //rdx = (int*)&args[end].character;
                }
                else if(end == 3){
                    rcx.data = &args[end].character;
                    rcx.type = svm_ffi_char;
                    rcx.set = true;
                    //rcx = (int*)&args[end].character;
                }
                else if(end == 4){
                    r8.data = &args[end].character;
                    r8.type = svm_ffi_char;
                    r8.set = true;
                    //r8 = (int*)&args[end].character;
                }
                else if(end == 5){
                    r9.data = &args[end].character;
                    r9.type = svm_ffi_char;
                    r9.set = true;
                    //r9 = (int*)&args[end].character;
                }
                break;
            }
            case svm_string: {
                if(end == 0){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    rdi.data = &strarray[strarrayi - 1];
                    rdi.type = svm_ffi_str;
                    rdi.set = true;
                    //rdi = (int*)&strarray[strarrayi - 1];
                }
                else if(end == 1){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    rsi.data = &strarray[strarrayi - 1];
                    rsi.type = svm_ffi_str;
                    rsi.set = true;
                    //rsi = (int*)&strarray[strarrayi - 1];
                }
                else if(end == 2){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    rdx.data = &strarray[strarrayi - 1];
                    rdx.type = svm_ffi_str;
                    rdx.set = true;
                    //rdx = (int*)&strarray[strarrayi - 1];
                }
                else if(end == 3){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    rcx.data = &strarray[strarrayi - 1];
                    rcx.type = svm_ffi_str;
                    rcx.set = true;
                    //rcx = (int*)&strarray[strarrayi - 1];
                }
                else if(end == 4){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    r8.data = &strarray[strarrayi - 1];
                    r8.type = svm_ffi_str;
                    r8.set = true;
                    //r8 = (int*)&strarray[strarrayi - 1];
                }
                else if(end == 5){
                    strarray[strarrayi++] = svm_char_from_svm_str(args[end].string);
                    r9.data = &strarray[strarrayi - 1];
                    r9.type = svm_ffi_str;
                    r9.set = true;
                    //r9 = (int*)&strarray[strarrayi - 1];
                }
                break;
            }
            case svm_float: {
                break;
            }
            default: 
                puts("unsupported type");
                break;
        }
    }

    for(size_t i = 0; ffi->nargs - i > end; i++){
        switch(args[ffi->nargs - i].type){
            case svm_integer: {
                FFI_NATIVE_PUSH(args[ffi->nargs - i].integer);
                break;
            }
            case svm_float: {
                FFI_NATIVE_PUSH(args[ffi->nargs - i].floating);
                break;
            }
            case svm_boolean: {
                FFI_NATIVE_PUSH(args[ffi->nargs - i].boolean);
                break;
            }
            case svm_character: {
                FFI_NATIVE_PUSH(args[ffi->nargs - i].character);
                break;
            }
            case svm_string: {
                strarray[strarrayi++] = svm_char_from_svm_str(args[ffi->nargs - i].string);
                FFI_NATIVE_PUSH(strarray[strarrayi - 1]);
                break;
            }
            default: 
                fprintf(stderr, "SimpleVM: FFI: invalid type on line #%d\n", __LINE__);
        }
    }

    if(rdi.set){
        if(rdi.type == svm_ffi_int){
            FFI_NATIVE_RDI(*((int*)rdi.data));
        }
        else if(rdi.type == svm_ffi_bool){
            FFI_NATIVE_RDI(*((bool*)rdi.data));
        }
        else if(rdi.type == svm_ffi_char){
            FFI_NATIVE_RDI(*((char*)rdi.data));
        }
        else if(rdi.type == svm_ffi_str){
            FFI_NATIVE_RDI(*((char**)rdi.data));
        }
    }

    if(rsi.set){
        if(rsi.type == svm_ffi_int){
            FFI_NATIVE_RSI(*((int*)rsi.data));
        }
        else if(rsi.type == svm_ffi_bool){
            FFI_NATIVE_RSI(*((bool*)rsi.data));
        }
        else if(rsi.type == svm_ffi_char){
            FFI_NATIVE_RSI(*((char*)rsi.data));
        }
        else if(rsi.type == svm_ffi_str){
            FFI_NATIVE_RSI(*((char**)rsi.data));
        }
    }

    if(rdx.set){
        if(rdx.type == svm_ffi_int){
            FFI_NATIVE_RDX(*((int*)rdx.data));
        }
        else if(rdx.type == svm_ffi_bool){
            FFI_NATIVE_RDX(*((bool*)rdx.data));
        }
        else if(rdx.type == svm_ffi_char){
            FFI_NATIVE_RDX(*((char*)rdx.data));
        }
        else if(rdx.type == svm_ffi_str){
            FFI_NATIVE_RDX(*((char**)rdx.data));
        }
    }

    if(rcx.set){
        if(rcx.type == svm_ffi_int){
            FFI_NATIVE_RCX(*((int*)rcx.data));
        }
        else if(rcx.type == svm_ffi_bool){
            FFI_NATIVE_RCX(*((bool*)rcx.data));
        }
        else if(rcx.type == svm_ffi_char){
            FFI_NATIVE_RCX(*((char*)rcx.data));
        }
        else if(rcx.type == svm_ffi_str){
            FFI_NATIVE_RCX(*((char**)rcx.data));
        }
    }

    if(r8.set){
        if(r8.type == svm_ffi_int){
            FFI_NATIVE_R8(*((int*)r8.data));
        }
        else if(r8.type == svm_ffi_bool){
            FFI_NATIVE_R8(*((bool*)r8.data));
        }
        else if(r8.type == svm_ffi_char){
            FFI_NATIVE_R8(*((char*)r8.data));
        }
        else if(r8.type == svm_ffi_str){
            FFI_NATIVE_R8(*((char**)r8.data));
        }
    }

    if(r9.set){
        if(r9.type == svm_ffi_int){
            FFI_NATIVE_R9(*((int*)r9.data));
        }
        else if(r9.type == svm_ffi_bool){
            FFI_NATIVE_R9(*((bool*)r9.data));
        }
        else if(r9.type == svm_ffi_char){
            FFI_NATIVE_R9(*((char*)r9.data));
        }
        else if(r9.type == svm_ffi_str){
            FFI_NATIVE_R9(*((char**)r9.data));
        }
    }

    asm volatile (
        "call %0;"
        :
        : "r" (ffi->func)
    );

}
