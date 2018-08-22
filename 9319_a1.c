#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct huffman_node{
    unsigned char   character;
    int    frequency;
    struct huffman_node *left_child;
    struct huffman_node *right_child;
}Huffman_Node;

typedef struct heap{
    int capacity;
    int size;
    Huffman_Node** arr;
} Heap;

typedef struct stack{
    Huffman_Node data[256];
    int index;
    int size;
} Stack;

typedef struct huffman_code_dict{
    char** codes;
    unsigned char character[256];
    int size;
    int max_size;
} Huffman_code_dict;

typedef struct header{
    int encoded_str_len;
    int structure_len;
    unsigned char* info;
}  Header;


void insert_huffman_code(Huffman_code_dict* huffman_code_dict, char* code, unsigned char character){
    int i = huffman_code_dict->size;
    huffman_code_dict->codes[i] = malloc(sizeof(char*) * strlen(code) + 1);
    huffman_code_dict->character[i] = character;
    strcpy(huffman_code_dict->codes[i], code);
    huffman_code_dict->size ++;
}

void insert_element(Heap* heap, Huffman_Node* node){
    int i;
    for (i = ++heap->size; heap->arr[i/2]->frequency > node->frequency; i /= 2)
        heap->arr[i] = heap->arr[i/2];
    heap->arr[i] = node;
}

Heap* init_Heap (int max){
    Heap *heap = malloc(sizeof(Heap));
    heap -> capacity = max;
    heap -> size = 0;
    heap -> arr = malloc(sizeof(Huffman_Node*) * (heap ->capacity + 1));
    Huffman_Node *zero_node = malloc(sizeof(Huffman_Node));
    zero_node->frequency = 0;
    heap->arr[0] = zero_node;
    return heap;
}

Huffman_Node* extract_min_element(Heap* heap){
    if (heap -> size == 0) {
        return NULL;
    }
    Huffman_Node* fisrt_element = heap -> arr[1];
    Huffman_Node* last_element = heap -> arr[heap -> size];
    heap -> arr[1] = last_element;
    heap ->size--;
    int x = 1,y = 2;
    while (y <= heap -> size) {
        
        if (y + 1 <= heap -> size && heap ->arr[y+1]->frequency < heap -> arr[y]->frequency ) {
            y ++;
        }
        if (last_element->frequency < heap -> arr[y]->frequency) {
            break;
        }
        heap->arr[x] = heap->arr[y];
        x = y;
        y = 2 * x;
    }
    heap->arr[x] = last_element;
    return fisrt_element;
}

Huffman_Node pop_stack(Stack* stack){
    stack->index --;
    return stack->data[stack->index+1];
}

void push_stack(Stack* stack, Huffman_Node* node){
    stack->index ++;
    stack->data[stack->index] = *node;
}

void minus_one(char* minus_arr,int* arr, int size){
    int i;
    for (i= 0; i < size; i ++) {
        if (arr[i] -1 == 0) {
            minus_arr[i] = 48;
        } else {
            minus_arr[i] = 49;
        }
    }
}

void reconstruct_huffman_tree(unsigned char* arr, Huffman_Node* tree, int structure_length){
    Huffman_Node** node_stack = malloc(sizeof(Huffman_Node*) * 260);
    tree->left_child = NULL;
    tree->right_child = NULL;
    int i = 0;
    int index = 1;
    node_stack[0] = tree;
    while (i < structure_length) {
        if (arr[i] == '\0') {
            Huffman_Node* middle_node = malloc(sizeof(Huffman_Node));
            middle_node->left_child = NULL;
            middle_node->right_child = NULL;
            if (node_stack[index - 1]->left_child == NULL) {
                node_stack[index - 1]->left_child = middle_node;
            } else {
                node_stack[index - 1]->right_child = middle_node;
            }
            node_stack[index] = middle_node;
            i ++;
            index++;
        } else {
            Huffman_Node* leave_node = malloc(sizeof(Huffman_Node));
            leave_node->character = arr[i + 1];
            leave_node->left_child = NULL;
            leave_node->right_child = NULL;
            if (node_stack[index - 1] ->left_child ==NULL) {
                node_stack[index - 1]->left_child = leave_node;
            } else{
                node_stack[index - 1]->right_child = leave_node;
            }
            while (index > 0) {
                if (node_stack[index - 1]->left_child && node_stack[index - 1]->right_child) {
                    node_stack[index] = NULL;
                    index--;
                } else {
                    break;
                }
            }
            i += 2;
        }
    }
}


int generate_huffman_tree_structure(unsigned char* structure,Huffman_Node* tree){
    Stack stack;
    stack.index = 0;
    //    char structure[1020] = {0};
    int length = -1;
    if (tree != NULL) {
        while (stack.index != 0 || tree) {
            while (tree) {
                push_stack(&stack, tree);
                char c = tree->character;
                tree = tree->left_child;
                if (tree) {
                    if (length == -1) {
                        length++;
                        continue;
                    }
                    structure[length] = 0;
                    length ++;
                } else {
                    structure[length] = 1;
                    length ++;
                    structure[length] = c;
                    length ++;
                }
            }
            Huffman_Node node = pop_stack(&stack);
            tree = node.right_child;
        }
    }
    return length == -1 ? 0 : length;
}

Huffman_Node* merge_huffman_tree(Heap* heap){
    int x = heap->size;
    if (x == 0) {
        heap->arr[1] = NULL;
    } else {
        int i;
        for ( i= 1; i <= x - 1; i++) {
            Huffman_Node* node_1 = extract_min_element(heap);
            Huffman_Node* node_2 = extract_min_element(heap);
            
            Huffman_Node* merge_node = malloc(sizeof(Huffman_Node));
            merge_node->frequency = node_1->frequency + node_2->frequency;
            merge_node->left_child = node_1;
            merge_node->right_child = node_2;
            insert_element(heap, merge_node);
        }
    }
    return heap->arr[1];
}

void encode(Huffman_code_dict* huffman_code_dict,unsigned char c, unsigned char* bitarray, int* count, FILE *fp_out, int* times, int *length){
    int i;
    for ( i= 0; i < huffman_code_dict->size; i ++) {
        if (huffman_code_dict->character[i] == c) {
            int j;
            for (j = 0;huffman_code_dict->codes[i][j] != '\0'; j++) {
                int row = (*count)/8;
                int move_bits = (*count) % 8;
                if(huffman_code_dict->codes[i][j] == '1'){
                    bitarray[row] = ((0x80 >> move_bits) | bitarray[row]);
                }
                (*count) ++;
                if (*count == 8000) {
                    fwrite(bitarray, sizeof(unsigned char),1000, fp_out);
                    memset(bitarray, 0, 1000);
                    *count = 0;
                    (*times) ++;
                }
            }
        }
    }
}


Huffman_Node* decode(Huffman_Node* tree,Huffman_Node* temp_tree,
                     unsigned char* encoded_string,int *encoded_str_length,
                     int *decode_str_len,unsigned char *decoded_string,
                     FILE* fp){
    int index = 0;
    int row = 0;
    int m = 0;
    unsigned char str[17] = {0};
    while (index < 16 && *encoded_str_length > 0) {
        str[index] = ((0x80 >> m) & encoded_string[row]) == 0?48:49;
        if (str[index] == 48) {
            temp_tree = temp_tree->left_child;
        } else {
            temp_tree = temp_tree->right_child;
        }
        
        if (temp_tree->left_child == NULL && temp_tree->right_child == NULL) {
            decoded_string[*decode_str_len] = temp_tree->character;
            (*decode_str_len) ++;
            
            temp_tree = tree;
            if ((*decode_str_len) == 100) {
                fwrite(decoded_string, sizeof(unsigned char), 100, fp);
                int i;
                for (i = 0; i < 100; i ++) {
                    decoded_string[i] = 48;
                }
                *decode_str_len = 0;
            }
        }
        index ++;
        m ++;
        if (m == 8) {
            m = 0;
            row ++;
        }
        (*encoded_str_length) --;
    }
    return temp_tree;
}

void last_occurrence(int* alphabet_last_occurrence,char* p_chars, int p_length){
    int i;
    for (i = 0 ; i < p_length; i ++) {
        char s = p_chars[i];
        alphabet_last_occurrence[s] = i;
    }
}

Huffman_Node* search_decode(Huffman_Node* tree,
                            Huffman_Node* temp_tree,
                            unsigned char* encoded_string,
                            int *original_char_len,
                            int *encoded_str_length,
                            int *decode_str_len,
                            unsigned char *buffer, unsigned char *rest_str,
                            int* rest_buffer_count){
    int index = 0;
    int row = 0;
    int m = 0;
    unsigned char str[17] = {0};
    int full_flag = 0;
    while (index < 16 && *encoded_str_length > 0 && full_flag == 0) {
        str[index] = ((0x80 >> m) & encoded_string[row]) == 0?48:49;
        if (str[index] == 48) {
            temp_tree = temp_tree->left_child;
        } else {
            temp_tree = temp_tree->right_child;
        }
        if (temp_tree->left_child == NULL && temp_tree->right_child == NULL) {
            buffer[*decode_str_len] = temp_tree->character;
            (*decode_str_len) ++;
            (*original_char_len) --;
            temp_tree = tree;
        }
        index ++;
        m ++;
        if (m == 8) {
            m = 0;
            row ++;
        }
        (*encoded_str_length) --;
        if (*original_char_len == 0 && *encoded_str_length > 0) {
            if (index < 16) {
                (*rest_buffer_count) = 16 - index;
                if (*rest_buffer_count > *encoded_str_length) {
                    *rest_buffer_count = *encoded_str_length;
                }
                int loop;
                for (loop = 0; loop < *rest_buffer_count; loop++) {
                    rest_str[loop] = ((0x80 >> m) & encoded_string[row]) == 0?48:49;
                    m ++;
                    if (m == 8) {
                        m = 0;
                        row ++;
                    }
                }
            }
            full_flag = 1;
            break;
        }
    }
    return temp_tree;
}

unsigned char fgetc_buffer(unsigned char* buffer, 
                           int* current_buffer_index, 
                           unsigned char*rest_buffer, 
                           int *rest_buffer_count, 
                           int *current_rest_buffer_index, 
                           Huffman_Node* tree,
                           int search_length,
                           FILE* fp_search,
                           int* encoded_string_length){
    unsigned char c = {0};
    Huffman_Node* temp_tree = tree;  
    if (*encoded_string_length == 0 && buffer[0] == '\0') {
        return '\0';
    }
    if (*current_buffer_index < ((search_length + 1) * 3)){
        int index = *current_buffer_index;
        c = buffer[index];
        (*current_buffer_index) ++;
        return c;
    }
    int found_leave = 0;
    memset(buffer, 0, ((search_length+1) * 3));
    if (*current_rest_buffer_index < *rest_buffer_count) {
        (*current_buffer_index) = 0;
        int i;
        for (i = *current_rest_buffer_index; i < *rest_buffer_count && rest_buffer[i] != '\0'; i ++) {
            if (rest_buffer[i] == 48) {
                tree = tree->left_child;
            } else {
                tree = tree->right_child;
            }
            (*current_rest_buffer_index) ++;
            (*encoded_string_length) --;
            if (tree->left_child == NULL && tree->right_child == NULL) {
                buffer[found_leave] = tree->character;
                found_leave ++;
                if (found_leave == (search_length + 1) * 3) {
                    (*current_buffer_index) = 1;
                    return buffer[0];
                }
                tree = temp_tree;
            }
        }
    }
    (*current_rest_buffer_index) = 0;
    memset(rest_buffer, 0,  17);
    while (found_leave < ((search_length+1) * 3) && *encoded_string_length > 0){
        unsigned char* new_bytes = malloc(sizeof(unsigned char) * 2);
        (*current_buffer_index) = 0;
        fread(new_bytes, sizeof(unsigned char), 2, fp_search);
        int index = 0;
        int row = 0;
        int m = 0;
        unsigned char str[16] = {0};
        while (index < 16 && *encoded_string_length > 0) {
            str[index] = ((0x80 >> m) & new_bytes[row]) == 0?48:49;
            if (str[index] == 48) {
                tree = tree->left_child;
            } else {
                tree = tree->right_child;
            }
            if (tree->left_child == NULL && tree->right_child == NULL) {
                buffer[found_leave++] = tree->character;
                tree = temp_tree;
            }
            index ++;
            m ++;
            if (m == 8) {
                m = 0;
                row ++;
            }
            (*encoded_string_length) --;
            if (found_leave == ((search_length+1) * 3)) {
                (*rest_buffer_count) = 16 - index;
                if (*rest_buffer_count > *encoded_string_length) {
                    *rest_buffer_count = *encoded_string_length;
                }
                (*current_rest_buffer_index) = 0;
                int loop;
                for (loop = 0 ; loop < *rest_buffer_count; loop++) {
                    rest_buffer[loop] = ((0x80 >> m) & new_bytes[row]) == 0?48:49;
                    m ++;
                    if (m == 8) {
                        m = 0;
                        row ++;
                    }
                }
                break;
            }
        }
    }
    c = buffer[0];
    (*current_buffer_index) ++;
    return c;
}


int search(unsigned char **t_chars,char* p_chars, int search_length,
           FILE* fp_search,unsigned char *buffer,int *current_buffer_index,
           unsigned char *rest_buffer,int *rest_buffer_index,int *current_rest_buffer_index,
           Huffman_Node* tree, int* encoded_string_length){
    int alphabet_last_occurrence[257] = {-1};
    int window_size = search_length;
    memset(alphabet_last_occurrence, -1, sizeof(alphabet_last_occurrence));
    last_occurrence(alphabet_last_occurrence, p_chars, search_length);
    int total_original_number = search_length * 2;
    search_length --;
    
    int p_index = search_length; 
    int t_index = p_index; 
    int times = 0;
    int found = 0;
    int row = 0; 
    int column = search_length; 
    int distance = 0; 
    unsigned char current_char = 0; 
    int replace_position = 0;
    int number_of_replace_char = 0;
    int over_index = 0;
    while (t_index < total_original_number && t_chars[row][column] != '\0') {
        while (t_chars[row][column] == p_chars[p_index]) {
            t_index--;
            row = (t_index -1 + 1 + window_size) / window_size - 1;
            row = row % 2;
            column = t_index % window_size;
            p_index--;
            if (p_index == -1) {
                times++;
                p_index = search_length;
                int i;
                for (i = 0 ; i < 1; i++) {
                    int replace_row = (replace_position + 1 - 1 + window_size) / window_size - 1;
                    replace_row %= 2;
                    int replace_column = replace_position % window_size;
                    if((current_char = fgetc_buffer(buffer,current_buffer_index,rest_buffer,rest_buffer_index,current_rest_buffer_index,tree, search_length, fp_search,encoded_string_length)) != '\0'){
                        t_chars[replace_row][replace_column] = current_char;
                        total_original_number ++;
                    } else {
                        t_chars[replace_row][replace_column] = '\0';
                        over_index = t_index;
                        break;
                    }
                    replace_position++;
                }
                t_index = t_index + search_length + 2;
                row = (t_index + 1 - 1 + window_size) / window_size - 1;
                row %= 2;
                column = t_index % window_size;
                found = 1;
            }
        }
        if(found == 0 && t_chars[row][column] != '\0'){
            int last_occurrence = alphabet_last_occurrence[t_chars[row][column]];
            if (last_occurrence != -1){
                if (last_occurrence < p_index){
                    distance = search_length - alphabet_last_occurrence[t_chars[row][column]];
                    t_index += distance;
                    number_of_replace_char = distance;
                    p_index = search_length;
                } else {
                    distance = (search_length + 1) - p_index;
                    p_index = search_length;
                    t_index = t_index + distance;
                    number_of_replace_char = 1;
                }
            } else { 
                distance = search_length + 1;
                t_index += distance;
                number_of_replace_char = p_index + 1;
                p_index = search_length;
            }
            int i;
            for (i = 0 ; i < number_of_replace_char; i++) {
                int replace_row = (replace_position -1 + 1 + window_size) / window_size - 1;
                replace_row %= 2;
                int replace_column = replace_position % window_size;
                if((current_char = fgetc_buffer(buffer,current_buffer_index,rest_buffer,rest_buffer_index,current_rest_buffer_index,tree, search_length, fp_search,encoded_string_length)) != '\0'){
                    t_chars[replace_row][replace_column] = current_char;
                    total_original_number ++;
                } else {
                    t_chars[replace_row][replace_column] = '\0';
                    over_index = t_index;
                    break;
                }
                replace_position ++;
            }
            row = (t_index + 1 - 1 + window_size) / window_size - 1;
            row %= 2;
            column = t_index % window_size;
        }
        found = 0;
    }
    return times;
}



void generate_huffman_dict(Huffman_code_dict* huffman_code_dict,unsigned char* codes,int length){
    int flag[257] = {0};
    flag[0] = 1;
    int index = 1;
    int i = 0;
    int count = 0;
    while (i < length){ 
        if (codes[i] == '\0') {
            flag[index] +=1;
            index ++;
            i ++;
        } else {
            char minus_arr[260] = {0};
            minus_one(minus_arr,flag, index);
            insert_huffman_code(huffman_code_dict,minus_arr, codes[i+1]);
            while (flag[--index] == 2) {
                flag[index] = 0;
            }
            flag[index] ++; 
            index ++; 
            i+=2;
            count++;
        }
    }
}


void init_huffman_code(Huffman_code_dict* huffman_code_and_char){
    huffman_code_and_char->max_size = 256;
    huffman_code_and_char->size = 0;
    huffman_code_and_char->codes = malloc(sizeof(char*) * 256);
}

int main(int argc, char* argv[]){
    if (strcmp(argv[1],"-e") == 0) {
        FILE *fp;
        Heap *heap = init_Heap(260);
        int i;
        fp = fopen(argv[2], "rb");
        if (fp == NULL) {
            return (-1);
        }
        FILE *fp_out;
        fp_out = fopen(argv[3], "wb");
        int c;
        int char_count = 0;
        int char_table[260] = {0};
        while ((c = fgetc(fp)) != EOF) {
            char_table[c]++;
            char_count ++;
        }
        fclose(fp);
        Huffman_Node *node = malloc(sizeof(Huffman_Node));
        int alphabet_number = 0;
        for (i = 0; i < 256; i++) {
            if (char_table[i] != 0) {
                alphabet_number ++;
                node = malloc(sizeof(Huffman_Node));
                node->left_child = NULL;
                node->right_child = NULL;
                node->frequency = char_table[i];
                node->character = i;
                insert_element(heap, node);
            }
        }
        int huffman_structure_length;
        unsigned char bitarray[1000] = {0};
        
        if (alphabet_number == 1) {
            huffman_structure_length = 2;
            unsigned char single_struc[1016] = {0};
            single_struc[0] = 49;
            single_struc[1] = node->character;
            int stru_size = 2;
            fwrite(&node->frequency, sizeof(int), 1, fp_out);
            fwrite(&stru_size,sizeof(int) , 1, fp_out);
            fwrite(&single_struc, sizeof(unsigned char),1016, fp_out);
            int bytes = (node->frequency - 1 + 8) / 8;
            unsigned char *encoded_string = malloc(sizeof(unsigned char) * (bytes + 1));
            memset(encoded_string, '\0', bytes);
            fwrite(encoded_string, sizeof(unsigned char), bytes, fp_out);
            fclose(fp_out);
            fclose(fp);
        } else {
            Huffman_Node* tree = merge_huffman_tree(heap);
            unsigned char huffman_tree_structure[1020] = {0};
            
            huffman_structure_length = generate_huffman_tree_structure(huffman_tree_structure,tree);
            Huffman_code_dict huffman_code_dict;
            init_huffman_code(&huffman_code_dict);
            generate_huffman_dict(&huffman_code_dict,huffman_tree_structure,huffman_structure_length);
            int padding = 0;
            fwrite(&padding, sizeof(int),1, fp_out);
            fwrite(&huffman_structure_length, sizeof(int), 1, fp_out);
            fwrite(&huffman_tree_structure, sizeof(unsigned char),1016, fp_out);
            fp = fopen(argv[2], "rb");
            int count = 0;
            int times = 0;
            int length = 0;
            while ((c = fgetc(fp)) != EOF) {
                encode(&huffman_code_dict, c, bitarray, &count,fp_out, &times, &length);
            }
            if (count != 0) { 
                int rest_bytes = (count + 8 - 1) / 8;
                times = times * 8000 + count;
                fwrite(bitarray, sizeof(unsigned char), rest_bytes, fp_out);
            }
            
            fseek(fp_out, 0, SEEK_SET);
            fwrite(&times, sizeof(int), 1, fp_out);
            fclose(fp_out);
        }
    }

    else if (strcmp(argv[1],"-d") == 0) {
        int *structure_length = malloc(sizeof(int));
        int *encoded_string_length = malloc(sizeof(int));
        FILE* fp;
        FILE* fp_out;
        fp = fopen(argv[2], "rb");
        fp_out = fopen(argv[3],"wb");
        fread(encoded_string_length, sizeof(int), 1, fp);
        fread(structure_length, sizeof(int), 1, fp);
        unsigned char* structure = malloc(sizeof(unsigned char) * (*structure_length + 1));
        fread(structure, sizeof(unsigned char), *structure_length, fp);
        int rest = 1024 - 8 - *structure_length;
        fseek(fp, rest, SEEK_CUR);
        int bytes_of_encoded_string = *encoded_string_length == 0 ? 0 : ((*encoded_string_length + 8 - 1) / 8);
        Huffman_Node top_node;
        reconstruct_huffman_tree(structure,&top_node, *structure_length);
        Huffman_Node *tem_tree = malloc(sizeof(Huffman_Node));
        tem_tree = &top_node;
        unsigned char read_buffer[2];
        int decode_str_len = 0;
        unsigned char decode_char_list[100] = {0};
        if (*structure_length == 2)
        {
            unsigned char *single_char_output = malloc(sizeof(unsigned char) * (*encoded_string_length + 1));
            memset(single_char_output, structure[1], *encoded_string_length);
            fwrite(single_char_output,sizeof(unsigned char), *encoded_string_length, fp_out);
        } else {
            int x;
            for (x = 0;x < bytes_of_encoded_string; x ++) {
                fread(read_buffer, sizeof(unsigned char), 2, fp);
                Huffman_Node* r = decode(&top_node, tem_tree,
                                         read_buffer,
                                         encoded_string_length,&decode_str_len,
                                         decode_char_list,fp_out);
                tem_tree = r;
            }
            if (decode_str_len != 0) {
                fwrite(decode_char_list, sizeof(unsigned char), decode_str_len, fp_out);
            }
        }
        fclose(fp);
        fclose(fp_out);
    }

    else if(strcmp(argv[1],"-s") == 0) {
        FILE* fp;
        fp = fopen(argv[3], "rb");
        int *structure_length = malloc(sizeof(int));
        int *encoded_string_length = malloc(sizeof(int));
        fread(encoded_string_length, sizeof(int), 1, fp);
        fread(structure_length, sizeof(int), 1, fp);
        unsigned char* structure = malloc(sizeof(unsigned char) * (*structure_length));
        fread(structure, sizeof(unsigned char), *structure_length, fp);
        int rest = 1024 - 8 - *structure_length;
        fseek(fp, rest, SEEK_CUR);
        int bytes_of_encoded_string = *encoded_string_length == 0 ? 0 : ((*encoded_string_length + 8 - 1) / 8);
        Huffman_Node top_node;
        reconstruct_huffman_tree(structure,&top_node, *structure_length);
        Huffman_Node *tem_tree = malloc(sizeof(Huffman_Node));
        tem_tree = &top_node;
        unsigned char read_buffer[2];
        int decode_str_len = 0;
        int search_length;
        search_length = (int)strlen(argv[2]);
        int original_char_len = search_length * 3;
        unsigned char *buffer = malloc(sizeof(unsigned char) * (search_length * 3 + 1));
        unsigned char *rest_buffer = malloc(sizeof(unsigned char) * 18);
        memset(rest_buffer, 0,17);
        int rest_buffer_count = 0;
        int copy_encode_string_length = *encoded_string_length;
        while (bytes_of_encoded_string > 0 && encoded_string_length > 0 && original_char_len > 0) {
            fread(read_buffer, sizeof(unsigned char), 2, fp);
            Huffman_Node* r = search_decode(&top_node, tem_tree,
                                            read_buffer,&original_char_len,
                                            encoded_string_length,&decode_str_len,
                                            buffer, rest_buffer,&rest_buffer_count);
            tem_tree = r;
            bytes_of_encoded_string -=2;
        }
        if (copy_encode_string_length == search_length) {
            int i;
            for (i = 0; i < search_length; i++) {
                if ((char)buffer[i] != argv[2][i]) {
                    printf("0\n");
                    return 0;
                }
            }
            printf("1\n");
            return 0;
        }
        unsigned char **original_txt_buffer = malloc(sizeof(unsigned char*) * 2);
        unsigned char *string_1 = malloc(sizeof(unsigned char) * (search_length + 1));
        unsigned char *string_2 = malloc(sizeof(unsigned char) * (search_length + 1));
        int buffer_count = 0;
        int j;
        for (j = 0; j < search_length && buffer[j] != '\0'; j++) {
            string_1[j % search_length] = buffer[j];
            buffer_count ++;
        }
        for (j = search_length; j <search_length * 2 && buffer[j] != '\0'; j++) {
            buffer_count++;
            string_2[j % search_length] = buffer[j];
        }
        if (buffer_count < search_length) {
            printf("0\n");
            return 0;
        }
        original_txt_buffer[0] = string_1;
        original_txt_buffer[1] = string_2;
        int *current_buffer_index = malloc(sizeof(int));
        *current_buffer_index = search_length * 2;
        int *current_rest_buffer_index = malloc(sizeof(int));
        *current_rest_buffer_index = 0;
        int times = search(original_txt_buffer, argv[2], search_length, fp,
                           buffer,current_buffer_index,rest_buffer,&rest_buffer_count,current_rest_buffer_index,&top_node,encoded_string_length);
        fclose(fp);
        printf("%d\n",times);
    }
    return 0;
}