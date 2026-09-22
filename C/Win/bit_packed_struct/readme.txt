Create a structure that represents a device configuration using one 32-bit integer:

31             24 23       20 19      16 15        8 7       4 3       0
+----------------+-----------+----------+------------+---------+---------+
|   reserved     |  mode     | priority |  timeout   | flags   | version |
+----------------+-----------+----------+------------+---------+---------+

Implement:

uint32_t config_set_mode(uint32_t config, uint8_t mode);
uint8_t  config_get_mode(uint32_t config);

uint32_t config_set_timeout(uint32_t config, uint8_t timeout);
uint8_t  config_get_timeout(uint32_t config);

uint32_t config_set_flag(uint32_t config, uint8_t flag);
bool     config_get_flag(uint32_t config, uint8_t flag);

Constraints:

No bit-fields.
Don't modify unrelated bits.
Correctly mask input values.
Avoid undefined behavior involving shifts.

Then add:

#define CONFIG_MODE_MASK ...
#define CONFIG_MODE_SHIFT ...