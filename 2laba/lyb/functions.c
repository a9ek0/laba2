#include "functions.h"

////////////////////////STACK

Stack *create_stack()
{
    Stack *stack = (Stack*) malloc(sizeof (Stack));
    stack->top = NULL;
    return stack;
}
Node* create_node(Data* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void push(Stack* stack, Data* data) {
    Node* new_node = create_node(data);
    new_node->next = stack->top;
    stack->top = new_node;
}

Data* pop(Stack* stack) {
    if (stack->top == NULL) {
        return NULL;
    }
    Data* data = stack->top->data;
    Node* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return data;
}

Data *peek(const Stack *stack)
{
    if(stack->top == NULL)
    {
        printf("Stack is empty");
        exit(EXIT_FAILURE);
    }
    const Node *top_node = stack->top;
    Data *data = (Data*) malloc(sizeof (Data));
    copy_data(data, top_node->data);
    return data;
}

int is_empty(const Stack *stack)
{
    if(stack->top == NULL)
        return 1;
    else
        return 0;
}

int stack_size(Stack *stack)
{
    int size = 0;
    if(is_empty(stack) == 1)
        return 0;
    Node *new_stack = stack->top;
    while(new_stack != NULL)
    {
        size++;
        new_stack = new_stack->next;
    }
    return size;
}

void clear(Stack *stack)
{
    Node *current_node = stack->top;
    while(current_node != NULL)
    {
        Node *next_node = current_node->next;
        free(current_node->data->name);
        free(current_node->data);
        free(current_node);
        current_node = next_node;
    }
    stack->top = NULL;
}

void rewrite_stack(const char *new_file_name, Stack *stack)
{
    clear(stack);
    text_to_stack(new_file_name, stack);
}

void destroy(Stack *stack)
{
    const void *tmp;
    if(is_empty(stack) == 0 || stack == NULL)
        exit(EXIT_FAILURE);
    Node *new_stack = stack->top;
    while(new_stack != NULL)
    {
        free(new_stack->data->name);
        tmp = new_stack->next;
        free(new_stack);
        new_stack = tmp;
        tmp = NULL;
    }
    free(stack);
}

void print_stack(Stack* stack) {
    printf("Stack contents:\n");
    Node* current_node = stack->top;
    while (current_node != NULL) {
        printf("\n%s - %d\n", current_node->data->name, current_node->data->frequency);
        current_node = current_node->next;
    }
}

void sort_stack_h_l(Stack *stack) {
    int n = stack_size(stack);
    Node *curr;
    for (int i = 0; i < n; i++) {
        curr = stack->top;
        for (int j = 0; j < n - i - 1; j++) {
            if (strlen(curr->data->name) < strlen(curr->next->data->name)
                || (strlen(curr->data->name) == strlen(curr->next->data->name)
                    && curr->data->frequency < curr->next->data->frequency)
                || (strlen(curr->data->name) == strlen(curr->next->data->name)
                    && curr->data->frequency == curr->next->data->frequency
                    && strcmp(curr->data->name, curr->next->data->name) < 0)) {
                swap(curr, curr->next);
            }
            curr = curr->next;
        }
    }
}



void sort_stack_l_h(Stack *stack) {
    int n = stack_size(stack);
    Node *curr;
    for (int i = 0; i < n; i++) {
        curr = stack->top;
        for (int j = 0; j < n - i - 1; j++) {
            if (strlen(curr->data->name) > strlen(curr->next->data->name)
                || (strlen(curr->data->name) == strlen(curr->next->data->name)
                    && curr->data->frequency > curr->next->data->frequency)
                || (strlen(curr->data->name) == strlen(curr->next->data->name)
                    && curr->data->frequency == curr->next->data->frequency
                    && strcmp(curr->data->name, curr->next->data->name) > 0)) {
                swap(curr, curr->next);
            }
            curr = curr->next;
        }
    }
}


void swap(Node *a, Node *b) {
    Data *temp = a->data;
    a->data = b->data;
    b->data = temp;
}






void stack_to_lyb(Stack *stack, Library *lyb) {
    int i = 0;
    Data *buff_data;
    sort_stack_l_h(stack);
    while (strlen(stack->top->data->name) < 6) {
        buff_data = pop(stack);
        if (strlen(buff_data->name) < 1 || buff_data->frequency != 1 || !isalpha(buff_data->name[0])) {
            free(buff_data->name);
            free(buff_data);
            continue;
        }
        lyb->num_of_words++;
        lyb->words = (replacement_words *) realloc(lyb->words, lyb->num_of_words * sizeof(replacement_words));
        lyb->words[i].word1 = (char *) malloc(sizeof(char));
        lyb->words[i].word2 = NULL;
        lyb->words[i].word1 = _strdup(buff_data->name);
        i++;
        free(buff_data->name);
    }
    buff_data = NULL;
    i = 0;
    sort_stack_h_l(stack);
    for (int j = 0; j < lyb->num_of_words; ++j) {
        while (strlen(stack->top->data->name) > 6) {
            buff_data = pop(stack);
            if (buff_data->frequency > 2 &&
                strlen(lyb->words[i].word1) * buff_data->frequency + strlen(buff_data->name) <
                strlen(buff_data->name) * buff_data->frequency + strlen(lyb->words[i].word1)) {
                lyb->words[j].word2 = _strdup(buff_data->name);
                free(buff_data->name);
                break;
            }
        }
    }
    free(buff_data);
}



int find_in_stack(Stack *stack, const char *word)
{
    if(stack == NULL)
        exit(EXIT_FAILURE);
    Node *new_node = stack->top;
    int size = stack_size(stack);
    for (int i = 0; i < size; ++i) {
        if(strcmp(word, new_node->data->name) == 0)
            return i;
        new_node = new_node->next;
    }
    return 0;
}

void find_node(Stack *stack, int num, Node **node)
{
    if (stack == NULL || *node == NULL)
    {
        printf("Invalid stack or node pointer\n");
        return;
    }

    Node *new_node = stack->top;
    for (int i = 0; i < num; i++)
    {
        new_node = new_node->next;
    }
    *node = new_node;
}


void find_popular(Stack *stack)
{
    unsigned long long int size;
    size = stack_size(stack);
    Data *buff_data;
    buff_data = (Data*) malloc(sizeof (Data));
    Node *new_node;
    for (int i = 0; i < size; ++i) {
        new_node = stack->top;
        for (int j = 0; j < size - 1; ++j) {
            if (new_node->data->frequency < new_node->next->data->frequency) {
                copy_data(buff_data, new_node->data);
                copy_data(new_node->data, new_node->next->data);
                copy_data(new_node->next->data, buff_data);
            }
            new_node = new_node->next;
        }
    }
    free(buff_data);
}


void find_unpopular(Stack *stack)
{
    unsigned long long int size;
    size = stack_size(stack);
    Data *buff_data;
    buff_data = (Data*) malloc(sizeof (Data));
    Node *new_node;
    for (int i = 0; i < size; ++i) {
        new_node = stack->top;
        for (int j = 0; j < size - 1; ++j) {
            if (new_node->data->frequency > new_node->next->data->frequency) {
                copy_data(buff_data, new_node->data);
                copy_data(new_node->data, new_node->next->data);
                copy_data(new_node->next->data, buff_data);
            }
            new_node = new_node->next;
        }
    }
    free(buff_data);
}

///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Data* push_specific_node(Stack *stack, int node)
{
    if(stack == NULL)
    {
        printf("Stack doesn't exist!");
        return NULL;
    }
    Stack *tmp_stack;
    tmp_stack = create_stack();
    Data *tmp_data;
    for (int i = 0; i < node; ++i) {
        push(tmp_stack, pop(stack));
    }
    tmp_data = pop(stack);
    for (int i = 0; i < node; ++i) {
        push(stack, pop(tmp_stack));
    }
    destroy(tmp_stack);
    return tmp_data;
}



///////////////////////////////////////MEMORY
void free_node(Node *node)
{
    if(node == NULL)
        exit(EXIT_FAILURE);
    free(node->data->name);
    free(node);
}


///////////////////////////////////////HUI Z
void copy_data(Data *dest_data, const Data *source_data)
{
    if(dest_data == NULL || source_data == NULL)
        exit(EXIT_FAILURE);
    dest_data->name = (char*) malloc(strlen(source_data->name) + 1);
    strcpy_s(dest_data->name, MAX_WORD_LEN, source_data->name);
    dest_data->name[strlen(source_data->name) + 1] = '\0';
    dest_data->frequency = source_data->frequency;
}
char* dell_punct_marks(char *word_ptr) {
    if (word_ptr == NULL || *word_ptr == '\0') {
        return NULL;
    }
    char *word = word_ptr;
    size_t size = strlen(word);
    while (size > 0 && !isalpha(word[0])) {
        size--;
        memmove(word, word + 1, size);
    }
    while (size > 0 && !isalpha(word[size - 1])) {
        word[--size] = '\0';
    }

    return word;
}

void text_to_stack(const char *name, Stack *stack) {
    FILE *f;
    char word[MAX_WORD_LEN];
    const char *cleaned_word;
    fopen_s(&f, name, "r");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Data *data;
    int is_new_word;
    while (fscanf_s(f, "%50s", word) == 1) {
        cleaned_word = dell_punct_marks(word);
        if (strlen(cleaned_word) == 0) {
            continue;
        }
        is_new_word = 1;
        Node* curr = stack->top;
        while (curr != NULL) {
            if (strcmp(curr->data->name, cleaned_word) == 0) {
                curr->data->frequency++;
                is_new_word = 0;
                break;
            }
            curr = curr->next;
        }
        if (is_new_word) {
            data = (Data*) malloc(sizeof(Data));
            data->name = (char*) malloc(strlen(cleaned_word) + 1);
            strcpy_s(data->name, MAX_WORD_LEN, cleaned_word);
            data->frequency = 1;
            push(stack, data);
        }
    }
    fclose(f);
}

void copy_file(const char *source_filename, const char *destination_filename) {
    FILE *source_file;
    FILE *destination_file;
    char buffer[1024];
    size_t bytes_read;

    fopen_s(&source_file, source_filename, "rb");
    if (source_file == NULL) {
        return;
    }

    fopen_s(&destination_file, destination_filename, "wb");
    if (destination_file == NULL) {
        fclose(source_file);
        return;
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
        fwrite(buffer, 1, bytes_read, destination_file);
    }

    fclose(source_file);
    fclose(destination_file);
}

void replace_word(char* text, const char* old_word, const char* new_word) {
    char* pos = text;
    ull old_len = strlen(old_word);
    ull new_len = strlen(new_word);

    while ((pos = strstr(pos, old_word)) != NULL) {
        memmove(pos + new_len, pos + old_len, strlen(pos + old_len) + 1);
        memcpy(pos, new_word, new_len);
        pos += new_len;
    }
}

void free_lyb(Library *lyb)
{
    for (int i = 0; i < lyb->num_of_words; i++) {
        free(lyb->words[i].word1);
        free(lyb->words[i].word2);
    }
    free(lyb->words);
    free(lyb);
}

void save_control_characters(FILE* file, FILE* file1) {
    int next_char = fgetc(file1);
        while(next_char == ' ' || next_char == '\n' || next_char == '\t' || next_char == '\r' || next_char == '\v' || next_char == '\f') {
            fputc(next_char, file);
            next_char = fgetc(file1);
        }
        if(next_char != EOF)
            fseek(file1, -1, SEEK_CUR);
}

int my_isalpha(int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}


void initialize_lyb(Library *lyb) {
    lyb->num_of_words = 0;
    lyb->words = malloc(1 * sizeof(replacement_words));
    for (int i = 0; i < 1; i++) {
        lyb->words[i].word1 = NULL;
        lyb->words[i].word2 = NULL;
    }
}

void put_lyb_to_file(const char *file_name, const Library *lyb)
{
    FILE *f;
    fopen_s(&f, file_name, "at");
    if(f == NULL)
        return;
    fputs("\n", f);
    fputs("!1RvD8*ku$%TqFw&zPbN@5sLx", f);
    fputs("\n", f);
    for (int i = 0; i < lyb->num_of_words; ++i) {
        fputs(lyb->words[i].word1, f);
        fputs(" ", f);
        fputs(lyb->words[i].word2, f);
        fputs("\n", f);
    }
    fclose(f);
}





void compress_file(const char *input_file, const char* output_file, const Library *lyb)
{

    FILE *fr;

    fopen_s(&fr, input_file, "rt");
    if(fr == NULL)
        return;

    FILE *fw;

    fopen_s(&fw, output_file, "wt+");
    if(fw == NULL) {
        fclose(fr);
        return;
    }
    char word[MAX_WORD_LEN];
    char tmp_word[MAX_WORD_LEN];
    const char *cleaned_word;
    int flag = 1;
    check_control_chars(fr, fw);

    while(fscanf_s(fr, "%50s", word) == 1)
    {
        strcpy_s(tmp_word, MAX_WORD_LEN, word);
        cleaned_word = dell_punct_marks(word);
        flag = 1;
        for (int i = 0; i < lyb->num_of_words - 1; ++i) {
            if(strcmp(cleaned_word, lyb->words[i].word1) == 0) {
                replace_word(tmp_word, cleaned_word, lyb->words[i].word2);
                fputs(tmp_word, fw);
                save_control_characters(fw, fr);
                flag = 0;
            } else if(strcmp(cleaned_word, lyb->words[i].word2) == 0) {
                replace_word(tmp_word, cleaned_word, lyb->words[i].word1);
                fputs(tmp_word, fw);
                save_control_characters(fw, fr);
                flag = 0;
            }
        } if (flag == 1) {
            fputs(tmp_word, fw);
            save_control_characters(fw, fr);
        }

    }
    fclose(fr);
    fclose(fw);
}


void check_control_chars(FILE* input_file, FILE* output_file) {
    if (input_file == NULL) {
        perror("Input file opening failed");
        return;
    }
    if (output_file == NULL) {
        perror("Output file opening failed");
        fclose(input_file);
        return;
    }

    int c;
    while ((c = fgetc(input_file)) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f') {
            fputc(c, output_file);
        } else {
            fseek(input_file, -1, SEEK_CUR);
            break;
        }
    }
}