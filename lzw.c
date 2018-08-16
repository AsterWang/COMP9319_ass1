#include <stdio.h>
#include<stdlib.h>
#include<string.h>


#define NOT_EXIST -1
typedef struct {
    char** seq;
    int* code;
    int size;
    int max_size;
} Dictionary;


void insert_seq(Dictionary* dict, char* seq){
    int i = dict ->size;
    dict ->seq[i] = malloc(sizeof(char) * strlen(seq) + 1);
    dict ->code[i] = i;
    dict ->size++;
    strcpy(dict->seq[i], seq);
}

void init_dictionary(Dictionary* dict, int max_size){
    dict ->max_size = max_size;
    dict ->size = 0;
    dict ->seq = malloc(sizeof(char*) * max_size);
    dict ->code = malloc(sizeof(int) * max_size);
    
    insert_seq(dict, "#");
    char seq[2] = "A";
    for (int i = 0; i < 26; ++i){
        insert_seq(dict, seq);
        seq[0]++;
    }
}

void print_dictionary(Dictionary* dict){
    printf("==========================\n");
    printf("code       Sequence\n");
    printf("==========================\n");
    for (int i = 0; i < dict->size; ++i)
    {
        printf("%5d%7c", dict -> code[i],' ');
        printf("%s\n", dict ->seq[i]);
    }
    printf("===========================\n");
}

int get_seq_code(Dictionary* dict, char* seq){
    for (int i = 0; i < dict->size; ++i)
    {
        /* code */
        if (strcmp(dict->seq[i],seq) == 0){
            return dict -> code[i];
        }
    }
    return NOT_EXIST;
}
char* get_code_seq(Dictionary* dict, int code){
    if(code < 0 || code >= dict -> size){
        return NULL;
    } else{
        int i = code;
        return dict ->seq[i];
    }
}


void lzw_encode(char* text, Dictionary* dict){
    char current[1000];
    char next;
    int code;
    int i = 0;
    while(i < strlen(text)){
        sprintf(current,"%c", text[i]);
        next = text[i+1];
        while(get_seq_code(dict, current) != NOT_EXIST){
            sprintf(current,"%s%c",current,next);
            i++;
            next = text[i+1];
        }
        current[strlen(current) - 1] = '\0'; //抹掉最后一个字符
        next = text[i];
        code = get_seq_code(dict,current);
        sprintf(current,"%s%c",current,next);
        insert_seq(dict,current);
        // printf("%d", code);
    }
}

void lzw_decode(int codes[], int n, Dictionary* dict){
    int code;
    char pre[1000];
    char* output;
    
    code = codes[0];
    output = get_code_seq(dict, code);
    printf("%s\n", output);
    
    int i;
    for (i = 1; i < n; ++i)
    {
        /* code */
        code = codes[i];
        strcpy(pre, output);
        output = get_code_seq(dict, code);
        sprintf(pre, "%s%c", pre, output[0]);
        insert_seq(dict, pre);
        printf("%s\n", output);
    }
    printf("\n");
}

int main(){
    Dictionary dict;
    init_dictionary(&dict,1000);
//    int arr[] = {20,15,2,5,15,18,14};
//    int size;
//    size = sizeof(arr)/sizeof(arr[0]);
//    printf("size is %d\n", size);
//    lzw_decode(arr,sizeof(arr)/sizeof(arr[0]),&dict);
//    print_dictionary(&dict);
//    lzw_encode("TOBEORNOTTOBEORTOBEORNOT",&dict);
    return 0;
}
