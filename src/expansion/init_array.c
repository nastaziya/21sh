
#include "../../inc/expansion.h"

void    tab_expansion_init(t_dynamic_array *arr)
{
	arr->used_size = 0;
	arr->capacity = TAB_INITIAL_CAPACITY;
	if (!(arr->array_expanded =(char*)malloc(sizeof(char) * (arr->capacity + 1))))
		return;
	arr->array_expanded[arr->used_size] = '\0';
	
}

void		add_char_to_array(t_dynamic_array *arr, char item)
{
    char            *temp;
	int				i;

	i = -1;
	if (arr->used_size == arr->capacity)
	{
		temp = arr->array_expanded;
		arr->capacity = (arr->used_size * 3) / 2 + 1;
		arr->array_expanded = malloc(sizeof(char) * arr->capacity + 1);
		if (arr->array_expanded == NULL)
			exit(EXIT_FAILURE);
		while (++i < arr->used_size)
			arr->array_expanded[i] = temp[i];
		free(temp);
	}
	arr->array_expanded[arr->used_size] = item;
	++arr->used_size;
	arr->array_expanded[arr->used_size] = '\0';

}
