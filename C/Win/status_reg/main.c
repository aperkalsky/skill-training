#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h> // Required for Windows Threading API

#define READY_BIT_MASK    (1 << 5) // Bit 5 is the "Ready" flag
#define MAX_TIMEOUT_LOOPS 500000000 // Increased for asynchronous timing

// This global variable simulates the memory-mapped hardware status register
// The hardware thread will change this
uint32_t simulated_hardware_register = 0;

// declare pointer to read-only HW register
// Note: Qualifiers before the asterisk * apply to the data being pointed to.
//       Qualifiers after the asterisk * apply to the pointer variable itself.
typedef const volatile uint32_t* const REGS_POINTER_TYPE;

/**
 * Polls Bit 5 of the register. Blocks until Bit 5 becomes 1.
 * Returns true if successful, false if it timeouts.
 */
bool wait_for_ready(REGS_POINTER_TYPE reg)
{
    for (uint32_t i = 0; i < MAX_TIMEOUT_LOOPS; i++)
    {
        if (*reg & READY_BIT_MASK)
        {
            return true;
        }
    }

    return false;
}

// Simulated hardware behavior running on a separate thread
DWORD WINAPI hardware_thread_proc(LPVOID lpParam) {
    printf("[Hardware Thread] Peripheral booted. Simulating 0.1 second hardware delay...\n");
    Sleep(100);

    // Simulate data arriving/peripheral ready by flipping bit 5
    simulated_hardware_register = READY_BIT_MASK;
    printf("[Hardware Thread] Processing complete! Bit 5 set to 1.\n");
    return 0;
}

int main()
{
    printf("--- Test: Multi-Threaded Hardware Register Monitor ---\n");


    REGS_POINTER_TYPE status_reg = &simulated_hardware_register;

    // Try uncommenting the line below! Visual Studio will throw a compilation error 
    // because your type definition successfully prevents writing to a read-only register:
    // *status_reg = 0xFF; 

    // Spin up the background hardware simulator thread
    HANDLE hThread = CreateThread(NULL, 0, hardware_thread_proc, NULL, 0, NULL);
    if (hThread == NULL) {
        printf("Failed to create hardware thread!\n");
        return 1;
    }

    // Call the function to poll the pointer
    printf("[Main Thread] Waiting for hardware to become ready...\n");

    if (wait_for_ready(status_reg)) {
        printf("[Main Thread] SUCCESS: Correctly detected the Ready flag from the thread!\n");
    } else {
        printf("[Main Thread] FAILURE: Timed out before hardware thread responded.\n");
    }

    // Clean up thread handle
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}
