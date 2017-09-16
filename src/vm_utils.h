#ifndef __VM_UTILS_H__
    #define __VM_UTILS_H__

    #include "vm.h"

    svm_t *svm_init(size_t code_size, size_t stack_size, size_t baseiptr);
    void svm_append_opcode(svm_stack_item_t *stack, size_t sptr, enum svm_opcodes opcode);
    void svm_append_integer(svm_stack_item_t *stack, size_t sptr, int value);
    void svm_append_float(svm_stack_item_t *stack, size_t sptr, float value);

#endif
