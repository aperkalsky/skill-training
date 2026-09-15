Themes: Bit Manipulation, Pointers, const Qualifier, Data Structures

Background:
Network protocols pack data tightly into bits to preserve bandwidth. You are handed a raw byte array representing a payload, and you need to parse it cleanly without modifying the source byte array.

Your Task:
Write a function that parses a raw 4-byte (32-bit) telemetry packet into a descriptive struct using bitmasking and bit-shifting (avoid C bit-fields due to compiler padding/endianness variations).

 

The raw buffer must be passed as a pointer to const uint8_t to guarantee the original data is read-only.

The 32-bit packet layout is as follows (from MSB to LSB):

Bits 31-28 (4 bits): Protocol Version

Bits 27-16 (12 bits): Sensor ID

Bits 15-4 (12 bits): Temperature Reading (unsigned integer)

Bits 3-1 (3 bits): Error Flags

Bit 0 (1 bit): Status (1 = Active, 0 = Standby)

Implement:
void parse_packet(const uint8_t *raw_bytes, TelemetryData *out_struct);