#ifndef __FFI_H__
    #define __FFI_H__

    typedef struct {
        char *f_name;
        void *handle;
        void (*func)(void);
    } svm_ffi_t;

    svm_ffi_t *svm_load_lib(char *path);
    svm_ffi_t *svm_get_function(svm_ffi_t *ffi);
    svm_ffi_t *svm_init_ffi(char *path, char *name);

#endif
