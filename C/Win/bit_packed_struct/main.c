#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*
 
uint32_t that keeps device configuration

31             24 23       20 19      16 15        8 7       4 3       0
+----------------+-----------+----------+------------+---------+---------+
|   reserved     |  mode     | priority |  timeout   | flags   | version |
+----------------+-----------+----------+------------+---------+---------+

*/

#define CONFIG_VERSION_MASK 0x0F
#define CONFIG_VERSION_SHIFT 0
#define CONFIG_FLAGS_MASK 0x0F
#define CONFIG_FLAGS_SHIFT 4
#define CONFIG_TIMEOUT_MASK 0xFF
#define CONFIG_TIMEOUT_SHIFT 8
#define CONFIG_PRIORITY_MASK 0x0F
#define CONFIG_PRIORITY_SHIFT 16
#define CONFIG_MODE_MASK 0x0F
#define CONFIG_MODE_SHIFT 20

uint32_t config_set_mode(uint32_t config, uint8_t mode)
{
	uint32_t new_mode_val = (uint32_t)(mode & CONFIG_MODE_MASK) << CONFIG_MODE_SHIFT;

	config &= ~(CONFIG_MODE_MASK << CONFIG_MODE_SHIFT);
	config |= new_mode_val;

	return config;
}

uint8_t  config_get_mode(uint32_t config)
{
	return (uint8_t)((config >> CONFIG_MODE_SHIFT) & CONFIG_MODE_MASK);
}

uint32_t config_set_timeout(uint32_t config, uint8_t timeout)
{
	uint32_t new_timeout_val = (uint32_t)(timeout & CONFIG_TIMEOUT_MASK) << CONFIG_TIMEOUT_SHIFT;

	config &= ~(CONFIG_TIMEOUT_MASK << CONFIG_TIMEOUT_SHIFT);
	config |= new_timeout_val;

	return config;
}

uint8_t  config_get_timeout(uint32_t config)
{
	return (uint8_t)((config >> CONFIG_TIMEOUT_SHIFT) & CONFIG_TIMEOUT_MASK);
}

// set flag 0..3
uint32_t config_set_flag(uint32_t config, uint8_t flag_index)
{
	// TODO: add flag index boundaries checking
	return config | (1u << (CONFIG_FLAGS_SHIFT + flag_index));
}

// set flag 0..3
uint32_t config_clear_flag(uint32_t config, uint8_t flag_index)
{
	// TODO: add flag index boundaries checking
	return config & ~(1u << (CONFIG_FLAGS_SHIFT + flag_index));
}

// set flag 0..3
bool config_get_flag(uint32_t config, uint8_t flag_index)
{
	// TODO: add flag index boundaries checking
	return (config & (1u << (CONFIG_FLAGS_SHIFT + flag_index))) != 0;
}

int main()
{
	uint32_t sample_cfg = 0x00100690; // mode:1, flags:9, timeout:6

	printf("mode = %d\n", config_get_mode(sample_cfg));

	printf("set new mode to 14\n");

	printf("before = 0x%08X\n", sample_cfg);

	uint32_t new_cfg = config_set_mode(sample_cfg, 14);

	printf("after  = 0x%08X\n", new_cfg);

	printf("new mode = %d\n", config_get_mode(new_cfg));

	printf("timeout = %d\n", config_get_timeout(sample_cfg));

	printf("flag 0 = %d\n", config_get_flag(sample_cfg, 0));
	printf("flag 1 = %d\n", config_get_flag(sample_cfg, 1));
	printf("flag 2 = %d\n", config_get_flag(sample_cfg, 2));
	printf("flag 3 = %d\n", config_get_flag(sample_cfg, 3));

	uint32_t cfg = sample_cfg;

	cfg = config_set_flag(cfg, 1);

	printf("after setting flag 1: 0x%08X\n", cfg);

	cfg = config_clear_flag(cfg, 3);

	printf("after clearing flag 3: 0x%08X\n", cfg);

	return 0;
}