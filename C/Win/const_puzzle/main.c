#include <stdio.h>
#include <stdint.h>

void test1()
{
	const int vals[] = {2, 5, 10};
	const int* p = vals;	// pointer to const int

	printf("-- test1 --\n");

	for (int i = 0; i < 3; i++)
	{
		printf("val[%d] = % d\n", i, *p++);
	}
}

void test2()
{
	const int vals[] = { 3, 22, 100};
	int const* p = vals;	// pointer to const int, different syntax

	printf("-- test2 --\n");

	for (int i = 0; i < 3; i++)
	{
		printf("val[%d] = % d\n", i, *p++);
	}

//	vals[2] = 6;	// gives compilation error
}

void test3()
{
	int val = 10;
	int* const p = &val;	// const pointer to int

	printf("-- test3 --\n");

	printf("val before = %d\n", *p);
	(*p)++;
	printf("val after = %d\n", *p);

//	p++;	// gives compilation error
}

void test4()
{
	int val = 4;	// no const specifier
	const int* const p = &val;	// const pointer to const int

	printf("-- test4 --\n");

	printf("val before = %d\n", *p);
//	p++;	// gives compilation error
	val++;	// it's legal, as according to the declaration only access path is const
	printf("val after = %d\n", *p);

	const int val2 = 3;
	const int* const p2 = &val2;

	printf("val2 = %d\n", *p2);
//	p2++;	// gives compilation error
//	val2++;	// gives compilation error
}

// data: pointer to const uint8_t, read-only data
void process1(const uint8_t* data, size_t len)
{
	printf("-- process1 --\n");

//	*data = 5;	// gives compilation error

	for (size_t i = 0; i < len; i++)
	{
		printf("data[%zu] = %d\n", i, *data);
		data++;
	}
}

// data: const pointer to uint8_t, read-write data
void process2(uint8_t* const data, size_t len)
{
//	data++;	// gives compilation error
	for (size_t i = 0; i < len; i++)
	{
		data[i] += 10;
	}
}

// data: const pointer to const uint8_t, read-only data and pointer
void process3(const uint8_t* const data, size_t len)
{
	printf("-- process3 --\n");

	for (size_t i = 0; i < len; i++)
	{
		printf("data[%zu] = %d\n", i, data[i]);
	}
}

// bad practice - do not use
void test_const_cast(void)
{
	const int x = 10;

	int* p = (int*)&x;

	*p = 20;	// undefined behavior

	printf("-- test_const_cast --\n");
	printf("%d\n", x);
}

int main()
{
	test1();
	test2();
	test3();
	test4();

	const uint8_t vals1[] = {6, 7, 9};
	process1(vals1, sizeof(vals1));

	uint8_t vals2[] = { 12, 22, 56 };

	for (size_t i = 0; i < sizeof(vals2); i++)
	{
		printf("vals2[%zu] before = %d\n", i, vals2[i]);
	}

	process2(vals2, sizeof(vals2));

	for (size_t i = 0; i < sizeof(vals2); i++)
	{
		printf("vals2[%zu] after = %d\n", i, vals2[i]);
	}

	process3(vals1, sizeof(vals1));

	test_const_cast();

	return 0;
}