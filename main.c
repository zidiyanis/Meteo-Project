#include "meteo.h"


int main(int argc, const char **argv)
{
    int file_nb;
    external_t **files;
    int f;
    options_t *options;
    function_pointer_t **functions = get_functions(6, "--abr", &abr, "--avl", &avl, "--tab", &list);

    if (functions == NULL)
        return (4);
    options = malloc(sizeof(options_t));
    if (options == NULL)
        return (4);
    options->input = NULL;
    options->output = NULL;
    options->reverse = 0;
    options->f = NULL;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-r"))
            options->reverse = 1;
        else if (!strcmp("-f", argv[i]) && i < argc - 1) {
            options->input = fopen(argv[i + 1], "r");
            if (options->input == NULL) {
                free_options(options);
                return (2);
            }
            i++;
        }
        else if (!strcmp("-o", argv[i]) && i < argc - 1) {
            options->output = fopen(argv[i + 1], "w");
            if (options->output == NULL) {
                free_options(options);
                return (3);
            }
            i++;
        }
        else {
            f = functions_contains(functions, argv[i]);
            if (f != -1 &&  options->f != NULL) {
                free_functions(functions);
                free_options(options);
                return (1);
            }
            if (f != -1)
                options->f = functions[f]->f;
            else {
                free_functions(functions);
                free_options(options);
                return (1);
            }
        }
    }
    if (options->f == NULL)
        options->f = &avl;
    if (options->input == NULL || options->output == NULL) {
        free_functions(functions);
        free_options(options);
        return (1);
    }
    file_nb = merge_sort(options);
    if (file_nb == -1) {
        return (4);
    }
	files = create_files(file_nb);
    if (files == NULL)
        return (4);
    if (sort_files(files, options) == -1)
        return (4);
    free_functions(functions);
	free_files(files);
    free(options);
	return (0);
}


double str_to_double(double *result, const char* str)
{
    char *endPtr;
    long value = strtod(str, &endPtr);

    if (strcmp(endPtr, ""))
        return (0);
    if (value > __DBL_MAX__ || value < __DBL_MIN__)
        return (0);
    *result = value;
    return (1);
}

int compare(double a, double b, int reverse)
{
    if (reverse)
        return (a > b);
    return (a < b);
}

linked_list_t *get_node_from_index(linked_list_t *list, int index)
{
    while (index > 0) {
        list = list->next;
        index--;
    }
    return (list);
}

int get_list_size(linked_list_t *list)
{
    int i = 0;

    while(list != NULL) {
        list = list->next;
        i++;
    }
    return (i);
}

int add_to_list(linked_list_t **list,  data_t *data, int index)
{
    linked_list_t *node = *list;
    linked_list_t *new = malloc(sizeof(linked_list_t));

    if (new == NULL) {
        MEMORY_ERR();
        return (1);
    }
    new->next = NULL;
    new->data = data;
    if (node == NULL) {
        *list = new;
        return (0);
    }
    if (index == 0) {
        new->next = node;
        *list = new;
        return (0);
    }
    while (node->next != NULL && index > 1) {
        node = node->next;
        index--;
    }
    node->next = new;
    return (0);
}

void free_list(linked_list_t *list)
{
    linked_list_t *prev;

    while (list != NULL) {
        prev = list;
        list = list->next;
        for (int i = 0; prev->data->value[i] != NULL; i++)
            free(prev->data->value[i]);
        free(prev->data->value);
        free(prev->data);
        free(prev);
    }
}

data_t *create_data(double key, char *value)
{
    data_t *data = malloc(sizeof(data_t));
	if (data == NULL)
		return (NULL);
	data->key = key;
	data->value = NULL;
    if (add_to_array(&data->value, value) == 1)
		return (NULL);
    data->nb = 1;
    return (data);
}

linked_list_t *list_insert(linked_list_t **list, double key, int reverse, char *value)
{
    data_t *data;
    linked_list_t *node = *list;
    int index = 0;
    if (node == NULL) {
        data = create_data(key, value);
        if (data == NULL)
            return (NULL);
        add_to_list(list, data, 0);
    }
    else {
        while(node != NULL) {
            if (node->data->key == key) {
                node->data->nb++;
                if (add_to_array(&node->data->value, value) == 1)
					return (NULL);
                return (*list);
            }
            if (compare(node->data->key, key, reverse)) {
                data = create_data(key, value);
                add_to_list(list, data, index);
                return (*list);
            }
            node = node->next;
            index++;
        }
        data = create_data(key, value);
        add_to_list(list, data, index);
    }
    return (*list);
}

void list_save(linked_list_t *list, FILE *f)
{
    while (list != NULL) {
        for (int i = 0; i < list->data->nb; i++)
	        fprintf(f, "%f;%s\n", list->data->key, list->data->value[i]);
        list = list->next;
    }
}

int list(FILE *file, char buffer[RAM], int reverse)
{
	double key = 0;
	char *line;
    char *key_buffer;
	char *value;
	linked_list_t *list = NULL;
    char *p;

    line = strtok_r(buffer, "\n",  &p);
    while (line != NULL) {
		if (line[0] == '\n')
			continue;
		key_buffer = strtok(line, ";");
    	value = strtok(NULL, "");
		str_to_double(&key, key_buffer);
        list_insert(&list, key, reverse, value);
        line = strtok_r(NULL, "\n", &p);
    }
	list_save(list, file);
	free_list(list);
	return (0);
}



void free_array(char **array)
{
    if (array == NULL)
        return;
    for (int i = 0; array[i] != NULL; i++)
        free(array[i]);
    free(array);
}

int get_array_len(char **array)
{
    int i = 0;

    if (array == NULL)
        return (0);
    for(; array[i] != NULL; i++);
    return (i);
}

int add_to_array(char ***array, char *msg)
{
    int len = get_array_len(*array);
    int tmp_len;
    char **new_array = malloc(sizeof(char *) * (len + 2));

    if (new_array == NULL) {
        MEMORY_ERR();
        return (1);
    }
    for (int i = 0; i < len; i++) {
        tmp_len = strlen((*array)[i]);
        new_array[i] = malloc(sizeof(char) * (tmp_len + 1));
        if (new_array[i] == NULL) {
            MEMORY_ERR();
            return (1);
        }
        for (int j = 0; j < tmp_len; j++)
            new_array[i][j] = (*array)[i][j];
        new_array[i][tmp_len] = '\0';
    }
    tmp_len = strlen(msg);
    new_array[len] = malloc(sizeof(char) * (tmp_len + 1));
    if (new_array[len] == NULL) {
        MEMORY_ERR();
        return (1);
    }
    for (int i = 0; i < tmp_len; i++)
        new_array[len][i] = msg[i];
    new_array[len][tmp_len] = '\0';
    new_array[len + 1] = NULL;
    free_array(*array);
    *array = new_array;
    return (0);
}




int get_min(external_t **files, options_t *options)
{
	double min = 0;
	int index = -1;
	int first = 1;

	for (int i = 0; files[i] != NULL; i++)
		if (files[i]->empty == 0 && (first || compare(files[i]->key, min, options->reverse))) {
            first = 0;
            min = files[i]->key;
            index = i;
        }
	return (index);
}

void free_files(external_t **files)
{
    for (int i = 0; files[i] != NULL; i++) {
		free(files[i]->value);
		free(files[i]);
	}
	free(files);
}

external_t **create_files(int file_nb)
{
    external_t *ex;
	char *key_buffer;
	char *value;
	char *line;
    external_t **files = malloc(sizeof(external_t *) * (file_nb + 1));

	if (files == NULL)
		return (NULL);
	for (int i = 0; i < file_nb; i++) {
	    ex = malloc(sizeof(external_t));
		if (ex == NULL)
			return (NULL);
		ex->empty = 0;
		ex->index = i;
		sprintf(ex->filename, "%d", i);
		ex->file = fopen(ex->filename, "r");
		if (ex->file == NULL)
			return (NULL);
		line = NULL;
		getline(&line, &(size_t){0}, ex->file);
		key_buffer = strtok(line, ";");
    	value = strtok(NULL, "");
		str_to_double(&(ex->key), key_buffer);
		ex->value = malloc(sizeof(char) * (strlen(value) + 1));
		if (ex->value == NULL)
			return (NULL);
		strcpy(ex->value, value);
		ex->value[strlen(value)] = '\0';
		files[i] = ex;
		free(line);
	}
	files[file_nb] = NULL;
    return (files);
}

int sort_files(external_t **files, options_t *options)
{
	while (1) {
		int index = get_min(files, options);
		if (index == -1)
			break;
		fprintf(options->output, "%s", files[index]->value);
		char *ss = NULL;
		int uu = getline(&ss, &(size_t){0}, files[index]->file);
		if (uu == -1) {
            remove(files[index]->filename);
			fclose(files[index]->file);
			files[index]->empty = 1;
		}
		else {
			files[index]->key = 0;
			char *key_buffer = strtok(ss, ";");
    		char *value = strtok(NULL, "");
			str_to_double(&(files[index]->key), key_buffer);
			free(files[index]->value);
			files[index]->value = malloc(sizeof(char) * (strlen(value) + 1));
			if (files[index]->value == NULL)
				return (-1);
			strcpy(files[index]->value, value);
			files[index]->value[strlen(value)] = '\0';
		}
		free(ss);
	}
	fclose(options->output);
	return (0);
}

int write_file(char str[10], int *file_nb, char *buffer, options_t *options)
{
    FILE *file;

    sprintf(str, "%d", *file_nb);
    file = fopen(str, "w");
	if (file == NULL)
		return (-1);
    if (options->f(file, buffer, options->reverse) != 0)
		return (-1);
    fclose(file);
    (*file_nb)++;
	return (0);
}

int merge_sort(options_t *options)
{
    int file_nb = 0;
    int c = 1;
    int size;
    char str[10]; 
	char *buffer = NULL;
	char *line = NULL;

	for (int x = 0; c; x = 0) {
		buffer = malloc(sizeof(char));
		if (buffer == NULL)
			return (-1);
		buffer[0] = '\0';
		while (x < RAM) {
			size = getline(&line, &(size_t){0}, options->input);
			if (size == -1) {
                if (x == 0)
				    c = 0;
                free(line);	
				break;
			}
			buffer = realloc(buffer, x + size + 1);
			if (buffer == NULL)
				return (-1);
			strcat(buffer, line);
            free(line);
			x += size;
		}
		if (c == 1)
			if (write_file(str, &file_nb, buffer, options) == -1)
				return (-1);
		free(buffer);
	}
	fclose(options->input);
    return (file_nb);
}





function_pointer_t **get_functions(int nb, ...)
{
    va_list(args);
    va_start(args,nb);  
    function_pointer_t **functions = malloc(sizeof(function_pointer_t *) * (nb / 2 + 1));

    if (functions == NULL)
        return (NULL);
    for (int i = 0; i < nb / 2; i++) {
        functions[i] = malloc(sizeof(function_pointer_t));
        if (functions[i] == NULL)
            return (NULL);
        functions[i]->name = va_arg(args, char *);
        functions[i]->f = va_arg(args, int (*)(FILE *f, char buffer[RAM], int reverse));
    }
    va_end(args);
    functions[nb / 2] = NULL;
    return (functions);
}

void free_functions(function_pointer_t **functions)
{
    for (int i = 0; functions[i] != NULL; i++)
        free(functions[i]);
    free(functions);
}

int functions_contains(function_pointer_t **functions, const char *key)
{
    for (int i = 0; functions[i] != NULL; i++)
        if (!strcmp(functions[i]->name, key))
            return (i);
    return (-1);
}

void free_options(options_t *options)
{
    if (options->input != NULL)
        fclose(options->input);
    if (options->output != NULL)
        fclose(options->output);
    free(options);
}


static avl_tree_t *avl_create()
{
	avl_tree_t *tree = NULL;

	if ((tree = malloc(sizeof(avl_tree_t))) == NULL)
		return NULL;
	tree->root = NULL;
	return tree;
}

static avl_node_t *avl_create_node()
{
	avl_node_t *node = NULL;
	
	if ((node = malloc(sizeof(avl_node_t))) == NULL)
		return NULL;
	node->left = NULL;
	node->right = NULL;
	node->data = malloc(sizeof(data_t));
	if (node->data == NULL)
		return (NULL);
	node->data->key = 0;
	node->data->value = 0;
    node->data->nb = 1;
	return node;	
}

static int avl_node_height(avl_node_t *node)
{
	int height_left = 0;
	int height_right = 0;

	if (node->left)
		height_left = avl_node_height(node->left);
	if (node->right)
		height_right = avl_node_height(node->right);
	return ((height_right > height_left) ? ++height_right : ++height_left);
}

static int avl_balance_factor(avl_node_t *node)
{
	int bf = 0;

	if (node->left)
		bf += avl_node_height(node->left);
	if (node->right)
		bf -= avl_node_height(node->right);
	return (bf);
}

static avl_node_t *avl_rotate_leftleft(avl_node_t *node)
{
 	avl_node_t *a = node;
	avl_node_t *b = a->left;
	
	a->left = b->right;
	b->right = a;
	return( b );
}

static avl_node_t *avl_rotate_leftright(avl_node_t *node)
{
	avl_node_t *a = node;
	avl_node_t *b = a->left;
	avl_node_t *c = b->right;
	
	a->left = c->right;
	b->right = c->left; 
	c->left = b;
	c->right = a;
	return (c);
}

static avl_node_t *avl_rotate_rightleft(avl_node_t *node)
{
	avl_node_t *a = node;
	avl_node_t *b = a->right;
	avl_node_t *c = b->left;
	
	a->right = c->left;
	b->left = c->right; 
	c->right = b;
	c->left = a;
	return (c);
}

static avl_node_t *avl_rotate_rightright(avl_node_t *node)
{
	avl_node_t *a = node;
	avl_node_t *b = a->right;
	
	a->right = b->left;
	b->left = a; 
	return (b);
}

static avl_node_t *avl_balance_node(avl_node_t *node)
{
	avl_node_t *newroot = NULL;
	int bf;

	if (node->left)
		node->left = avl_balance_node(node->left);
	if (node->right) 
		node->right = avl_balance_node(node->right);
	bf = avl_balance_factor(node);
	if (bf >= 2) {
		if (avl_balance_factor(node->left) <= -1)
			newroot = avl_rotate_leftright(node);
		else 
			newroot = avl_rotate_leftleft(node);
	}
	else if (bf <= -2 ) {
		if (avl_balance_factor(node->right) >= 1)
			newroot = avl_rotate_rightleft(node);
		else 
			newroot = avl_rotate_rightright(node);
	}
	else
		newroot = node;
	return( newroot );	
}

static void avl_balance( avl_tree_t *tree )
{
	avl_node_t *newroot = NULL;

	newroot = avl_balance_node(tree->root);
	if (newroot != tree->root)
		tree->root = newroot; 
}

static int avl_insert( avl_tree_t *tree, double key, int balance, int reverse, char *value)
{
	avl_node_t *node = NULL;
	avl_node_t *next = NULL;
	avl_node_t *last = NULL;

	if( tree->root == NULL ) {
		node = avl_create_node();
		node->data->key = key;
		node->data->value = NULL;
		if (add_to_array(&node->data->value, value) == 1)
			return (1);
		tree->root = node;
	}
	else {
		next = tree->root;
		while (next != NULL) {
			last = next;
			if (compare(key, next->data->key, reverse))
				next = next->left;
			else if (compare(key, next->data->key, !reverse))
				next = next->right;
			else if (key == next->data->key) {
				if (add_to_array(&next->data->value, value) == 1)
					return (1);
                next->data->nb++;
                return (0);
			}
		}
		node = avl_create_node();
		node->data->key = key;
		node->data->value = NULL;
		if (add_to_array(&node->data->value, value) == 1)
			return (1);
		if (compare(key, last->data->key, reverse))
			last->left = node;
		if (compare(key, last->data->key, !reverse))
			last->right = node;	
	}
	if (balance)
		avl_balance(tree);
	return (0);
}

static void avl_save_node( avl_node_t *node, int depth, FILE *f)
{
	if (node->left)
		avl_save_node(node->left, depth + 2, f);
    for (int i = 0; i < node->data->nb; i++) {
	    fprintf(f, "%f;%s\n", node->data->key, node->data->value[i]);
	}
	if (node->right)
		avl_save_node(node->right, depth + 2, f);
}

static void avl_free_node(avl_node_t *node)
{
    if (node == NULL)
        return;
    avl_free_node(node->left);
    avl_free_node(node->right);
	free_array(node->data->value);
	free(node->data);
    free(node);
}

static int tree(FILE *f, char buffer[RAM], int balance, int reverse)
{
	double key = 0;
	char *line;
	char *key_buffer;
	char *value;
	avl_tree_t *tree = avl_create();
	char *p;

    line = strtok_r(buffer, "\n",  &p);
    while (line != NULL) {
		if (line[0] == '\n')
			continue;
		key_buffer = strtok(line, ";");
    	value = strtok(NULL, "");
		str_to_double(&key, key_buffer);
        avl_insert(tree, key, balance, reverse, value);
        line = strtok_r(NULL, "\n", &p);
    }
	avl_save_node(tree->root, 0, f);
	avl_free_node(tree->root);
	free(tree);
	return (0);
}

int avl(FILE *file, char buffer[RAM], int reverse)
{
	return (tree(file, buffer, 1, reverse));
}

int abr(FILE *file, char buffer[RAM], int reverse)
{
	return (tree(file, buffer, 0, reverse));
}



