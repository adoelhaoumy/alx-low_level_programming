#include "search_algos.h"

/**
 * linear_search - Search for a value in an array using linear search algorithm
 * @array: Pointer to the array to search in
 * @size: The number of elements in the array
 * @value: The value to search for
 *
 * Return: If the value is found, the index of the first occurrence is returned
 *         If the value is not found or the array is NULL, -1 is returned
 */
int linear_search(int *array, size_t size, int value)
{
	size_t i;

	if (!array)
		return (-1);

	for (i = 0; i < size; i++)
	{
		printf("Value checked array[%lu] = [%d]\n", i, array[i]);
		if (array[i] == value)
			return (i);
	}

	return (-1);
}
