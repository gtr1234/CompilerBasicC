#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#define MAX_SIZE 100
typedef struct _Node{
    char * value;
    char * eval;
    int num_children;
    struct _Node* children;
    int null_value;
    int left;
    int right;
} Node;

char *table[100];
int po = 0;
float values[100];
int gIndex=0;
int num_statements=0;



//char variableTable[5000][2]; 


Node programTree(char* string);
Node statementTree(char * string);
Node readTree(char * string);
Node printTree(char * string);
Node whileTree(char * string);
Node assignmentTree(char * string);
Node expressionTree(char * string);
Node expression1Tree(char * string);
Node termTree(char * string);
Node factorTree(char * string);
Node constantTree(char * string);
Node variableTree(char * string);
int isValidCharacter(char c);
void preorder(Node *root,FILE * f);
void postorder(Node* root);
float strToFloat(char *c);
//void postorder(Node *root,FILE * f);


int isValidCharacter(char c){   // if anything is missed, add... . No requirement of space character(confirmation required)
    if((c >='0' && c<='9') || (c=='+') || (c=='-') || (c=='*') || (c=='/') || (c=='<') || (c=='=') || (c=='.') ||(c=='(') || (c==')') ||(c=='[') || (c==']') || (c>='a' && c<='z'))
        return 1;
    return 0;
}

Node programTree(char* string){
    Node root;
    root.eval = NULL;

    if(string==NULL || strlen(string)==0){
        root.null_value=1;
        return root;
    }

    root.null_value=0;



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
    Node temp = programTree(token2);
    if(temp.null_value == 1){
        root.num_children=1;
        root.left = 0;
    	root.right = -1;
    }
    else{
        root.children[1] = temp;
        root.left = 0;
        root.right = 1;
    }
    
    return root;
}


Node statementTree(char * string){
    Node root;
    root.eval = NULL;
    root.value="S";
    root.children = (Node*)malloc(sizeof(Node)*1);
    root.num_children=1;
    root.children[0].num_children=0;
    root.null_value=0;


    int len = strlen(string);
    int while_state=0;  // 1 - while, 2- read, 3- print , 4- assignment

    if(strstr(string,"while")){  // while loop
        while_state=1;
    }
    else if(strstr(string,"read")){
        while_state=2;
    }
    else if(strstr(string,"print")){
        while_state=3;
    }

    if(while_state==1){   // while
        root.children[0] = whileTree(string);
    }
    else if(while_state==0){   // assignment
        root.children[0] = assignmentTree(string);
    }
    else if(while_state==2){   // read
        root.children[0] = readTree(string);
    }
    else if(while_state==3){    // print
        root.children[0] = printTree(string);
    }

    root.left = 0;
    root.right = -1;
    
    return root;
}


Node readTree(char * string) {
    Node root;
    root.eval = NULL;
    root.null_value = 0;
    root.value = "R";
    root.children = (Node *) malloc(sizeof(Node) * 2);
    int i;
    for (i = 0; i < 2; i++) {
        root.children[i].num_children = 0;
    }

    int length_max = strlen(string);

    root.num_children=2;

    char  variable[length_max];
    int variable_len=0;

    int state=0;
    for(i=0;i<strlen(string);i++){
        if(state==0 && string[i]=='d'){
            state=1;
            //i++;  // this is for ignoring the next space...
        }
        else if(state==1){
            variable[variable_len++] = string[i];
        }
    }
    variable[variable_len]=0;

    root.children[0].value = "read";
    root.children[1] = variableTree(variable);

    root.left = 0;
    root.right = 1;
    return root;
}

Node printTree(char * string) {
    Node root;
    root.eval = NULL;
    root.null_value = 0;
    root.value = "Pr";
    root.children = (Node *) malloc(sizeof(Node) * 2);
    int i;
    for (i = 0; i < 2; i++) {
        root.children[i].num_children = 0;
    }

    int length_max = strlen(string);

    root.num_children=2;

    char  expression[length_max];
    int expression_len=0;

    int state=0;
    for(i=0;i<strlen(string);i++){
        if(state==0 && string[i]=='t'){
            state=1;
            //i++;  // this is for ignoring the next space...
        }
        else if(state==1){
            expression[expression_len++] = string[i];
        }
    }
    expression[expression_len]=0;
    //printf("estring - %s",string);
   //	printf("print expression = %s\n",expression);
    
    root.children[0].value = "print";
    root.children[1] = expressionTree(expression);
    
    root.left = 0;
    root.right = 1;

    return root;
}


Node whileTree(char * string){
    Node root;
    root.eval = NULL;
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

    root.left = 1;
    root.right = 4;

    return root;
}

Node assignmentTree(char * string){
    Node root;
    root.eval = NULL;
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

    root.left = 0;
    root.right = 2;
    return root;
}

Node expressionTree(char * string){   // no space should be there in string
    Node root;
    root.eval = NULL;
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
        
        root.left = 0;
        root.right = 2;
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
        
        root.left = 0;
        root.right = 2;

    }

    else if(state==0){
        root.children[0] = expression1Tree(string);
        root.num_children=1;
        
        root.left = 0;
        root.right = -1;
    }

    
    return root;
}


Node expression1Tree(char * string){
    Node root;
    root.eval = NULL;
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
        
        root.left = 0;
        root.right = 2;
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
        root.left = 0;
        root.right = 2;
    }
    else if(state==0){
        root.children[0] = termTree(string);
        root.num_children=1;
        root.left = 0;
        root.right = -1;
    }
    
    return root;
}


Node termTree(char * string){
    Node root;
    root.eval = NULL;
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
        root.left = 0;
        root.right = 2;
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
        root.left = 0;
        root.right = 2;
    }
    else if(state==0){
        root.children[0] = factorTree(string);
        root.num_children=1;
        root.left = 0;
        root.right = -1;
    }

    return root;
}


Node factorTree(char * string){
    Node root;
    root.eval = NULL;
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
        
        root.left = 1;
        root.right = -1;
    }
    else if(state1==2){
        root.children[0] = constantTree(string);
        root.num_children=1;
        
        root.left = 0;
        root.right = -1;
    }
    else if(state1==3){
        root.children[0] = variableTree(string);
        root.num_children=1;
        
        root.left = 0;
        root.right = -1;
    }


    return root;
}


Node constantTree(char * string){
    Node root;
    root.eval = NULL;
    root.value = "C";
    root.children = (Node *) malloc(sizeof(Node) * 1);
    root.children[0].num_children=0;
    root.num_children=1;
    root.null_value=0;

    root.children[0].value = (char *) malloc(sizeof(char)*strlen(string));

    strcpy(root.children[0].value,string);

    root.left = 0;
    root.right = -1;
    return root;
}

Node variableTree(char * string){
    Node root;

    root.value = "V";
    root.eval = NULL;
    root.children = (Node *) malloc(sizeof(Node) * 1);
    root.children[0].num_children=0;
    root.num_children=1;
    root.null_value=0;

    root.children[0].value = (char *) malloc(sizeof(char)*strlen(string));

    strcpy(root.children[0].value,string);

    root.left = 0;
    root.right = -1;
    return root;
}

void preorder(Node * root,FILE * f){
    if(root == NULL || root->null_value==1){
        return;
    }

    if(root->value!=NULL) {
        fprintf(f,"%s", root->value);
    }

    int i;
    int state=0;
    for(i=0;i<root->num_children;i++){
        if(&root->children[i] !=NULL){
            fprintf(f,"@");
            state=1;
        }
    }

    for(i=0;i<root->num_children;i++){
        preorder(&root->children[i],f);
    }
    fprintf(f,"#");
}

float strToFloat(char *c){
    char *it = c;
    int size1 = 0,size2 = 0;
    while(isalnum(*it)){
        it++;
        size1++;
    }
    
    it++; 
    
    while(isalnum(*it)){
        it++;
        size2++;
    }

    int n = size1 + size2 + 1;
    int i;
    float ans = 0.0;
    int mult = 1;
    if(size2 == 0) n = size1;
    for(i = n-1;i>=0;i--){
        if(c[i] == '.')
            continue;
        else{
            ans += mult*(c[i] - '0');
            mult *= 10;
        }
    }

    float div = 1.0f;
    for(i = 0; i < size2; i++)
        div *= 10;

    ans /= div;


    return ans;

}




int main() {
    FILE *file = fopen("input.c", "rb");

    fseek (file, 0, SEEK_END);
    long length = ftell (file);
    fseek (file, 0, SEEK_SET);
    char * buffer = malloc (length);
    fread (buffer, 1, length, file);
    fclose (file);

    buffer[length]=0;

    Node root;
    char * buffer_new = (char *) malloc(length);

    int buffer_new_len=0;


    int i;
    for(i=0;i<strlen(buffer);i++){  // check again to add required characters
        if(isalnum(buffer[i]) || buffer[i]=='+' || buffer[i]=='-' || buffer[i]=='*' || buffer[i]=='/' || buffer[i]=='=' || buffer[i]==';' || buffer[i]=='(' || buffer[i]==')' || buffer[i]=='[' || buffer[i]==']' || buffer[i]=='{' || buffer[i]=='}' || buffer[i]=='<' || buffer[i]=='.' ){
            buffer_new[buffer_new_len++] = buffer[i];
        }
    }

    buffer_new[buffer_new_len]=0;



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

    FILE *fout = fopen("output.txt", "w");

    for(po=0;po<100;po++){
       table[po] = malloc(100);  
    }
    
    
    preorder(&root,fout);
    fprintf(fout,"\n");
    postorder(&root);
    fclose(fout);
    
    
printf("\n%s\n",table[0]);
printf("\n%f\n",values[0]);

printf("\n%s\n",table[1]);
printf("\n%f\n",values[1]);

    return 0;
}



// Stack type
struct Stack
{
    int size;
    int top;
    //struct _Node* children;
    struct _Node* *array;
};

struct Stack* createStack(int size)
{
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    stack->size = size;
    stack->top = -1;
    stack->array = (struct _Node**) malloc(stack->size * sizeof(struct _Node*));
    return stack;
}
 
// BASIC OPERATIONS OF STACK
int isFull(struct Stack* stack)
{  return stack->top - 1 == stack->size; }
 
int isEmpty(struct Stack* stack)
{  return stack->top == -1; }
 
void push(struct Stack* stack, struct _Node* node)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = node;
}
 
struct _Node* pop(struct Stack* stack)
{
    if (isEmpty(stack))
        return NULL;
    return stack->array[stack->top--];
}
 
struct _Node* peek(struct Stack* stack)
{
    if (isEmpty(stack))
        return NULL;
    return stack->array[stack->top];
}

// An iterative function to do postorder traversal of a given binary tree
void postorder(Node* root)
{
    
    if (root == NULL)
        return;
    
    struct Stack* stack = createStack(MAX_SIZE);

    do
    {
        while (root)
        {
           
            if (root->right!=-1 && &root->children[root->right]!=NULL)
                push(stack, &root->children[root->right]);
            push(stack, root);
            root = &root->children[root->left];
                	
        }

       
        root = pop(stack);
 
       
        if (&root->children[root->right] && peek(stack) == &root->children[root->right])
        {
            pop(stack);  
            push(stack, root);  
            root = &root->children[root->right]; 
                                
        }
        else  
        {
        	int i=0;
		printf("Node - > %s \n", root->value);
		
		if(root->num_children == 0){

		   root->eval = root->value;

		   printf("assigned value = %s\n",root->value);
		   printf("when no children %s \n",root->eval);
		}
		else{
		
		    int cnt=0,sing=0,doub=0;
		    for(i=0;i<root->num_children;i++){
		    
			if(&root->children[i] !=NULL && (root->children[i].eval !=NULL)){
			
			    if(cnt == 0){
			      sing = i;
			    }
			    else{
			      doub = i;
			      
			    }
			    cnt++;
			    printf("cnt value = %d\n", cnt);

			printf("node---- value = %s \n", root->children[i].value);
			}
			
		    }	
		    	 printf("count value =  %d \n",cnt);
		   if(cnt == 1){
		     
		     
		     
		     root->eval = root->children[sing].eval;
		     printf("root->eval haha =%s", root->eval);
		   	
		   }
		   else if(cnt == 2){
		   
				     float temp1=0.0f,temp2=0.0f,temp3=0.0f;
				     char tem[100];
				     int chk=0;
				     

				     if (root->children[sing].eval[0] > 48 && root->children[sing].eval[0] <57 ){

				     	
				     	temp1=strToFloat(root->children[sing].eval);
				     	
				     	

				     }
				     else{
				    
				                        
					for(i=0;i<100;i++){
						
						if(table[i]!=NULL){
						int tyr = 0;
						tyr = strcmp(root->children[sing].eval, table[i]);

				                     if(tyr == 0){
				                        
							temp1 = values[i];
							printf("final= %6f\n",temp1);
							chk=1;
							break;
							}
						}			        
				
					}   
				     }


				     if (root->children[doub].eval[0] >= 48 && root->children[doub].eval[0] <=57 ){
				     	

				     	temp2=strToFloat(root->children[doub].eval);
				     	
				     }
				     else{
				     
				     	
					for(i=0;i<100;i++){
						int tyr = 0;
						tyr = strcmp(root->children[doub].eval, table[i]);

					   if(table[i]!=NULL){
						if(tyr == 0){
				
							temp2 = values[i];
							break;
						}			        
				           }
					}   
				     }
		     
		     
		  if(root->children[sing+1].value == "+"){

		     temp3 = temp1+temp2;
		     printf("addition= %6f\n",temp3);
		     	  char lol[100];
			  snprintf(lol, 100, "%f", temp3);
			  root->eval = (char *) malloc(sizeof(char)*100);
			  strcpy(root->eval,lol);
		  }
		  else if(root->children[sing+1].value == "-"){
		     temp3 = temp1-temp2;
		          char lol[100];
			  snprintf(lol, 100, "%f", temp3);
			  root->eval = (char *) malloc(sizeof(char)*100);
			  strcpy(root->eval,lol);

		  }
		  else if(root->children[sing+1].value == "*"){
		     temp3 = temp1*temp2;
		          char lol[100];
			  snprintf(lol, 100, "%f", temp3);
			  root->eval = (char *) malloc(sizeof(char)*100);
			  strcpy(root->eval,lol);

		  } 
		  else if(root->children[sing+1].value == "/"){
		     temp3 = temp1/temp2;
		     	  char lol[100];
			  snprintf(lol, 100, "%f", temp3);
			  root->eval = (char *) malloc(sizeof(char)*100);
			  strcpy(root->eval,lol);
			  
		  }
		  else if(root->children[sing+1].value == "="){
		    printf("final* =%s\n", root->children[sing].eval);
		    printf("final*= %f\n",temp2);
		    
		    int bolo = 0;
		    
		    for(i=0; i <gIndex; i++){
		    
		    if(!strcmp(table[i],root->children[sing].eval)){
		    	
		    	values[i] = temp2;
		    	bolo =1;
		    	break;
		    }		     
		    
		    }
		    
		    if(bolo==0){
			
			table[gIndex] = root->children[sing].eval;
           		values[gIndex] = temp2;
		        gIndex++; 
		    		    
		    }
		    
		        
		  }
		  else if(root->children[sing].value == "print"){
		    
		    printf("**printing**= %f\n",temp2);
		  }
		  else if(root->children[sing].value == "read"){
		  
		   float val = 0.0f;
		   
		   scanf("%f",&val);
		   table[gIndex] = root->children[doub].eval;
		   values[gIndex] = val;
		   gIndex++; 
		  
		  
		  }  
		  
		  
		   }//cnt == 2	
		}
		
		
		printf("\n");
            root = NULL;
        }
    } while (!isEmpty(stack));
}


