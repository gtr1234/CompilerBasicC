#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

typedef struct _Node{
    char * value;
    int num_children;
    struct _Node* children;
    int null_value;
} Node;


int num_statements=0;



//char variableTable[5000][2];


Node programTree(char* string);
Node statementTree(char * string);
Node whileTree(char * string);
Node assignmentTree(char * string);
Node expressionTree(char * string);
Node expression1Tree(char * string);
Node termTree(char * string);
Node factorTree(char * string);
Node constantTree(char * string);
Node variableTree(char * string);
int isValidCharacter(char c);
void preorder(Node *root);



int isValidCharacter(char c){   // if anything is missed, add... . No requirement of space character(confirmation required)
    if((c >='0' && c<='9') || (c=='+') || (c=='-') || (c=='*') || (c=='/') || (c=='<') || (c=='=') || (c=='.') ||(c=='(') || (c==')') ||(c=='[') || (c==']') || (c>='a' && c<='z'))
        return 1;
    return 0;
}

Node programTree(char* string){
    Node root;

    printf("came to programTree\n");
    
    if(string==NULL || strlen(string)==0){ 
        root.null_value=1;
        return root;
    }

    root.null_value=0;

    
    printf("P str - %s -- %d\n",string,strlen(string));


    int length_max = strlen(string);

    root.value="P";
    root.children = (Node*)malloc(sizeof(Node)*2);
    root.num_children=2;

    root.children[0].num_children=0;
    root.children[1].num_children=0;


    int while_state=0;

    if(strstr(string,"while")){
        while_state=1;
    }

    char * token1;
    char * token2;

    if(while_state==1){
        token1 = (char *) malloc(sizeof(char)*length_max);
        token2 = (char *) malloc(sizeof(char)*length_max);

        int len_token1=0;
        int len_token2=0;

        int brackcount=0;
        int i;
        int state_1=1;

        for(i=0;i<length_max;i++){
            if(state_1==1 && string[i]=='{'){
                brackcount++;
                token1[len_token1++] = string[i];
            }
            else if(state_1==1 && brackcount==0){
                token1[len_token1++] = string[i];
            }
            else if(state_1==1 && brackcount>1){
                token1[len_token1++] = string[i];
            }
            else if(state_1==1 && brackcount==1 && string[i]!='}'){
                token1[len_token1++] = string[i];
            }
            else if(state_1==1 && brackcount==1 && string[i]=='}'){
                token1[len_token1++] = string[i];
                state_1=0;
                break;
            }
        }
        token1[len_token1]=0;
        if(len_token1 != length_max-1){ 
            int i;
            for(i=len_token1+1;i<length_max;i++){
                token2[len_token2++] = string[i];
            }
            token2[len_token2]=0;

            //printf("%s ____ %s\n",token1,token2);
        }
        else{
            token2=NULL;
        } 
    }
    else if(while_state==0){
        token1 = strtok(string,";");
        token2 = strtok(NULL,"");
    }

    
    root.children[0] = statementTree(token1);
    root.children[1] = programTree(token2);

    return root;
}


Node statementTree(char * string){
    Node root;

    root.value="S";
    root.children = (Node*)malloc(sizeof(Node)*1);
    root.num_children=1;
    root.children[0].num_children=0;
    root.null_value=0;


    int len = strlen(string);
    int while_state=0;

    if(strstr(string,"while")){  // while loop
        while_state=1;
    }
    
    if(while_state==1){
        root.children[0] = whileTree(string);
    }
    else if(while_state==0){  // if not while then assignment
        root.children[0] = assignmentTree(string);
    }

    return root;
}

Node whileTree(char * string){
    Node root;

    root.null_value=0;
    root.value="W";
    root.children = (Node*)malloc(sizeof(Node)*7);
    int i;
    for(i=0;i<7;i++){
        root.children[i].num_children=0;
    }

    int length_max = strlen(string);

    root.num_children=7;

    char  expression[length_max];
    int expression_len=0;
    char statement[length_max];
    int statement_len=0;


    int brackcount=0;

    int state=0;

    for(i=0;i<strlen(string);i++){
        if(isValidCharacter(string[i])) {
            if (string[i] == '(') {             // here bracket matching use stack or counter !!! V-Imp
                if(state==0){
                    state=1;
                }
                else if(state==1){
                    expression[expression_len++] = string[i];
                }
                brackcount++;
            }
            else if (state == 1 && brackcount > 1) {
                expression[expression_len++] = string[i];
            } 
            else if (state == 1 && brackcount == 1 && string[i] != ')') {
                expression[expression_len++] = string[i];
            } 
            else if (state == 1 && string[i] == ')' && brackcount == 1) {
                state = 0;
                break;
            }
        }
    }

    brackcount=0;
    state=0;
    for(i=0;i<strlen(string);i++){
        if(string[i] == '{'){
            if(state==0){
                state=1;
            }
            else if(state==1){
                statement[statement_len++] = string[i];
            }
            brackcount++;
        }
        else if(state==1 && brackcount > 1) {
            statement[statement_len++] = string[i];
        }
        else if(state==1 && brackcount==1 && string[i]!='}'){
            statement[statement_len++] = string[i];
        }
        else if(state==1 && brackcount==1 && string[i]=='}'){
            state=0;
            break;
        }
    }

    expression[expression_len]=0;
    statement[statement_len]=0;

    root.children[0].value = "while";
    root.children[1].value = "(";

    root.children[2] = expressionTree(expression);

    root.children[3].value = ")";
    root.children[4].value = "{";

    root.children[5] = statementTree(statement);

    root.children[6].value = "}";



    return root;
}

Node assignmentTree(char * string){
    Node root;

    root.value="A";
    root.children = (Node*)malloc(sizeof(Node)*3);
    root.num_children=3;
    root.null_value=0;

    int i;
    for(i=0;i<3;i++){
        root.children[i].num_children=0;
    }

    int length_max = strlen(string);

    char *expression;
    int expression_len=0;

    char *variableName;
    int variableLen=0;

    variableName = (char *) malloc(sizeof(char)*length_max);
    expression = (char *) malloc(sizeof(char)*length_max);

    int state=0;
    for(i=0;i<strlen(string);i++){
        if(isValidCharacter(string[i])) {
            if (string[i] == '=') {
                state = 1;
            } else if (state == 1) {
                expression[expression_len++] = string[i];
            }
        }
    }
    expression[expression_len]=0;

    for(i=0;i<strlen(string);i++){
        if(string[i]=='='){
            break;
        }
        else if(string[i]!=' '){
            variableName[variableLen++] = string[i];
        }
    }

    variableName[variableLen]=0;
    //for children[0] variable table required?

    root.children[0] = variableTree(variableName);
    root.children[1].value = "=";
    root.children[2] = expressionTree(expression);


    return root;
}

Node expressionTree(char * string){   // no space should be there in string
    Node root;

    root.value="E";
    root.children = (Node*)malloc(sizeof(Node)*3);
    root.num_children=3;
    root.null_value=0;

    int i;
    for(i=0;i<3;i++){
        root.children[i].num_children=0;
    }


    int brackcount=0;
    int state=0;  // 1 - <,2 - ==
    int indexofoperator=-1;

    int prev_equal=0;

    for(i=0;i<strlen(string);i++){
        if(string[i]=='(') {
            brackcount++;
            prev_equal=0;
        }
        else if(string[i]==')'){
            brackcount--;
            prev_equal=0;
        }
        else if(brackcount==0){
            if(string[i]=='<'){
                state=1;
                indexofoperator=i;
                break;
            }
            else if(string[i]=='='){
                if(prev_equal==0){
                    prev_equal=1;
                }
                else if(prev_equal==1){
                    state=2;
                    indexofoperator=i;
                    break;
                }
            }
        }
    }

    int length_max = strlen(string);

    char * str1;    // careful on lengths (no of characters+1 -- due to /0)
    char * str2;
    int str1_len=0;
    int str2_len=0;

    str1 = (char *) malloc(sizeof(char)*length_max);
    str2 = (char *) malloc(sizeof(char)*length_max);


    if(state==1){
        for(i=0;i<indexofoperator;i++){
            str1[str1_len++] = string[i];
        }
        for(i=indexofoperator+1;i<strlen(string);i++){
            str2[str2_len++] = string[i];
        }

        str1[str1_len]=0;
        str2[str2_len]=0;

        root.children[0] = expression1Tree(str1);
        root.children[1].value = "<";
        root.children[2] = expressionTree(str2);

    }
    else if(state==2){
        for(i=0;i<indexofoperator-1;i++){
            str1[str1_len++]=string[i];
        }
        for(i=indexofoperator+1;i<strlen(string);i++){
            str2[str2_len++]=string[i];
        }

        str1[str1_len]=0;
        str2[str2_len]=0;

        root.children[0] = expression1Tree(str1);
        root.children[1].value = "==";
        root.children[2] = expressionTree(str2);

    }

    else if(state==0){
        root.children[0] = expression1Tree(string);
        root.num_children=1;
    }

    return root;
}


Node expression1Tree(char * string){
    Node root;

    root.value="E1";
    root.children = (Node*)malloc(sizeof(Node)*3);
    root.num_children=3;
    root.null_value=0;

    int i;
    for(i=0;i<3;i++){
        root.children[i].num_children=0;
    }

    int state=0;    //1 - +,2 - -
    int indexofoperator=-1;
    int brackcount=0;

    for(i=0;i<strlen(string);i++){
        if(string[i]=='(') {
            brackcount++;
        }
        else if(string[i]==')'){
            brackcount--;
        }
        else if(brackcount==0){
            if(string[i]=='+'){
                state=1;
                indexofoperator=i;
                break;
            }
            else if(string[i]=='-'){
                state=2;
                indexofoperator=i;
                break;
            }
        }

    }

    int length_max = strlen(string);

    char * str1;
    char * str2;
    int str1_len=0;
    int str2_len=0;

    str1 = (char *) malloc(sizeof(char)*length_max);
    str2 = (char *) malloc(sizeof(char)*length_max);


    if(state==1){
        for(i=0;i<indexofoperator;i++){
            str1[str1_len++]=string[i];
        }
        for(i=indexofoperator+1;i<strlen(string);i++){
            str2[str2_len++]=string[i];
        }

        str1[str1_len]=0;
        str2[str2_len]=0;

        root.children[0] = termTree(str1);
        root.children[1].value = "+";
        root.children[2] = expression1Tree(str2);
    }
    else if(state==2){
        for(i=0;i<indexofoperator;i++){
            str1[str1_len++]=string[i];
        }
        for(i=indexofoperator+1;i<strlen(string);i++){
            str2[str2_len++]=string[i];
        }

        str1[str1_len]=0;
        str2[str2_len]=0;

        root.children[0] = termTree(str1);
        root.children[1].value = "-";
        root.children[2] = expression1Tree(str2);
    }
    else if(state==0){
        root.children[0] = termTree(string);
        root.num_children=1;
    }

    return root;
}


Node termTree(char * string){
    Node root;

    root.value="T";
    root.children = (Node*)malloc(sizeof(Node)*3);
    root.num_children=3;
    root.null_value=0;

    int i;
    for(i=0;i<3;i++){
        root.children[i].num_children=0;
    }


    int state=0;    //1 - *,2 - /
    int indexofoperator=-1;
    int brackcount=0;

    for(i=0;i<strlen(string);i++){
        if(string[i]=='(') {
            brackcount++;
        }
        else if(string[i]==')'){
            brackcount--;
        }
        else if(brackcount==0){
            if(string[i]=='*'){
                state=1;
                indexofoperator=i;
                break;
            }
            else if(string[i]=='/'){
                state=2;
                indexofoperator=i;
                break;
            }
        }

    }

    int length_max = strlen(string);
    char * str1;
    char * str2;
    int str1_len=0;
    int str2_len=0;

    str1 = (char *) malloc(sizeof(char)*length_max);
    str2 = (char *) malloc(sizeof(char)*length_max);


    if(state==1){
        for(i=0;i<indexofoperator;i++){
            str1[str1_len++]=string[i];
        }
        for(i=indexofoperator+1;i<strlen(string);i++){
            str2[str2_len++]=string[i];
        }

        str1[str1_len]=0;
        str2[str2_len]=0;

        root.children[0] = factorTree(str1);
        root.children[1].value = "*";
        root.children[2] = termTree(str2);
    }
    else if(state==2){
        for(i=0;i<indexofoperator;i++){
            str1[str1_len++]=string[i];
        }
        for(i=indexofoperator+1;i<strlen(string);i++){
            str2[str2_len++]=string[i];
        }

        str1[str1_len++]=0;
        str2[str2_len++]=0;

        root.children[0] = factorTree(str1);
        root.children[1].value = "/";
        root.children[2] = termTree(str2);
    }
    else if(state==0){
        root.children[0] = factorTree(string);
        root.num_children=1;
    }

    return root;
}


Node factorTree(char * string){
    Node root;

    root.value = "F";
    root.children = (Node *) malloc(sizeof(Node) * 3);
    root.num_children=3;
    root.null_value=0;

    int i;
    for(i=0;i<3;i++){
        root.children[i].num_children=0;
    }

    int state=0;
    int state1=0;    // 1-(),2-C,3-V
    int brackcount=0;

    int length_max = strlen(string);
    
    char * str1;
    int str1_len=0;
    str1 = (char *) malloc(sizeof(char)*length_max);

    for(i=0;i<strlen(string);i++){
        if(string[i]=='('){
            if(state==0){
                state=1;
            }
            else if(state==1){
                str1[str1_len++]=string[i];
            }
            state1=1;
            brackcount++;
        }
        else if(state==1 && brackcount>1){
            str1[str1_len++]=string[i];
        }
        else if(state==1 && brackcount==1 && string[i]!=')'){
            str1[str1_len++]=string[i];
        }
        else if(state==1 && brackcount==1 && string[i]==')'){
            state=0;
            break;
        }
    }

    str1[str1_len]=0;

    if(state1==0){
        int err_state1=0;
        for(i=0;i<strlen(string);i++){
            if(!isalpha(string[i])){
                err_state1=1;
                break;
            }
        }
        if(err_state1==0) state1=3;
    }

    if(state1==0){
        int err_state1=0;
        for(i=0;i<strlen(string);i++){
            if(!isdigit(string[i]) && string[i]!='.'){
                err_state1=1;
                break;
            }
        }
        if(err_state1==0) state1=2;
    }


    if(state1==1){
        root.children[0].value="(";
        root.children[1]=expressionTree(str1);
        root.children[2].value=")";
    }
    else if(state1==2){
        root.children[0] = constantTree(string);
        root.num_children=1;
    }
    else if(state1==3){
        root.children[0] = variableTree(string);
        root.num_children=1;
    }


    return root;
}


Node constantTree(char * string){
    Node root;

    root.value = "C";
    root.children = (Node *) malloc(sizeof(Node) * 1);
    root.children[0].num_children=0;
    root.num_children=1;
    root.null_value=0;

    root.children[0].value = (char *) malloc(sizeof(char)*strlen(string));

    strcpy(root.children[0].value,string);

    printf("came to constantTree\n");

    //printf("str - %s\n",string);

    return root;
}

Node variableTree(char * string){
    Node root;

    root.value = "V";
    root.children = (Node *) malloc(sizeof(Node) * 1);
    root.children[0].num_children=0;
    root.num_children=1;
    root.null_value=0;

    root.children[0].value = (char *) malloc(sizeof(char)*strlen(string));

    strcpy(root.children[0].value,string);

    printf("came to variableTree\n");

    return root;
}

void preorder(Node * root){
    if(root == NULL || root->null_value==1) return;

    if(root->value!=NULL) {
        printf("%s ", root->value);
    }
    int i;
    for(i=0;i<root->num_children;i++){
        preorder(&root->children[i]);
    }
}

int main() {
    FILE *file = fopen("test.txt", "rb");

    fseek (file, 0, SEEK_END);
    long length = ftell (file);
    fseek (file, 0, SEEK_SET);
    char * buffer = malloc (length);
    fread (buffer, 1, length, file);
    fclose (file);

    buffer[length]=0;

    Node root;
    printf("%ld\n",length);
    char * buffer_new = (char *) malloc(length);

    int buffer_new_len=0;


    printf("buff-%d-%s\n",strlen(buffer),buffer);

    int i;
    for(i=0;i<strlen(buffer);i++){  // check again to add required characters
        if(isalnum(buffer[i]) || buffer[i]=='+' || buffer[i]=='-' || buffer[i]=='*' || buffer[i]=='/' || buffer[i]=='=' || buffer[i]==';' || buffer[i]=='(' || buffer[i]==')' || buffer[i]=='[' || buffer[i]==']' || buffer[i]=='{' || buffer[i]=='}' || buffer[i]=='<' || buffer[i]=='.' ){
            buffer_new[buffer_new_len++] = buffer[i];
        }
    }

    buffer_new[buffer_new_len]=0;



    printf("buff_new-%d-%s\n",strlen(buffer_new),buffer_new);

    root = programTree(buffer_new);

    /*printf("root - %s\n",root.value);
    printf("root.c0 - %s\n",root.children[0].value);
    printf("root.c1 - %s\n",root.children[1].value);

    /*printf("root.c.c.c0 - %s\n",root.children[0].children[0].children[0].value);
    printf("root.c.c.c1 - %s\n",root.children[0].children[0].children[1].value);
    printf("root.c.c.c2 - %s\n",root.children[0].children[0].children[2].value);

    printf("root.c.c.c2 - %s\n",root.children[0].children[0].children[2].children[0].value);
    printf("root.c.c.c2 - %s\n",root.children[0].children[0].children[2].children[0].children[0].value);
    printf("root.c.c.c2 - %s\n",root.children[0].children[0].children[2].children[0].children[0].children[0].value);
    printf("root.c.c.c2 - %s\n",root.children[0].children[0].children[2].children[0].children[0].children[0].children[0].value);
    printf("root.c.c.c2 - %s\n",root.children[0].children[0].children[2].children[0].children[0].children[0].children[0].children[0].value);
    */

    //preorder traversal
    printf("error in preorder\n");
    preorder(&root);
    printf("\n");

    return 0;
}



