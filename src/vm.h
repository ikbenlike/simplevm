#ifndef __VM_H__
    #define __VM_H__

    #include <stdbool.h>
    #include <stddef.h>

    enum svm_opcodes {
        NOP,
        IADD,
        ISUB,
        IDIV,
        IMUL,
        IMOD,
        FADD,
        FSUB,
        FDIV,
        FMUL,
        FMOD,
        SADD,
        CADD,
        CSUB,
        CDIV,
        CMUL,
        CMOD,
        IEQ,
        INEQ,
        FEQ,
        FNEQ,
        SEQ,
        SNEQ,
        CEQ,
        CNEQ,
        CMPB,
        CMPD,
        IPRINT,
        FPRINT,
        SPRINT,
        BPRINT,
        CPRINT,
        SIN,
        PUSH,
        CMP,
        CALL,
        RET,
        RJMP,
        RJMPIT,
        RJMPIF,
        LOAD,
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
        char *str;
        size_t len;
    } svm_string_t;

    typedef struct {
        size_t nargs;
        size_t nlocals;
        size_t addr;
    } svm_function_t;

    typedef struct {
        enum svm_types type;
        union {
            int opcode;
            int integer;
            float floating;
            bool boolean;
            char character;
            svm_string_t *string;
            svm_function_t function;
        };
    } svm_stack_item_t;

    typedef struct {
        int ret;
        svm_stack_item_t *locals;
    } svm_context_t;

    typedef struct {
        size_t baseiptr;
        size_t code_size;
        size_t stack_size;
        size_t cstack_size;
        svm_stack_item_t *code;
        svm_stack_item_t *stack;
        svm_context_t *cstack;
    } svm_t;

    svm_stack_item_t *svm_exec(svm_t *vm);

#endif
