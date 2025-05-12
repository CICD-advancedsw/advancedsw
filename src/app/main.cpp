#include "application/dvm.h"
#include "presentation/controller.h"

int main(void) {
    DVM dvm;
    Controller controller(&dvm);
    controller.run(); 

    return 0;
}