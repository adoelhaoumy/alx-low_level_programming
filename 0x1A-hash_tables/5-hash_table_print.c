#include "hash_tables.h"
#include <stdio.h>

/**
 * hash_table_print - Prints a hash table.
 * @ht: Pointer to the hash table.
 */
void hash_table_print(const hash_table_t *ht)
{
	unsigned long int i, isComa = 0;
	hash_node_t *node;

	if (ht)
	{
		printf("{");
		for (i = 0; i < ht->size; i++)
		{
			node = ht->array[i];
			while (node)
			{
				if (isComa)
					printf(", ");
				printf("\'%s\': \'%s\'", node->key, node->value);
				isComa = 1;
				node = node->next;
			}
		}
		printf("}\n");
	}
}
