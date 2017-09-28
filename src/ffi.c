#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "vm.h"
#include "vm_utils.h"
#include "ffi.h"

#define FFI_NATIVE_PUSH(ITEM) asm volatile ( \
                                    "push rax;" \
                                    : \
                                    : "a" (ITEM) \
                                );

#define FFI_NATIVE_RDI(ITEM) asm volatile ( "mov rdi, rax;" : : "a" (ITEM));
#define FFI_NATIVE_RSI(ITEM) asm volatile ( "mov rsi, rax;" : : "a" (ITEM));
#define FFI_NATIVE_RDX(ITEM) asm volatile ( "mov rdx, rax;" : : "a" (ITEM));
#define FFI_NATIVE_RCX(ITEM) asm volatile ( "mov rcx, rax;" : : "a" (ITEM));
#define FFI_NATIVE_R8(ITEM) asm volatile ( "mov r8, rax;" : : "a" (ITEM));
#define FFI_NATIVE_R9(ITEM) asm volatile ( "mov r9, rax;" : : "a" (ITEM));


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
    return ffi;
}

svm_ffi_t *svm_init_ffi(char *path, char *name){
    svm_ffi_t *ffi = svm_load_lib(path);
    ffi = svm_get_function(ffi);
    return ffi;
}

void svm_ffi_call(svm_ffi_t *ffi, svm_stack_item_t *args, size_t nargs){
    char **strarray = calloc(nargs, sizeof(char*));
    size_t strarrayi = 0;
    size_t end = 0;
    for(end = 0; end < 6; end++){
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
                    FFI_NATIVE_RDI(args[end].string);
                }
                else if(end == 1){
                    FFI_NATIVE_RSI(args[end].string);
                }
                else if(end == 2){
                    FFI_NATIVE_RDX(args[end].string);
                }
                else if(end == 3){
                    FFI_NATIVE_RCX(args[end].string);
                }
                else if(end == 4){
                    FFI_NATIVE_R8(args[end].string);
                }
                else if(end == 5){
                    FFI_NATIVE_R9(args[end].string);
                }
                break;
            }
            default: 
                puts("type not supported yet");
                break;
        }
    }
    for(size_t i = 0; nargs - i > end; i++){
        switch(args[nargs - i].type){
            case svm_integer: {
                FFI_NATIVE_PUSH(args[nargs - i].integer);
                break;
            }
            case svm_float: {
                FFI_NATIVE_PUSH(args[nargs - i].floating);
            }
            case svm_boolean: {
                FFI_NATIVE_PUSH(args[nargs - i].floating);
            }
            case svm_character: {
                FFI_NATIVE_PUSH(args[nargs - i].floating);
            }
            case svm_string: {
                strarray[strarrayi++] = svm_char_from_svm_str(args[nargs - i].string);
                FFI_NATIVE_PUSH(args[nargs - i].floating);
            }
            default: 
                fprintf(stderr, "SimpleVM: FFI: invalid type on line #%d\n", __LINE__);
        }
    }
    asm volatile (
        "call %0;"
        :
        : "r" (ffi->func)
    );
}
