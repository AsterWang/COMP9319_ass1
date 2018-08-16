#include "9319_Ass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN -1

typedef struct huffman_node{
    char   character;
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
    char* character;
    int size;
    int max_size;
} Huffman_code_dict;


void insert_huffman_code(Huffman_code_dict* huffman_code_dict, char* code, char character){
    int i = huffman_code_dict->size;
    huffman_code_dict->codes[i] = malloc(sizeof(char*) * strlen(code) + 1);
    huffman_code_dict->character[i] = character;
    strcpy(huffman_code_dict->codes[i], code);
    huffman_code_dict->size ++;
}


void insert_element(Heap* heap, Huffman_Node* node){
    int i;
//    heap->arr[i] = node;
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
//    zero_node->character
//    zero_node->character = "#";
    heap->arr[0] = zero_node;
//    heap->size++;
    return heap;
}

void print_heap(Heap* heap) {
    for (int i = 1; i <= heap->size; i++)
        printf("Element[%d] = %2d    character is : %c\n", i, heap->arr[i]->frequency, heap->arr[i]->character);
}



// 1.we need to swap the first element and the last element
// 2.then we pop down the new fisrt element to the right place.
Huffman_Node* extract_min_element(Heap* heap){
    if (heap -> size == 0) {
        printf("heap has been NULL");
        exit(0);
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
    if (stack->index == -1) {
        printf("the stack has been null!");
        exit(0);
    }
    stack->index --;
    return stack->data[stack->index+1];
}

void push_stack(Stack* stack, Huffman_Node* node){
    if (stack->index == 256) {
        printf("the stack has been full!");
        exit(0);
    }
    stack->index ++;
    stack->data[stack->index] = *node;
}

void print_stack(Stack* stack){
    for (int i = 1; i <= stack->index; i++) {
        printf("stack[%d] is %d\n", i, stack->data[i].frequency);
    }
}

void print_huffman_code(int stack[], int length){
    for (int i = 0; i <= length; i ++) {
        printf("%d", stack[i] - 1);
    }
//    printf("/n");
}

char* minus_one(int* arr, int size){
    char* minus_arr = malloc(sizeof(char) * size);
    for (int i = 0; i < size; i ++) {
        if (arr[i] -1 == 0) {
            minus_arr[i] = 48;
        } else {
            minus_arr[i] = 49;
        }
    }
    return minus_arr;
}
/**
 Reconstruct the huffman tree based on huffman tree constructure
 and generate the huffman code dictionary
 */
void generate_huffman_dict(Huffman_code_dict* huffman_code_dict,char* codes,int length){
    int flag[257] = {0};
    flag[0] = 1;
    int index = 1;
    int i = 0;
//    Huffman_code* h;
    int count = 0;
    while (i < length){ // i < length
        //code[i] = 0 , means not meet the leave node yet.说明还没到叶节点
        if (codes[i] == '\x0') {
            flag[index] +=1;
            index ++;
            i ++;
        } else {
//            h = malloc(sizeof(Huffman_code));
//            h->codes = malloc(sizeof(int) * 256);
            for (int i = 0; i < index; i ++) {
                int x = flag[i] - 1;
//                h->codes[i]= x ;
                printf("%d", x);
            }
            insert_huffman_code(huffman_code_dict,minus_one(flag, index), codes[i+1]);
//            h->character = codes[i+1];
            printf("    %c\n", codes[i+1]);
            while (flag[--index] == 2) {
                flag[index] = 0;
            }
            flag[index] ++; // 节点 + 1， 说明访问次数 + 1
            index ++; // loop the right child. 访问子树的右节点
            i+=2;
//            huffman_code_and_char[count] = h;
            count++;
        }
    }
    printf("\n");
}

/**
 Generate the structure of huffman tree
 */
int generate_huffman_tree_structure(char* structure,Huffman_Node* tree, FILE* fp){
    //preorder the tree to produce the huffman code;
    Stack stack;
    stack.index = 0;
//    char structure[1020] = {0};
    int length = -1;
    if (tree) {
        while (stack.index != 0 || tree) {
            while (tree) {
//                printf("the frequency is : %d, character is %c\n", tree->frequency,tree->character);
                push_stack(&stack, tree);
                char c = tree->character;
                tree = tree->left_child;
                if (tree) {
//                    index++;
                    
                    if (length == -1) {
                        length++;
                        continue;
                    }
                    printf("0");
                    structure[length] = 0;
                    length ++;
                } else {
                    structure[length] = 1;
                    length ++;
                    structure[length] = c;
                    length ++;
                    printf("1");
                    printf("%c",c);
                }
            }
            Huffman_Node node = pop_stack(&stack);
            tree = node.right_child;
        }
        printf("\n");
    }
//    fwrite(&length, sizeof(int), 1, fp);
//    fwrite(structure, sizeof(char),1020,fp);
//    return &structure;
    return length;
}

Huffman_Node* merge_huffman_tree(Heap* heap){
    int x = heap->size;
    for (int i = 1; i <= x - 1; i++) {
        Huffman_Node* node_1 = extract_min_element(heap);
        Huffman_Node* node_2 = extract_min_element(heap);
        
        Huffman_Node* merge_node = malloc(sizeof(Huffman_Node));
        merge_node->frequency = node_1->frequency + node_2->frequency;
//        merge_node->character = ";
        merge_node->left_child = node_1;
        merge_node->right_child = node_2;
        insert_element(heap, merge_node);
    }
    return heap->arr[1];
}

void add_bit(char* bitarray, int k){
    int i = k / 8;
    int j = k % 8;
    bitarray[i] = ((0x80 >> j) | (bitarray[i]));
}

void encode(Huffman_code_dict* huffman_code_dict,int c, char* bitarray, int* count, int* extend_size){
    for (int i = 0; i < huffman_code_dict->size; i ++) {
        if (huffman_code_dict->character[i] == c) {
            for (int j = 0; huffman_code_dict->codes[i][j] != '\0'; j++) {
                int row = *count / 8;
                int move_bits = (*count % 8) ;
                
                //if bitarray already full, enlarge its size.
                if (*count == (*extend_size * 8) && *count != 0) {
//                    char* new_array;
//                    bitarray = malloc(sizeof(char) * 2);
                    *extend_size = (*extend_size) * 2;
                    bitarray = realloc(bitarray, *extend_size * sizeof(char));
//                    bitarray = new_array;
                }
                printf("%c",huffman_code_dict->codes[i][j]);
                if(huffman_code_dict->codes[i][j] == 49){
                    bitarray[row] = ((0x80 >> move_bits) | bitarray[row]);
                }
                (*count) ++;
            }
//            printf("\n");
        }
    }
    printf("\n");
}

void decode(char* arr, Huffman_code_dict* huffman_code_dict){
    
}

void init_huffman_code(Huffman_code_dict* huffman_code_and_char){
    huffman_code_and_char->max_size = 256;
    huffman_code_and_char->size = 0;
    huffman_code_and_char->codes = malloc(sizeof(char*) * 256);
    huffman_code_and_char->character = malloc(sizeof(char) * 256);
}


//int main(){
//    char codes[] = "01A01G1H001C1D1E";
//    int length = sizeof(codes) / sizeof(char);
//    recover_huffman_tree(codes,length);
//    return 0;
//}

int main(){
    FILE *fp;
    Heap *heap = init_Heap(256);
    int i;
    fp = fopen("/Users/wangeric/Documents/study/9319/ass1/test.txt", "r");
    if (fp == NULL) {
        perror("Error in opening file");
        return (-1);
    }
    int c;
    int char_table[256] = {0};
    while ((c = fgetc(fp)) != EOF) {
        char_table[c]++;
    }
    fclose(fp);
    Huffman_Node *node;
    for (i = 0; i < 256; i++) {
        if (char_table[i] != 0) {
            node = malloc(sizeof(Huffman_Node));
            node->left_child = NULL;
            node->right_child = NULL;
            node->frequency = char_table[i];
            node->character = i;
            insert_element(heap, node);
        }
    }

    Huffman_Node* tree = merge_huffman_tree(heap);
    char huffman_tree_structure[1020] = {0};
    
    //generate huffman structure
    int huffman_structure_length = generate_huffman_tree_structure(huffman_tree_structure,tree,fp);
    
    
    //get huffman code dictionary by huffman-tree's structure.
    Huffman_code_dict huffman_code_dict;
    init_huffman_code(&huffman_code_dict);
    generate_huffman_dict(&huffman_code_dict,huffman_tree_structure,huffman_structure_length);
    
    //encode txt file
    int* extend_size = malloc(sizeof(int));
    *extend_size = 3;
    char *bitarray = malloc(sizeof(char) * 3);
 
//===================================ENCODE======================================================
    int count = 0;
    fp = fopen("/Users/wangeric/Documents/study/9319/ass1/test.txt", "r");
    while ((c = fgetc(fp)) != EOF) {
        encode(&huffman_code_dict, c, bitarray, &count, extend_size);
    }
    printf("total count is : %d\n", count);
    int rest_bits_number = (*extend_size) - (*extend_size * 8 - count) / 8;
    fp = fopen("/Users/wangeric/Documents/study/9319/ass1/code_test.huffman", "wb");
    fwrite(&huffman_structure_length, sizeof(int), 1, fp);
    fwrite(huffman_tree_structure, sizeof(char), 1020, fp);
    fwrite(bitarray, sizeof(char), rest_bits_number, fp);
    fclose(fp);
//================================================================================================
    
    
//===============================================DECODE============================================
    int *L = malloc(sizeof(int));
    fp = fopen("/Users/wangeric/Documents/study/9319/ass1/code_test.huffman", "r");
    fread(L, sizeof(int), 1, fp);
    char* arr = malloc(sizeof(char) * (*L));
    fread(arr, sizeof(char), *L, fp);
    int rest = 1020 - *L;
    char* rest_arr = malloc(sizeof(char) * rest);
    fread(rest_arr, sizeof(char), rest, fp);
    
    char* encode_string = malloc(sizeof(char) * 2);
    fread(encode_string, sizeof(char), 2, fp);
    
    //get huffman code dictionary by huffman-tree's structure.
    Huffman_code_dict huffman_code_dict_2;
    init_huffman_code(&huffman_code_dict_2);
    generate_huffman_dict(&huffman_code_dict_2,arr,*L);
    decode(arr, &huffman_code_dict_2);
//==================================================================================================
    return 0;
}
