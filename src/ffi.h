#ifndef __FFI_H__
    #define __FFI_H__

    #include "vm.h"

    svm_ffi_t *svm_load_lib(char *path);
    svm_ffi_t *svm_get_function(svm_ffi_t *ffi);
    svm_ffi_t *svm_init_ffi(char *path, char *name, size_t nargs);
    void svm_ffi_call(svm_ffi_t *ffi, svm_stack_item_t *args);

#endif
