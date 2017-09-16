#ifndef __VM_H__
    #define __VM_H__

    #include <stdbool.h>
    #include <stddef.h>

    enum svm_opcodes {
        NOP = 0,
        IADD = 1,
        ISUB = 2,
        IDIV = 3,
        IMUL = 4,
        IMOD = 5,
        FADD = 6,
        FSUB = 7,
        FDIV = 8,
        FMUL = 9,
        FMOD = 10,
        IPRINT,
        FPRINT,
        PUSH,
        HALT,
    };

    enum svm_types {
        svm_opcode,
        svm_integer,
        svm_float,
        svm_boolean,
        svm_character,
        svm_string,
        svm_function,
    };

    typedef struct {
        enum svm_types type;
        union {
            int opcode;
            int integer;
            float floating;
            bool boolean;
            char character;
        };
    } svm_stack_item_t;

    typedef struct {
        size_t baseiptr;
        size_t code_size;
        size_t stack_size;
        svm_stack_item_t *code;
        svm_stack_item_t *stack;
    } svm_t;

    svm_stack_item_t *svm_exec(svm_t *vm);

#endif
