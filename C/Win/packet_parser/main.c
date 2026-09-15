#include <stdio.h>
#include <stdint.h>

/*
The 32 - bit packet layout is as follows(from MSB to LSB) :

	Bits 31 - 28 (4 bits) : Protocol Version

	Bits 27 - 16 (12 bits) : Sensor ID

	Bits 15 - 4 (12 bits) : Temperature Reading(unsigned integer)

	Bits 3 - 1 (3 bits) : Error Flags

	Bit 0 (1 bit) : Status(1 = Active, 0 = Standby)
*/

#define NUM_BITS_IN_BYTE		8
#define NUM_RAW_BYTES_TO_PARSE	4

#define NUM_BITS_IN_STATUS			1
#define BIT_MASK_STATUS				0x01
#define NUM_BITS_IN_ERR_FLAGS		3
#define BIT_MASK_ERR_FLAGS			0x07
#define NUM_BITS_IN_TEMPERATURE		12
#define BIT_MASK_TEMPERATURE		0xFFF
#define NUM_BITS_IN_SENSOR_ID		12
#define BIT_MASK_SENSOR_ID			0xFFF
#define BIT_MASK_PROTOCOL_VER		0x0F



typedef struct
{
	uint8_t protocol_version;
	uint16_t sensor_id;
	uint16_t temperature;
	uint8_t error_flags;
	uint8_t status;

}TelemetryData;

void parse_packet(const uint8_t* raw_bytes, TelemetryData* out_struct)
{
	uint32_t buf = ((uint32_t)raw_bytes[0] << 24) |
		((uint32_t)raw_bytes[1] << 16) |
		((uint32_t)raw_bytes[2] << 8) |
		((uint32_t)raw_bytes[3]);

	printf("buffer before parsing = 0x%08X\n", buf);

	// do parsing
	out_struct->status = (uint8_t)(buf & BIT_MASK_STATUS);
	buf >>= NUM_BITS_IN_STATUS;

	out_struct->error_flags = (uint8_t)(buf & BIT_MASK_ERR_FLAGS);
	buf >>= NUM_BITS_IN_ERR_FLAGS;

	out_struct->temperature = (uint16_t)(buf & BIT_MASK_TEMPERATURE);
	buf >>= NUM_BITS_IN_TEMPERATURE;

	out_struct->sensor_id = (uint16_t)(buf & BIT_MASK_SENSOR_ID);
	buf >>= NUM_BITS_IN_SENSOR_ID;

	out_struct->protocol_version = (uint8_t)(buf & BIT_MASK_PROTOCOL_VER);
}

int main()
{
	// Example layout representation: 
	// 0x12345678 -> 0001 001000110100 010101100111 100 0
	uint8_t data_to_parse[NUM_RAW_BYTES_TO_PARSE] = {0x12, 0x34, 0x56, 0x78};
	
	TelemetryData parsed_data;

	parse_packet(data_to_parse, &parsed_data);

	printf("protocol_version = %d (Expected: 1)\n", parsed_data.protocol_version);
	printf("sensor_id        = %d (Expected: 564)\n", parsed_data.sensor_id);
	printf("temperature      = %d (Expected: 1383)\n", parsed_data.temperature);
	printf("error_flags      = %d (Expected: 4)\n", parsed_data.error_flags);
	printf("status           = %d (Expected: 0)\n", parsed_data.status);
}