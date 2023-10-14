#include <iostream>

void soft_interrupt() {
    std::cout << "Soft interrupt handler called" << std::endl;
}

void hard_interrupt() {
    std::cout << "Hard interrupt handler called" << std::endl;
}

int main() {
    // Enable soft interrupts
    asm("sti");

    // Enable hard interrupts
    asm("sti");

    // Generate a soft interrupt
    asm("int $0x20");

    // Generate a hard interrupt
    asm("int $0x21");

    return 0;
}