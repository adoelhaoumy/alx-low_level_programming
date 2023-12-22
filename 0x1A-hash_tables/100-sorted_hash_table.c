#include "hash_tables.h"
#include <stdio.h>

/**
 * shash_table_create - Creates a hash table
 * @size: the size of the array
 * Return: a pointer to the newly created hash table
 */
shash_table_t *shash_table_create(unsigned long int size)
{
	shash_table_t *new_table;
	unsigned long int i;

	if (!size)
		return (NULL);

	new_table = malloc(sizeof(shash_table_t));
	if (!new_table)
		return (NULL);

	new_table->size = size;
	new_table->array = malloc(sizeof(shash_node_t *) * size);
	new_table->shead = NULL;
	new_table->stail = NULL;
	if (!new_table->array)
	{
		free(new_table);
		return (NULL);
	}
	for (i = 0; i < size; i++)
		new_table->array[i] = NULL;

	return (new_table);
}

/**
 * update_val - Updates the value of a key in a hash table.
 * @head: Pointer to the head of the linked list.
 * @key: The key to search for in the hash table.
 * @value: The new value to update.
 *
 * Return: 1 on success, 0 on failure.
 */
int update_val(shash_node_t **head, const char *key, const char *value)
{
	shash_node_t *cp = *head;
	char *newValue;

	while (cp)
	{
		if (strcmp(cp->key, key) == 0)
		{
			newValue = strdup(value);
			if (!cp->value)
				return (-1);
			free(cp->value);
			cp->value = newValue;
			return (1);/* Return 1 to indicate success (value updated) */
		}
		cp = cp->next;
	}
	return (0);/* Return 0 to indicate failure (key not found) */
}

/**
 * add_node - Adds a new node to a hash table.
 * @head: Pointer to the head of the linked list.
 * @key: The key of the new node.
 * @value: The value of the new node.
 *
 * Return: Pointer to the newly added node, or NULL on failure.
 */
shash_node_t *add_node(shash_node_t **head, const char *key, const char *value)
{
	shash_node_t *new;
	char *newKey, *newValue;


	new = malloc(sizeof(shash_node_t));
	if (!new)
		return (NULL);

	newKey = strdup(key);
	if (!newKey)
	{
		free(new);
		return (NULL);
	}
	newValue = strdup(value);
	if (!newValue)
	{
		free(new);
		free(newKey);
		return (NULL);
	}

	new->key = newKey;
	new->value = newValue;
	new->next = *head;

	*head = new;
	new->snext = NULL;
	new->sprev = NULL;
	return (*head);
}

/**
 * sorted_linked_list - sets doubly linked list
 * @ht: hash table to go through
 * @new: node to sort
 * Return: pass or fail
 */
int sorted_linked_list(shash_table_t *ht, shash_node_t *new)
{
	shash_node_t *sorter;

	if (!ht->shead)
	{
		ht->shead = new;
		ht->stail = new;
		return (1);
	}
	if (strcmp(new->key, ht->shead->key) < 0)
	{
		ht->shead->sprev = new;
		new->snext = ht->shead;
		ht->shead = new;
		return (1);
	}
	else
	{
		sorter = ht->shead->snext;
		while (sorter && strcmp(new->key, sorter->key) > 0)
			sorter = sorter->snext;
		if (!sorter)
		{
			new->sprev = ht->stail;
			ht->stail->snext = new;
			ht->stail = new;
			return (1);
		}
		new->sprev = sorter->sprev;
		sorter->sprev->snext = new;
		sorter->sprev = new;
		new->snext = sorter;
	}
	return (1);
}

/**
 * shash_table_set - Adds or updates a key-value pair in a hash table.
 * @ht: Pointer to the hash table.
 * @key: The key of the key-value pair.
 * @value: The value of the key-value pair.
 *
 * Return: 1 on success, 0 on failure.
 */
int shash_table_set(shash_table_t *ht, const char *key, const char *value)
{

	shash_node_t *new;
	unsigned long int index;
	int isUpdate;

	if (!ht || !ht->size || !strlen(key))
		return (0);

	index = key_index((const unsigned char *)key, ht->size);
	isUpdate = update_val(&(ht->array[index]), key, value);
	if (isUpdate == 0)/* If the key doesn't exist in the linked list */
	{
		/* Add a new node with the key-value pair */
		new = add_node(&(ht->array[index]), key, value);
		if (!new)
			return (0);

		ht->array[index] = new;
	}
	else if (isUpdate == -1)/* If memory allocation for value update failed */
		return (0);
	return (sorted_linked_list(ht, new));
}

/**
 * shash_table_get - Retrieves the value associated with a key in a hash table.
 * @ht: Pointer to the hash table.
 * @key: The key to search for.
 *
 * Return: Pointer to the value associated with the key, or NULL if not found.
 */
char *shash_table_get(const shash_table_t *ht, const char *key)
{
	unsigned long int index;
	shash_node_t *node;

	if (!ht)
		return (NULL);

	index = key_index((const unsigned char *)key, ht->size);
	node = ht->array[index];
	while (node)
	{
		/* Compare the current node's key with the provided key */
		if (strcmp(node->key, key) == 0)
			return (node->value);
		node = node->next;
	}
	return (NULL);

}

/**
 * shash_table_print - Prints a hash table.
 * @ht: Pointer to the hash table.
 */
void shash_table_print(const shash_table_t *ht)
{
	unsigned long int isComa = 0;
	shash_node_t *node;

	if (ht)
	{
		printf("{");
		node = ht->shead;
		while (node)
		{
			if (isComa)
				printf(", ");
			printf("\'%s\': \'%s\'", node->key, node->value);
			isComa = 1;
			node = node->snext;
		}
		printf("}\n");
	}
}

/**
 * shash_table_print_rev - prints table reversed
 * @ht: hash table to print
 */
void shash_table_print_rev(const shash_table_t *ht)
{
	unsigned long int isComa = 0;
	shash_node_t *tail;

	if (ht)
	{
		printf("{");
		tail = ht->stail;
		while (tail)
		{
			if (isComa)
				printf(", ");
			printf("\'%s\': \'%s\'", tail->key, tail->value);
			isComa = 1;
			tail = tail->sprev;
		}
		printf("}\n");
	}
}
/**
 * shash_table_delete - Deletes a hash table.
 * @ht: Pointer to the hash table.
 */
void shash_table_delete(shash_table_t *ht)
{
	unsigned long int i;
	shash_node_t *tmp, *delete;

	if (ht)
	{
		for (i = 0; i < ht->size; i++)
		{
			tmp = ht->array[i];
			while (tmp)
			{
				delete = tmp;
				tmp = tmp->next;
				free(delete->key);
				free(delete->value);
				free(delete);
			}
		}
		free(ht->array);
		free(ht);
	}
}
