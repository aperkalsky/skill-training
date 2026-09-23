Diagnose the const/pointer puzzle

Write code that makes each of these declarations meaningful:

const int *p;
int const *p;
int *const p;
const int *const p;

Then implement:

void process1(const uint8_t *data, size_t len);
void process2(uint8_t *const data, size_t len);
void process3(const uint8_t *const data, size_t len);

For each function, determine:

Can data be modified?
Can *data be modified?
Can data++ be performed?
What does the caller's original object need to be?

Programming task: write a small API for a read-only packet buffer where the function must guarantee that it won't modify the packet.

Then intentionally introduce incorrect const usage and fix the compiler errors without using casts.