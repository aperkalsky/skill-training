Themes volatile Qualifier, Pointers, Bit Manipulation, const Pointers

Background
In bare-metal embedded programming, Microcontroller peripherals are controlled by reading and writing to explicit memory-mapped addresses. A common bug occurs when the compiler optimizes away a loop that waits for a hardware status bit to change. [1]

Your Task
Simulate an embedded hardware peripheral interaction.

 

Assume a 32-bit Hardware Status Register is mapped to a specific memory location (you can simulate this by pointing to a local uint32_t variable).

Declare a pointer named status_reg. Choose the exact right placement for const and volatile qualifiers so that

The value pointed to by status_reg can change at any time due to external hardware (volatile data).

The program is strictly forbidden from writing to this address—it is read-only hardware (const data).

The pointer itself is fixed and cannot be changed to point anywhere else (const pointer). [2, 3]

Write a function bool wait_for_ready(YOUR_POINTER_TYPE status_reg); that polls Bit 5 (the Ready flag) of the register. It must block in a loop until Bit 5 becomes 1, or timeout after 10,000 iterations.