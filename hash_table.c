#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
 * TODO
 */
#define HASH_TABLE_LOAD_FACTOR 10

/*
 * Node for the data.
 */
struct node {
	int index;
	int data;
	struct node *next;
};

/*
 * Each bucket node.
 */
struct hash_table_node {
	int bucket_count;
	struct node *next;
};

/*
 * Table structure.
 */
struct hash_table {
	int table_size;
	int total_count;
	struct hash_table_node **table;
};

/*
 * hash()
 *	A simple hash function.
 */
int hash(int data, int table_size)
{
	return data % table_size;
}

/*
 * hash_table_print()
 *	Prints the hash table.
 */
void hash_table_print(struct hash_table *h)
{
	for (int i = 0; i < h->table_size; i++) {
		printf("[%d]", i);
		struct node *tmp = h->table[i]->next;
		while (tmp) {
			printf("->%d", tmp->data);
			tmp = tmp->next;
		}
		printf("\n");
	}		
}

/*
 * hash_table_data_add()
 *	Adds data to the table.
 */
void hash_table_data_add(struct hash_table *h, int data)
{
	struct node *n = (struct node *)malloc(sizeof(struct node));
	if (!n) {
		printf("Unable to allocate memory for node\n");
		return;
	}

	int index = hash(data, h->table_size);

	n->next = h->table[index]->next;
	h->table[index]->next = n;
	n->data = data;
	n->index = index;

	h->total_count++;	
	h->table[index]->bucket_count++;
}

/*
 * hash_table_create()
 *	Creates hash table.
 */
struct hash_table *hash_table_create(int size)
{
	struct hash_table *ht = (struct hash_table *)malloc(sizeof(struct hash_table));
	if (!ht) {
		printf("Unable to allocate memory for hash table\n");
		return NULL;
	}

	ht->table_size = (size / HASH_TABLE_LOAD_FACTOR);
	ht->total_count = 0;

	ht->table = (struct hash_table_node **)malloc(sizeof(struct hash_table_node *) * ht->table_size);
	if (!ht->table) {
		printf("Unable to allocate memory for hash table buckets\n");
		free(ht);
		return NULL;
	}

	unsigned int i;
	for (i = 0; i < ht->table_size; i++) {
		ht->table[i] = (struct hash_table_node *)malloc(sizeof(struct hash_table_node));
		if (!ht->table[i]) {
			printf("Unable to allocate memory for table bucket index %d\n", i);
			goto buckets_failed;
		}
		ht->table[i]->next = NULL;
		ht->table[i]->bucket_count = 0;
	}

	printf("Created hash table\n");
	return ht;

buckets_failed:
	while (i--) {
		free(ht->table[i]);
	}
	free(ht->table);
	free(ht);

	printf("Table creation failed\n");
	return NULL;
}

/*
 * Test code.
 */
int main(void)
{
	struct hash_table *htable = hash_table_create(100);
	if (!htable) {
		printf("Unable to create hash table\n");
		return -1;
	}

	srand(time(NULL));
	for (int i = 0; i < 100; i++) {
		hash_table_data_add(htable, rand() % 1000);
	}
	hash_table_print(htable);
}
