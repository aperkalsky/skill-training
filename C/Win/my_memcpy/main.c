#include <stdio.h>
#include<stdint.h>

void* my_memcpy(void* dest, const void* src, size_t n)
{
	if (!src || !n)
	{
		return NULL;
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
	if (!src || !n)
	{
		return NULL;
	}

	if (dest == src)
	{
		return dest;	// nothing to move
	}

	uint8_t* pSrc = (uint8_t*)src;
	uint8_t* pDst = (uint8_t*)dest;

	// check for overlap
	if ((pSrc + n - 1) < pDst)
	{
		// no overlap, can use memcpy
		return my_memcpy(dest, src, n);
	}
	else
	{
		// overlapping regions
		pSrc += n;
		pDst += n;

		for (size_t i = 0; i < n; i++)
		{
			*pDst = *pSrc;
			pSrc--;
			pDst--;
		}

		return dest;
	}
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

	return 0;
}