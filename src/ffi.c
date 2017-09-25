#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "vm.h"
#include "vm_utils.h"
#include "ffi.h"

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

    /*
        TODO: use -masm=intel instead
        TODO: abandon pushq and movq etc and use Intel syntax equivalents
    */

    char **strarray = calloc(nargs, sizeof(char*));
    size_t strarrayi = 0;
    for(size_t i = 0; nargs - i >= 0; i++){
        switch(args[nargs - i].type){
            case svm_integer: {
                asm volatile (
                    "push rax;"
                    :
                    : "a" (args[nargs - i].integer)
                );
                break;
            }
            case svm_float: {
                asm volatile (
                    "push rax;"
                    :
                    : "a" (args[nargs - i].floating)
                );
            }
            case svm_boolean: {
                asm volatile (
                    "push rax;"
                    :
                    : "a" (args[nargs - i].boolean)
                );
            }
            case svm_character: {
                asm volatile (
                    "push rax;"
                    :
                    : "a" (args[nargs - i].character)
                );
            }
            case svm_string: {
                strarray[strarrayi++] = svm_char_from_svm_str(args[nargs - i].string);
                asm volatile (
                    "push rax;"
                    :
                    : "a" (strarray[strarrayi - i])
                );
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
