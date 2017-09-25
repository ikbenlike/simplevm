#ifndef __FFI_H__
    #define __FFI_H__

    typedef struct {
        char *f_name;
        void *handle;
        void (*func)(void);
    } svm_ffi_t;

#endif
