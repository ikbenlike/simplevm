#include <stdio.h>
#include "vm.h"
#include "vm_utils.h"
#include "vm_image.h"

int main(int argc, char **argv){
    if(argc == 2){
        svm_t *vm = svm_read(argv[1]);
        svm_exec(vm);
    }
    else {
        printf("SimpleVM: please provide a file to execute");
    }

    return 0;
}
