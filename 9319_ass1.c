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


void insert_huffman_code(Huffman_code_dict* huffman_code_dict, char* code, unsigned char character){
//    printf("code is %s\n", code);
    int i = huffman_code_dict->size;
    huffman_code_dict->codes[i] = malloc(sizeof(char*) * strlen(code) + 1);
    huffman_code_dict->character[i] = character;
    printf("insert codes are %s\n", code);
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
//        exit(0);
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

void minus_one(char* minus_arr,int* arr, int size){
    for (int i = 0; i < size; i ++) {
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


/**
 Generate the structure of huffman tree
 */
int generate_huffman_tree_structure(unsigned char* structure,Huffman_Node* tree){
    //preorder the tree to produce the huffman code;
    Stack stack;
    stack.index = 0;
    //    char structure[1020] = {0};
    int length = -1;
    if (tree != NULL) {
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
    return length == -1 ? 0 : length;
}

Huffman_Node* merge_huffman_tree(Heap* heap){
    int x = heap->size;
    if (x == 0) {
        heap->arr[1] = NULL;
    } else {
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
    }
    printf("merge length of huffman tree structure  is %d\n",heap->arr[1]->frequency);
    return heap->arr[1];
}
/**
 每个bitarray有十个bytes, 都初始化为0
 如果huffman字典中有和参数字符c匹配的字符，获取它的huffman code.
 逐位遍历匹配上的huffman code, 将每一个bytes上的bits设置成对应的huffman code
    .例如第一个huffman code = 101. 对应第一个byte上的bits修改为1000 0000 -> 1000 0000 -> 1010 0000。 以此类推。
 当次数=8时，对bitarray中的下一个bytes进行修改。直到次数=80，说明bitarray全部修改完成，写入文件并将biarray全部元素设置为0，重新设置。
 */
void encode(Huffman_code_dict* huffman_code_dict,unsigned char c, unsigned char* bitarray, int* count, FILE *fp_out, int* times, int *length){
//    printf("huffman dict length is :%d\n", huffman_code_dict->size);
//    int total_length = 0;
//    for (int i = 0; i < huffman_code_dict->size; i++) {
//        printf("codes are %s, length is %d\n",huffman_code_dict->codes[i], (int)strlen(huffman_code_dict->codes[i]));
//    }
//    printf("total codes are : %d\n",total_length);
//    printf("huffman dict codes are :%s\n", huffman_code_dict->character);
    for (int i = 0; i < huffman_code_dict->size; i ++) {
        //如果从字典中找到对应的character，遍历该位置上的huffman codes
        if (huffman_code_dict->character[i] == c) {
            for (int j = 0;huffman_code_dict->codes[i][j] != '\0'; j++) {
//                *length = (*length) + strlen(huffman_code_dict->codes[i]);
                int row = (*count)/8;
                int move_bits = (*count) % 8;
                if(huffman_code_dict->codes[i][j] == '1'){
                    bitarray[row] = ((0x80 >> move_bits) | bitarray[row]);
                }
                (*count) ++;
                //满80次， 即10个byte，写入文件，重置bitarray。
                if (*count == 8000) {
                    printf("write into file...\n");
                    fwrite(bitarray, sizeof(unsigned char),1000, fp_out);
                    memset(bitarray, 0, 1000);
                    *count = 0;
                    (*times) ++;
                }
            }
        }
    }
}

/**
 Decoding
 */
Huffman_Node* decode(Huffman_Node* tree,Huffman_Node* temp_tree,
            unsigned char* encoded_string,
            int bytes_of_encoded_str, int *encoded_str_length,
            int *decode_str_len,unsigned char *decoded_string,
            FILE* fp){
    int index = 0;
    int row = 0;
    int m = 0;
    unsigned char str[17] = {0};
//    Huffman_Node* temp_tree = tree;
//    int path_length_to_leave = 0;
//    char temp_char[257] = {0};
    
    //读进2个char,对2个char中的bit进行遍历，如果bit = 0，进入tree的左节点，如果bit = 1,进入tree的右节点。
    //如果中途到达叶节点，记录下叶节点的character,然后写进数组，同时将树重定位到根节点
    //如果中途到第16个bit都没有遇到叶节点，则保留住此时树的位置。下一次新的2个char进入时直接从记录的树节点开始遍历。
    while (index < 16 && *encoded_str_length > 0) {
        str[index] = ((0x80 >> m) & encoded_string[row]) == 0?48:49;
//        if (temp_tree->left_child == NULL || temp_tree->right_child == NULL) {
//            printf("%s","NULL!!!");
//        }
        if (str[index] == 48) {
            temp_tree = temp_tree->left_child;
        } else {
            temp_tree = temp_tree->right_child;
        }
      
        //如果if内容满足left_child 跟right_child 都为Null,说明到达叶子节点，即获得decode过的字符
        if (temp_tree->left_child == NULL && temp_tree->right_child == NULL) {
            if (temp_tree->character == 'l') {
                printf("%s\n","meet d");
            }
            decoded_string[*decode_str_len] = temp_tree->character;
            printf("%c",decoded_string[*decode_str_len]);
            (*decode_str_len) ++;
            
            temp_tree = tree;
            //如果decoded string数组满了，写入文件，将数组元素全部设置为0
            if ((*decode_str_len) == 100) {
                fwrite(decoded_string, sizeof(unsigned char), 100, fp);
                for (int i = 0; i < 100; i ++) {
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


//void search(unsigned char* original_chars, unsigned char* search_chars, int original_length){
//    int i = 0;
//    while (i < original_length) {
//        while (<#condition#>) {
//            <#statements#>
//        }
//    }
//
//}

/**
 Reconstruct the huffman tree based on huffman tree constructure
 and generate the huffman code dictionary
 */
void generate_huffman_dict(Huffman_code_dict* huffman_code_dict,unsigned char* codes,int length){
    int flag[257] = {0};
    flag[0] = 1;
    int index = 1;
    int i = 0;
    //    Huffman_code* h;
    int count = 0;
    while (i < length){ // i < length
        //code[i] = 0 , means not meet the leave node yet.说明还没到叶节点
        if (codes[i] == '\0') {
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
            char minus_arr[260] = {0};
            minus_one(minus_arr,flag, index);
            insert_huffman_code(huffman_code_dict,minus_arr, codes[i+1]);
            //            h->character = codes[i+1];
//            printf("    %c\n", codes[i+1]);
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

void init_huffman_code(Huffman_code_dict* huffman_code_and_char){
    huffman_code_and_char->max_size = 256;
    huffman_code_and_char->size = 0;
    huffman_code_and_char->codes = malloc(sizeof(char*) * 256 + 1);
//    huffman_code_and_char->character;
}

void add_encoded_str_len_info(int encoded_str_len, FILE* fd, FILE* fpTemp){
//    FILE* fd;
//    fd = fopen("/Users/wangeric/Documents/study/9319/ass1/code_test.huffman", "rb");
//    FILE* fpTemp;
//    fpTemp = fopen("/Users/wangeric/Documents/study/9319/ass1/code_test2.huffman", "wb");
    
    long Fin;
    long lSize;
    int position = 4;
    fseek(fd, 0L, SEEK_END);
    Fin = ftell(fd);//I get the old file size
    fseek(fd, position, SEEK_SET);//you should have declared and initialized "position"
    char * buffer;
    lSize = Fin - position;//lenght from where you want to insert the chars and the eof
    
    buffer = (char*) malloc(sizeof(unsigned char) * lSize);
    fread(buffer, 1, lSize, fd);//fread (Fin - position) into buffer
    fseek(fd, position, SEEK_SET);//fseek to position
    
//    int xx = 10;
    fwrite(&encoded_str_len, sizeof(int), 1, fpTemp);//you should know what you want to write (word) and it is a char pointer
    fwrite(buffer, sizeof(unsigned char), lSize, fpTemp);
    fclose(fd);
    fclose(fpTemp);
}


int main(){
    FILE *fp;
    Heap *heap = init_Heap(260);
    int i;
    fp = fopen("/Users/wangeric/Documents/study/9319/ass1/1234.png", "rb");
    if (fp == NULL) {
        perror("Error in opening file");
        return (-1);
    }
    int c;
    int char_table[260] = {0};
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
    unsigned char huffman_tree_structure[1020] = {0};
    
    //generate huffman structure
    int huffman_structure_length = generate_huffman_tree_structure(huffman_tree_structure,tree);
    printf("huffman_structure_length is :%d\n",huffman_structure_length);
    
    
    //get huffman code dictionary by huffman-tree's structure.
    Huffman_code_dict huffman_code_dict;
    init_huffman_code(&huffman_code_dict);
    //生成huffman code的字典
    generate_huffman_dict(&huffman_code_dict,huffman_tree_structure,huffman_structure_length);
    int total_length = 0;
    for (int i = 0; i < huffman_code_dict.size; i++) {
        printf("codes are %s, length is %d\n",huffman_code_dict.codes[i], (int)strlen(huffman_code_dict.codes[i]));
    }
    printf("total codes are : %d\n",total_length);
    unsigned char bitarray[1000] = {0};
    
//===================================ENCODE======================================================
    FILE *fp_out;
    fp_out = fopen("/Users/wangeric/Documents/study/9319/ass1/code_test.huffman", "wb");
    int b = 0;
    fwrite(&b, sizeof(int), 1, fp_out);
    fwrite(&huffman_structure_length, sizeof(int), 1, fp_out);
    fwrite(&huffman_tree_structure, sizeof(unsigned char),1016, fp_out);
    fp = fopen("/Users/wangeric/Documents/study/9319/ass1/1234.png", "rb");
    int count = 0;
    int times = 0;
//    int ss = 1;
//    printf("characters is :\n");
    int length = 0;
    while ((c = fgetc(fp)) != EOF) {
        encode(&huffman_code_dict, c, bitarray, &count,fp_out, &times, &length);
    }
//    printf("previous times is : %d\n", times * 8000);
//    printf("\n====================================\n");
    if (count != 0) { //如果有多余的bits,继续
//        printf("count is %d\n", count);
        int rest_bytes = (count + 8 - 1) / 8;
        times = times * 8000 + count;
//        printf("times is %d:\n", times);
        fwrite(bitarray, sizeof(unsigned char), rest_bytes, fp_out);
    }
//    printf("times is %d\n",times);
    fclose(fp_out);
    fp_out = fopen("/Users/wangeric/Documents/study/9319/ass1/code_test.huffman", "rb");
    FILE* fpTemp;
    fpTemp = fopen("/Users/wangeric/Documents/study/9319/ass1/code_test2.huffman", "wb");
    add_encoded_str_len_info(times, fp_out, fpTemp);
//================================================================================================
    
    
//===============================================DECODE============================================
    int *structure_length = malloc(sizeof(int));
    int *encoded_string_length = malloc(sizeof(int));

    fp = fopen("/Users/wangeric/Documents/study/9319/ass1/code_test2.huffman", "rb");
    fp_out = fopen("/Users/wangeric/Documents/study/9319/ass1/copy.png", "wb");
    
    
    fread(encoded_string_length, sizeof(int), 1, fp); //encode string 长度
    fread(structure_length, sizeof(int), 1, fp); //树结构长度
    unsigned char* structure = malloc(sizeof(unsigned char) * (*structure_length)); //树结构
    fread(structure, sizeof(unsigned char), *structure_length, fp);
    int rest = 1024 - 8 - *structure_length;
    fseek(fp, rest, SEEK_CUR);
    int bytes_of_encoded_string = *encoded_string_length == 0 ? 0 : ((*encoded_string_length + 8 - 1) / 8);
//    unsigned char* encoded_string = malloc(sizeof(unsigned char) * bytes_of_encoded_string); //encode string
    
    Huffman_Node top_node;
    reconstruct_huffman_tree(structure,&top_node, *structure_length);
    Huffman_Node *tem_tree = malloc(sizeof(Huffman_Node));
    tem_tree = &top_node;
    unsigned char read_buffer[2];
    int decode_str_len = 0;
    unsigned char decode_char_list[100] = {0};
    for (int x = 0;x < bytes_of_encoded_string - 1; x ++) {
        fread(read_buffer, sizeof(unsigned char), 2, fp);
        Huffman_Node* r = decode(&top_node, tem_tree,read_buffer, bytes_of_encoded_string, encoded_string_length,&decode_str_len,decode_char_list,fp_out);
        tem_tree = r;
    }
    if (decode_str_len != 0) {
        fwrite(decode_char_list, sizeof(unsigned char), decode_str_len, fp_out);
    }
    fclose(fp);
    //==================================================================================================
    return 0;
}
