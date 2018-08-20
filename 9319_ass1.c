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
//    printf("code is %s\n", code);
    int i = huffman_code_dict->size;
    huffman_code_dict->codes[i] = malloc(sizeof(char*) * strlen(code) + 1);
    huffman_code_dict->character[i] = character;
    printf("insert codes are %s\n", code);
    strcpy(huffman_code_dict->codes[i], code);
    huffman_code_dict->size ++;
}

//void init_header(Header* header){
//    header->encoded_str_len = 0;
//    header->structure_len = 0;
//    header->info = malloc(sizeof(unsigned char) * 1016);
//}

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
            unsigned char* encoded_string,int *encoded_str_length,
            int *decode_str_len,unsigned char *decoded_string,
            FILE* fp){
    int index = 0;
    int row = 0;
    int m = 0;
    unsigned char str[17] = {0};
    //读进2个char,对2个char中的bit进行遍历，如果bit = 0，进入tree的左节点，如果bit = 1,进入tree的右节点。
    //如果中途到达叶节点，记录下叶节点的character,然后写进数组，同时将树重定位到根节点
    //如果中途到第16个bit都没有遇到叶节点，则保留住此时树的位置。下一次新的2个char进入时直接从记录的树节点开始遍历。
    while (index < 16 && *encoded_str_length > 0) {
        str[index] = ((0x80 >> m) & encoded_string[row]) == 0?48:49;
        if (str[index] == 48) {
            temp_tree = temp_tree->left_child;
        } else {
            temp_tree = temp_tree->right_child;
        }
      
        //如果if内容满足left_child 跟right_child 都为Null,说明到达叶子节点，即获得decode过的字符
        if (temp_tree->left_child == NULL && temp_tree->right_child == NULL) {
//            if (temp_tree->character == 'l') {
//                printf("%s\n","meet d");
//            }
            decoded_string[*decode_str_len] = temp_tree->character;
//            printf("%c",decoded_string[*decode_str_len]);
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

void last_occurrence(int* alphabet_last_occurrence,char* p_chars, int p_length){
    for (int i = 0 ; i < p_length; i ++) {
        char s = p_chars[i];
        printf("s is :%d\n", s);
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
    //读进2个char,对2个char中的bit进行遍历，如果bit = 0，进入tree的左节点，如果bit = 1,进入tree的右节点。
    //如果中途到达叶节点，记录下叶节点的character,然后写进数组，同时将树重定位到根节点
    //如果中途到第16个bit都没有遇到叶节点，则保留住此时树的位置。下一次新的2个char进入时直接从记录的树节点开始遍历。
    while (index < 16 && *encoded_str_length > 0 && full_flag == 0) {
        str[index] = ((0x80 >> m) & encoded_string[row]) == 0?48:49;
        if (str[index] == 48) {
            temp_tree = temp_tree->left_child;
        } else {
            temp_tree = temp_tree->right_child;
        }
        //如果if内容满足left_child 跟right_child 都为Null,说明到达叶子节点，即获得decode过的字符
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
        if (*original_char_len == 0) {
//            index ++;
            if (index < 16) {
                (*rest_buffer_count) = 16 - index;
                for (int i = 0; i < 16 - index; i++) {
                    rest_str[i] = ((0x80 >> m) & encoded_string[row]) == 0?48:49;
                    m ++;
                    if (m == 8) {
                        m = 0;
                        row ++;
                    }
                }
            }
            full_flag = 1;
        }
        (*encoded_str_length) --;
    }
    return temp_tree;
}

unsigned char fgetc_buffer(unsigned char* buffer, //用来存储原文缓存字符
                           int* current_buffer_index, //用来记录现在buffer中读取到的位置
                           unsigned char*rest_buffer, // 用来保存之前剩余的bits的 0/1数据
                           int *rest_buffer_count, //保存 0/1数据的个数
                           int *current_rest_buffer_index, //保存现在读取到rest_buffer中的位置。
                           Huffman_Node* tree,//遍历树需要。
                           int search_length,
                           FILE* fp_search){
    unsigned char c = {0};
    Huffman_Node* temp_tree = tree;
    //如果目前还没读取完buffer中的缓存数据，那么先读取其中的数据。
    if (*current_rest_buffer_index < search_length * 3){
        c = buffer[*current_rest_buffer_index];
        (*current_rest_buffer_index) ++;
        return c;
    }
    
    //如果buffer中的数据已经读取完，如果rest_buffer中还有剩余的 0/1数据，那么先读取rest_buffer中的数据
    //如果有有发现leave note，则全部加入到buffer中，然后返回buffer[0].
    int found_leave = 0;
    if (*current_rest_buffer_index <= *rest_buffer_count) {
        memset(buffer, 0, search_length * 3);
        (*current_rest_buffer_index) = 0;
        for (int i = *current_rest_buffer_index; i < *rest_buffer_count; i ++) {
            if (rest_buffer[i] == 48) {
                tree = tree->left_child;
            } else {
                tree = tree->right_child;
            }
            (*current_rest_buffer_index) ++;
            //如果发现leave note,加入到buffer中。
            if (tree->left_child == NULL && tree->right_child == NULL) {
                buffer[found_leave] = tree->character;
                tree = temp_tree;
            }
        }
    }
    //如果在rest buffer中找到的leave node 还没有满足search_length 的 3 倍，说明buffer还没被填满。则从.huffman文件中读取bytes,填充buffer.
    if (found_leave < search_length * 3){
        (*current_rest_buffer_index) = 0;
        memset(rest_buffer, 0, search_length * 3);
        unsigned char* new_bytes = malloc(sizeof(unsigned char) * 2);
        fread(new_bytes, sizeof(unsigned char), 2, fp_search);
        int index = 0;
        int row = 0;
        int m = 0;
        while (index < 16) {
            rest_buffer[index] = ((0x80 >> m) & new_bytes[row]) == 0?48:49;
            if (rest_buffer[index] == 48) {
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
            //如果buffer满了，但是 16个bits 还没读完，那么将剩余的Bits读入rest_buffer中。
            if (found_leave == search_length * 3) {
                (*rest_buffer_count) = 16 - index;
                (*current_rest_buffer_index) = 0; //把rest_buffer_index 设置为0，下一次从第0个位置开始读取 0/1 数据。
                for (int i = index ; i < 16; i++) {
                    rest_buffer[i] = ((0x80 >> m) & new_bytes[row]) == 0?48:49;
                    index ++;
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
    current_buffer_index ++;
    return c;
}

/**
 t_chars是一个二维数组，每一行的列数等于search_chars的长度，一共两列
 每次移动的时候，从文件中读取对应移动的个数，把原来的字符替换。直到没有多余的字符可以读取。
 */
int search(unsigned char **t_chars,char* p_chars, int search_length,
           FILE* fp_search,unsigned char *buffer,int *current_buffer_index,
           unsigned char *rest_buffer,int *rest_buffer_index,int *current_rest_buffer_index,Huffman_Node* tree){
    //get last occurrence table
    int alphabet_last_occurrence[257] = {-1};
    int window_size = search_length;
    memset(alphabet_last_occurrence, -1, sizeof(alphabet_last_occurrence));
    last_occurrence(alphabet_last_occurrence, p_chars, search_length);
    //初始index都在search_length
    int total_original_number = search_length * 2;
    search_length --;
    
    int p_index = search_length; // 该变量用于记录 P 里指向的元素位置
    int t_index = p_index; // 该变脸用于记录 T 里指向的元素位置
    int times = 0;
    int found = 0;
    int row = 0; //用来记录此时移动到了第几列，需要用 t_index % 2 取余来获取当前行，2行为一个周期。
    int column = search_length; //用来记录此时移动到第row行的第column列，需要用 t_index / window_size 来获取当前列，每一行的长度 = search_length。
    int distance = 0; //记录移动的格数
    unsigned char current_char = 0; //用来读取字符。
    int replace_position = 0;
    int number_of_replace_char = 0;
    int over = 0;
    int over_index = 0;
//    if (original_length > search_length) {
        while (t_index < total_original_number) {
            //如果p[index] == t[index]， 往左移动。
            while (t_chars[row][column] == p_chars[p_index]) {
                t_index--;
                row = (t_index -1 + 1 + window_size) / window_size - 1;
                row = row % 2;
                column = t_index % window_size;
                p_index--;
                if (p_index == -1) {
                    times++;
                    printf("p_index is : %d\n", p_index);
                    printf("t_index is : %d\n", t_index);
                    p_index = search_length;
                
//                    row = (t_index -1 + 1 + window_size) / window_size;
//                    row = row % 2;
//                    column = t_index / window_size - 1;
//                    add_char_to_window(search_length + 2);
                    if (over == 0) {
                        for (int i = 0 ; i < 1; i++) {
                            int replace_row = (replace_position + 1 - 1 + window_size) / window_size - 1;
                            replace_row %= 2;
                            int replace_column = replace_position % window_size;
    //                        fread(&t_chars[replace_row][replace_column], sizeof(unsigned char), 1, fp);
                            if((current_char = fgetc_buffer(buffer,current_buffer_index,rest_buffer,rest_buffer_index,current_rest_buffer_index,tree, search_length, fp_search)) != '\0'){
//                           if(((current_char = fgetc(fp_search)) != EOF)){
                                t_chars[replace_row][replace_column] = current_char;
                               total_original_number ++;
                            } else {
                                t_chars[replace_row][replace_column] = '\0';
                                over = 1;
                                over_index = t_index;
                                break;
                            }
                            printf("the (%d, %d) is %c\n", replace_row, replace_column, t_chars[replace_row][replace_column]);
                            replace_position++;
                            //                    add_char_to_window(search_length + 1, row, column, t_chars);
                        }
                    }
//                    replace_position += search_length + 2;
                    t_index = t_index + search_length + 2;
                    row = (t_index + 1 - 1 + window_size) / window_size - 1;
                    row %= 2;
                    column = t_index % window_size;
                    
                    printf("meet one matched string in original string ! \n");
                    found = 1;
                }
            }
            if(found == 0 && t_chars[row][column] != '\0'){
                
                //如果发现p[index] != t[index]，说明发现不匹配字符。进行三种不同的操作
                int last_occurrence = alphabet_last_occurrence[t_chars[row][column]];
                //如果查询字符串"存在"原字符串在不匹配处的字符
                if (last_occurrence != -1){
                    
                    //如果该字符出现在当前位置的左边，说明lastest version还没过。
                    if (last_occurrence < p_index){
                        printf("found lastest version in the left...\n");
                        distance = search_length - alphabet_last_occurrence[t_chars[row][column]];
                        t_index += distance;
//                        row = (t_index -1 + 1 + window_size) / window_size;
//                        row = row % 2;
//                        column = t_index / window_size - 1;
//                        add_char_to_window(distance);
                        number_of_replace_char = distance;
                        p_index = search_length;
                    } else {
                        printf("found lastest version in the right...\n");
                        distance = search_length - p_index;
                        p_index = search_length;
                        t_index = t_index + distance + 1;
                        number_of_replace_char = distance - (search_length - p_index);
//                        row = (t_index -1 + 1 + window_size) / window_size;
//                        row = row % 2;
//                        column = t_index / window_size - 1;
//                        add_char_to_window(distance + 1);
                        
                        
                    }
                } else { //如果查询字符串“不存在"原字符串在不匹配处的字符
                    //                    int distance = p_index;
                    printf("not found...\n");
                    distance = search_length + 1;
                    t_index += distance;
                    number_of_replace_char = p_index + 1;
                    p_index = search_length;
                }
//                int c = 0;
                if (over == 0) {
                    for (int i = 0 ; i < number_of_replace_char; i++) {
                        int replace_row = (replace_position -1 + 1 + window_size) / window_size - 1;
                        replace_row %= 2;
                        int replace_column = replace_position % window_size;
    //                    t_chars[replace_row][replace_column] = fgetc(fp);
    //                    fread(&t_chars[replace_row][replace_column], sizeof(unsigned char), 1, fp);
                        
//                        if(((current_char = fgetc(fp_search)) != EOF)){
                        if((current_char = fgetc_buffer(buffer,current_buffer_index,rest_buffer,rest_buffer_index,current_rest_buffer_index,tree, search_length, fp_search)) != '\0'){
                            t_chars[replace_row][replace_column] = current_char;
                            total_original_number ++;
                        } else {
                            t_chars[replace_row][replace_column] = '\0';
                            over = 1;
                            over_index = t_index;
                            break;
                        }
                        printf("the (%d, %d) is %c\n", replace_row, replace_column, t_chars[replace_row][replace_column]);
                        replace_position ++;
                    }
                }
//                t_index = t_index + search_length + 2;
                row = (t_index + 1 - 1 + window_size) / window_size - 1;
                row %= 2;
                column = t_index % window_size;
            }
            found = 0;
        }
//    }
    printf("times is %d\n", times);
    return times;
}


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


/**
 Rewrite the output.huffman file to add the length of encoded string to the beginning.
 */
void init_huffman_code(Huffman_code_dict* huffman_code_and_char){
    huffman_code_and_char->max_size = 256;
    huffman_code_and_char->size = 0;
    huffman_code_and_char->codes = malloc(sizeof(char*) * 256 + 1);
//    huffman_code_and_char->character;
}

void add_encoded_str_len_info(int encoded_str_len, char *file){
    FILE* fd;
    fd = fopen(file, "rb");
    printf("open huffman file...\n");
    printf("transferring...\n");
    printf("encoded_str length is %d\n", encoded_str_len);
    int encoded_bytes = (encoded_str_len + 8 - 1) / 8;
    int total_bytes = encoded_bytes + 1020;
    
    //总共需要读几次
    int reading_times = (total_bytes + 256 - 1) / 256;
    
//    unsigned char b[256] = {0};
    unsigned char buffer[reading_times][256];
    size_t length = 0;
    int time = 0;
    fseek(fd,4, SEEK_SET);
    while(((length = fread(buffer[time], sizeof(unsigned char), 256, fd)) == 256)){
        printf("add....\n");
        printf("length is : %zu\n", length);
        time ++;
    }
    printf("after length is %zu\n", length);
    if (length != 0) {
        time ++;
        fread(buffer[time], sizeof(unsigned char), length, fd);
    }
    fclose(fd);
    fd = fopen(file, "wb");
    fwrite(&encoded_str_len, sizeof(int), 1, fd);
    for (int j = 0; j < reading_times - 1; j ++) {
        printf("rewriting...\n");
//         printf("temp_buffer[j] is %s\n", temp_buffer[j]);
        fwrite(buffer[j], sizeof(unsigned char), 256, fd);
        total_bytes -=256;
    }
    fwrite(buffer[reading_times - 1], sizeof(unsigned char), total_bytes, fd);
    fclose(fd);
}


//int main(int argc, char* argv[]){
//    FILE* fp_search;
//    fp_search = fopen("/Users/wangeric/Documents/C++/mac/test.txt", "rb");
//    char search_string[] = "aa";
//    int search_length = sizeof(search_string) / sizeof(unsigned char) - 1;
////    unsigned char original_string[2][search_length] = {0};
//    unsigned char** original_string = malloc(sizeof(unsigned char*) * 2);
//    unsigned char *string_1 = malloc(sizeof(unsigned char) * search_length);
//    unsigned char *string_2 = malloc(sizeof(unsigned char) * search_length);
////    original_string[0] = {0};
////    original_string[1] = {0};
////    int length = sizeof(original_string) /sizeof(unsigned char) - 1;
//
////    original_string[0] = search_string;
//    fread(string_1, sizeof(unsigned char), search_length, fp_search);
//    fread(string_2, sizeof(unsigned char), search_length, fp_search);
//
//    original_string[0] = string_1;
//    original_string[1] = string_2;
////    int c = fgetc(fp_search);
////    printf("%c", c);
//    search(original_string,search_string,search_length,fp_search);
//}


int main(){
//    if (strcmp(argv[1],"-e") == 0) {
//        printf("encoding......\n");
////        printf("encoding file is %s\n", argv[2]);
//        FILE *fp;
//        Heap *heap = init_Heap(260);
//        int i;
////        fp = fopen(argv[2], "rb");
//        fp = fopen(argv[2], "rb");
////        fp = fopen("/Users/wangeric/Documents/C++/mac/test.txt", "rb");
//        if (fp == NULL) {
//            perror("Error in opening file");
//            return (-1);
//        }
//        int c;
//        int char_count = 0;
//        int char_table[260] = {0};
//        while ((c = fgetc(fp)) != EOF) {
//            char_table[c]++;
//            char_count ++;
//        }
//        fclose(fp);
//        Huffman_Node *node;
//        for (i = 0; i < 256; i++) {
//            if (char_table[i] != 0) {
//                node = malloc(sizeof(Huffman_Node));
//                node->left_child = NULL;
//                node->right_child = NULL;
//                node->frequency = char_table[i];
//                node->character = i;
//                insert_element(heap, node);
//            }
//        }
//
//        Huffman_Node* tree = merge_huffman_tree(heap);
//        unsigned char huffman_tree_structure[1020] = {0};
//
//        //generate huffman structure
//        int huffman_structure_length = generate_huffman_tree_structure(huffman_tree_structure,tree);
//        printf("huffman_structure_length is :%d\n",huffman_structure_length);
//
//
//        //get huffman code dictionary by huffman-tree's structure.
//        Huffman_code_dict huffman_code_dict;
//        init_huffman_code(&huffman_code_dict);
//        //生成huffman code的字典
//        generate_huffman_dict(&huffman_code_dict,huffman_tree_structure,huffman_structure_length);
////        int total_length = 0;
//        for (int i = 0; i < huffman_code_dict.size; i++) {
//            printf("codes are %s, length is %d\n",huffman_code_dict.codes[i], (int)strlen(huffman_code_dict.codes[i]));
//        }
////        printf("total codes are : %d\n",total_length);
//        unsigned char bitarray[1000] = {0};
//
//    //===================================ENCODE======================================================
//        FILE *fp_out;
//        fp_out = fopen(argv[3], "wb");
//        int padding = 0;
//        fwrite(&padding, sizeof(int),1, fp_out);
//        fwrite(&huffman_structure_length, sizeof(int), 1, fp_out);
//        fwrite(&huffman_tree_structure, sizeof(unsigned char),1016, fp_out);
//        fp = fopen(argv[2], "rb");
////        fp = fopen(argv[2], "rb");
//        int count = 0;
//        int times = 0;
//    //    int ss = 1;
//    //    printf("characters is :\n");
//        int length = 0;
//        while ((c = fgetc(fp)) != EOF) {
//            encode(&huffman_code_dict, c, bitarray, &count,fp_out, &times, &length);
//        }
//        if (count != 0) { //如果有多余的bits,继续
//            int rest_bytes = (count + 8 - 1) / 8;
//            printf("rest bytes are : %d\n", rest_bytes);
//            times = times * 8000 + count;
//            printf("times is %d\n", times);
//            fwrite(bitarray, sizeof(unsigned char), rest_bytes, fp_out);
//        }
//        fseek(fp_out, 0, SEEK_SET);
////        int header = 10;
//        fwrite(&times, sizeof(int), 1, fp_out);
//        fclose(fp_out);
////        fp_out = fopen("/Users/wangeric/Documents/C++/mac/output.huffman", "rb");
////        FILE* fp_out;
////        );
////    char file[49] = "/Users/wangeric/Documents/C++/mac/output.huffman";
////        add_encoded_str_len_info(times, argv[3]);
////        fclose(fp_out);
////        fclose(fpTemp);
//    }
////================================================================================================
//
//
////===============================================DECODE============================================
//    else if (strcmp(argv[1],"-d") == 0) {
//        printf("decoding.....\n");
//        int *structure_length = malloc(sizeof(int));
//        int *encoded_string_length = malloc(sizeof(int));
//        FILE* fp;
//        FILE* fp_out;
//        fp = fopen(argv[2], "rb");
//        fp_out = fopen(argv[3], "wb");
//        fread(encoded_string_length, sizeof(int), 1, fp); //encode string 长度
//        fread(structure_length, sizeof(int), 1, fp); //树结构长度
//        unsigned char* structure = malloc(sizeof(unsigned char) * (*structure_length)); //树结构
//        fread(structure, sizeof(unsigned char), *structure_length, fp);
//        int rest = 1024 - 8 - *structure_length;
//        fseek(fp, rest, SEEK_CUR);
//        int bytes_of_encoded_string = *encoded_string_length == 0 ? 0 : ((*encoded_string_length + 8 - 1) / 8);
//    //    unsigned char* encoded_string = malloc(sizeof(unsigned char) * bytes_of_encoded_string); //encode string
//
//        Huffman_Node top_node;
//        reconstruct_huffman_tree(structure,&top_node, *structure_length);
//        Huffman_Node *tem_tree = malloc(sizeof(Huffman_Node));
//        tem_tree = &top_node;
//        unsigned char read_buffer[2];
//        int decode_str_len = 0;
//        unsigned char decode_char_list[100] = {0};
//        for (int x = 0;x < bytes_of_encoded_string - 1; x ++) {
//            fread(read_buffer, sizeof(unsigned char), 2, fp);
//            Huffman_Node* r = decode(&top_node, tem_tree,
//                                     read_buffer,
//                                     encoded_string_length,&decode_str_len,
//                                     decode_char_list,fp_out);
//            tem_tree = r;
//        }
//        if (decode_str_len != 0) {
//            fwrite(decode_char_list, sizeof(unsigned char), decode_str_len, fp_out);
//        }
//        fclose(fp);
//        fclose(fp_out);
//    }
//////==================================================================================================
////
////
//////===========================================SEARCH================================================
////
//     else if(strcmp(argv[1],"-s") == 0) {
         FILE* fp;
//         FILE* fp_out = NULL;
         fp = fopen("/Users/wangeric/Documents/C++/mac/output.huffman", "rb");
         int *structure_length = malloc(sizeof(int));

         int *encoded_string_length = malloc(sizeof(int));
         fread(encoded_string_length, sizeof(int), 1, fp); //encode string 长度
         printf("encoded_string_length is %d\n", *encoded_string_length);
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
//         int s = *encoded_string_length;

         int search_length;
        char search_string[] = "aa";
         search_length = sizeof(search_string) / sizeof(unsigned char) - 1;

         int original_char_len = search_length * 3;
         unsigned char *buffer = malloc(sizeof(unsigned char) * (search_length * 3) + 1);
//         int finish_flag = 0;
        unsigned char *rest_str = malloc(sizeof(unsigned char) * (search_length *3));
    
        //记录还剩下多少个bits剩余
        int rest_buffer_count = 0;
        //每次读2个bytes, 直到读满查询字符串长度的3倍，作为read buffer
        //如果读满 search_length * 3,则停止
        //如果未读满 search_length * 3就提前停止，那么将保留这个read_buffer中剩余Bits对应的 0/1 数组。
         while (bytes_of_encoded_string > 0 && encoded_string_length > 0 && original_char_len > 0) {
             fread(read_buffer, sizeof(unsigned char), 2, fp);
             Huffman_Node* r = search_decode(&top_node, tem_tree,
                                      read_buffer,&original_char_len,
                                      encoded_string_length,&decode_str_len,
                                      buffer, rest_str,&rest_buffer_count);
             tem_tree = r;
             bytes_of_encoded_string -=2;
         }
    unsigned char **original_txt_buffer = malloc(sizeof(unsigned char*) * 2);
    unsigned char *string_1 = malloc(sizeof(unsigned char) * search_length);
    unsigned char *string_2 = malloc(sizeof(unsigned char) * search_length);
    for (int j = 0; j < search_length; j++) {
        string_1[j % search_length] = buffer[j];
    }
    for (int j = search_length; j <search_length * 2; j++) {
        string_2[j % search_length] = buffer[j];
    }
    original_txt_buffer[0] = string_1;
    original_txt_buffer[1] = string_2;
    int current_buffer_index = search_length * 2;
    
//    search(unsigned char **t_chars,char* p_chars, int search_length,
//           FILE* fp_search,unsigned char *buffer,int *current_buffer_index,
//           unsigned char *rest_buffer,int *rest_buffer_index,int *current_rest_buffer_index,Huffman_Node* tree)
    
    //初始化目前读到的rest_buffer的index = 0;
    int current_rest_buffer_index = 0;
    search(original_txt_buffer, search_string, search_length, fp,
           buffer,&current_buffer_index,rest_str,&rest_buffer_count,&current_rest_buffer_index,&top_node);
         
//     }
    return 0;
}
//==================================================================================================
