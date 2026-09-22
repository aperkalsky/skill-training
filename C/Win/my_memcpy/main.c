#include <stdio.h>
#include<stdint.h>

void* my_memcpy(void* dest, const void* src, size_t n)
{
	if (!dest || !src)
	{
		return NULL;
	}

	if (n == 0)
	{
		return dest;
	}

	uint8_t* pSrc = (uint8_t*)src;
	uint8_t* pDst = (uint8_t*)dest;

	for (size_t i = 0; i < n; i++)
	{
		pDst[i] = pSrc[i];
	}

	return dest;
}

void* my_memmove(void* dest, const void* src, size_t n)
{
	if (!dest || !src)
	{
		return NULL;
	}

	if (n == 0 || dest == src)
	{
		return dest;
	}

	uint8_t* pSrc = (uint8_t*)src;
	uint8_t* pDst = (uint8_t*)dest;

	if (pDst < pSrc)
	{
		for (size_t i = 0; i < n; i++)
		{
			pDst[i] = pSrc[i];
		}
	}
	else
	{
		pSrc += n - 1;
		pDst += n - 1;

		for (size_t i = 0; i < n; i++)
		{
			*pDst-- = *pSrc--;
		}
	}

	return dest;
}

int main()
{
	char buffer[32];
	const char pattern[16] = "ABCDEFGHIJKLMNO";

	printf("== testing my_memcpy() ==\n");

	sprintf_s(buffer, sizeof(pattern), "%s", pattern);

	printf("Before: %s\n", buffer);

	// memcpy gives unpredictable results in case of overlap
	my_memcpy(buffer + 2, buffer, 10);

	printf("After:  %s\n", buffer);

	sprintf_s(buffer, sizeof(pattern), "%s", pattern);

	printf("== testing my_memmove() ==\n");

	printf("Before: %s\n", buffer);

	// memmove will work correctly
	my_memmove(buffer + 2, buffer, 10);

	printf("After:  %s\n", buffer);

	// another overlap case
	sprintf_s(buffer, sizeof(buffer), "%s", pattern);

	printf("== testing my_memmove() ==\n");

	printf("Before: %s\n", buffer);

	my_memmove(buffer, buffer + 2, 10);

	printf("After:  %s\n", buffer);

	// test identical pointers
	sprintf_s(buffer, sizeof(buffer), "%s", pattern);

	printf("== testing my_memmove() ==\n");

	printf("Before: %s\n", buffer);

	my_memmove(buffer + 5, buffer + 5, 5);

	printf("After:  %s\n", buffer);

	return 0;
}