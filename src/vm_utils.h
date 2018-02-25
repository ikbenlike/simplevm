#ifndef __VM_UTILS_H__
    #define __VM_UTILS_H__

    #include "vm.h"

    svm_t *svm_init(size_t code_size, size_t stack_size, size_t cstack_size, size_t baseiptr);
    void svm_append_opcode(svm_stack_item_t *stack, size_t sptr, enum svm_opcodes opcode);
    void svm_append_integer(svm_stack_item_t *stack, size_t sptr, int value);
    void svm_append_float(svm_stack_item_t *stack, size_t sptr, float value);
    void svm_append_string(svm_stack_item_t *stack, size_t sptr, svm_string_t *value);
    void svm_append_bool(svm_stack_item_t *stack, size_t sptr, bool value);
    void svm_append_char(svm_stack_item_t *stack, size_t sptr, char value);
    void svm_append_function(svm_stack_item_t *stack, size_t sptr, svm_function_t value);
    svm_function_t svm_generate_function(size_t nargs, size_t nlocals, size_t addr);
    svm_string_t *svm_string_cat(svm_string_t *a, svm_string_t *b);
    svm_string_t *svm_string_from_cstr(char *str);
    char *svm_char_from_svm_str(svm_string_t *str);
    char *svm_get_term_input(FILE *stream);
    char *svm_type_to_str(enum svm_types type);

#endif
