#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

typedef struct _Node{
    char * value;
    struct _Node* children;
} Node;


int num_statements=0;


char variableTable[5000][2];


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



int isValidCharacter(char c){   // if anything is missed, add... . No requirement of space character(confirmation required)
    if((c >='0' && c<='9') || (c=='+') || (c=='-') || (c=='*') || (c=='/') || (c=='<') || (c=='=') || (c=='.') ||(c=='(') || (c==')') ||(c=='[') || (c==']') || (c>='a' && c<='z'))
        return 1;
    return 0;
}

Node programTree(char* string){
    Node root;

    if(string==NULL) return root;

    root.value="P";
    root.children = (Node*)malloc(sizeof(Node)*2);

    char * token1 = strtok(NULL,";");
    root.children[0] = statementTree(token1);

    char * token2 = strtok(NULL,"");
    root.children[1] = programTree(token2);

    return root;
}


Node statementTree(char * string){
    Node root;

    root.value="S";
    root.children = (Node*)malloc(sizeof(Node)*1);

    int len = strlen(string);
    int equal_state=0;
    int while_state=0;

    if(strstr(string,"while")){  // while loop
        while_state=1;
    }
    if(while_state==0) {
        for (int i = 0; i < len; i++) {
            if (string[i] == '=') {   //assignment or == in expression
                equal_state=1;
                break;
            }
        }
    }

    if(while_state==1){
        root.children[0] = whileTree(string);
    }
    else if(equal_state==1){
        root.children[0] = assignmentTree(string);
    }

    return root;
}

Node whileTree(char * string){
    Node root;

    root.value="W";
    root.children = (Node*)malloc(sizeof(Node)*7);

    char  expression[100];
    int expression_len=0;
    char statement[100];
    int statement_len=0;


    int brackcount=0;

    int state=0;

    for(int i=0;i<strlen(string);i++){
        if(isValidCharacter(string[i])) {
            if (string[i] == '(') {             // here bracket matching use stack or counter !!! V-Imp
                state = 1;
                brackcount++;
            } else if (state == 1 && brackcount > 1) {
                expression[expression_len++] = string[i];
            } else if (state == 1 && brackcount == 1 && string[i] != ')') {
                expression[expression_len++] = string[i];
            } else if (state == 1 && string[i] == ')' && brackcount == 1) {
                state = 0;
                break;
            }
        }
    }

    for(int i=0;i<strlen(string);i++){
        if(string[i] == '{'){
            state=1;
        }
        else if(state==1 && string[i]!='}'){
            statement[statement_len++] = string[i];
        }
        else if(string[i]=='}'){
            break;
        }
    }



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

    char expression[100];
    int expression_len=0;

    char variableName[100];
    int variableLen=0;

    int state=0;
    for(int i=0;i<strlen(string);i++){
        if(isValidCharacter(string[i])) {
            if (string[i] == '=') {
                state = 1;
            } else if (state == 1) {
                expression[expression_len++] = string[i];
            }
        }
    }

    for(int i=0;i<strlen(string);i++){
        if(string[i]=='='){
            break;
        }
        else if(string[i]!=' '){
            variableName[variableLen++] = string[i];
        }
    }

    //for children[0] variable table required?

    root.children[0].value = variableName;
    root.children[1].value = "=";
    root.children[2] = expressionTree(expression);


    return root;
}

Node expressionTree(char * string){   // no space should be there in string
    Node root;

    root.value="E";
    root.children = (Node*)malloc(sizeof(Node)*3);


    int brackcount=0;
    int state=0;  // 1 - <,2 - ==
    int indexofoperator=-1;

    int prev_equal=0;

    for(int i=0;i<strlen(string);i++){
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



    char str1[100];    // careful on lengths (no of characters+1 -- due to /0)
    char str2[100];

    if(state==1){
        for(int i=0;i<indexofoperator;i++){
            str1[i] = string[i];
        }
        for(int i=indexofoperator+1;i<strlen(string);i++){
            str2[i] = string[i];
        }

        root.children[0] = expression1Tree(str1);
        root.children[1].value = "<";
        root.children[2] = expressionTree(str2);

    }
    else if(state==2){
        for(int i=0;i<indexofoperator-1;i++){
            str1[i]=string[i];
        }
        for(int i=indexofoperator+1;i<strlen(string);i++){
            str2[i]=string[i];
        }

        root.children[0] = expression1Tree(str1);
        root.children[1].value = "==";
        root.children[2] = expressionTree(str2);

    }

    else if(state==0){
        root.children[0] = expression1Tree(string);
    }

    return root;
}


Node expression1Tree(char * string){
    Node root;

    root.value="E1";
    root.children = (Node*)malloc(sizeof(Node)*3);

    int state=0;    //1 - +,2 - -
    int indexofoperator=-1;
    int brackcount=0;

    for(int i=0;i<strlen(string);i++){
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

    char str1[100];
    char str2[100];

    if(state==1){
        for(int i=0;i<indexofoperator;i++){
            str1[i]=string[i];
        }
        for(int i=indexofoperator+1;i<strlen(string);i++){
            str2[i]=string[i];
        }

        root.children[0] = termTree(str1);
        root.children[1].value = "+";
        root.children[2] = expression1Tree(str2);
    }
    else if(state==2){
        for(int i=0;i<indexofoperator;i++){
            str1[i]=string[i];
        }
        for(int i=indexofoperator+1;i<strlen(string);i++){
            str2[i]=string[i];
        }

        root.children[0] = termTree(str1);
        root.children[1].value = "-";
        root.children[2] = expression1Tree(str2);
    }
    else if(state==0){
        root.children[0] = termTree(string);
    }

    return root;
}


Node termTree(char * string){
    Node root;

    root.value="T";
    root.children = (Node*)malloc(sizeof(Node)*3);

    int state=0;    //1 - *,2 - /
    int indexofoperator=-1;
    int brackcount=0;

    for(int i=0;i<strlen(string);i++){
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

    char str1[100];
    char str2[100];

    if(state==1){
        for(int i=0;i<indexofoperator;i++){
            str1[i]=string[i];
        }
        for(int i=indexofoperator+1;i<strlen(string);i++){
            str2[i]=string[i];
        }

        root.children[0] = factorTree(str1);
        root.children[1].value = "*";
        root.children[2] = termTree(str2);
    }
    else if(state==2){
        for(int i=0;i<indexofoperator;i++){
            str1[i]=string[i];
        }
        for(int i=indexofoperator+1;i<strlen(string);i++){
            str2[i]=string[i];
        }

        root.children[0] = factorTree(str1);
        root.children[1].value = "/";
        root.children[2] = termTree(str2);
    }
    else if(state==0){
        root.children[0] = factorTree(string);
    }

    return root;
}


Node factorTree(char * string){
    Node root;

    root.value = "F";
    root.children = (Node *) malloc(sizeof(Node) * 3);

    int state=0;
    int state1=0;    // 1-(),2-C,3-V
    int brackcount=0;

    int str1[100];

    for(int i=0;i<strlen(string);i++){
        if(string[i]=='('){
            state=1;
            state1=1;
            brackcount++;
        }
        else if(state==1 && brackcount>1){
            str1[i]=string[i];
        }
        else if(state==1 && brackcount==1 && string[i]!=')'){
            str1[i]=string[i];
        }
        else if(state==1 && brackcount==1 && string[i]==')'){
            state=0;
        }
    }

    if(state1==0){
        int err_state1=0;
        for(int i=0;i<strlen(string);i++){
            if(!isalpha(string[i])){
                err_state1=1;
                break;
            }
        }
        if(err_state1==0) state1=3;
    }

    if(state1==0){
        int err_state1=0;
        for(int i=0;i<strlen(string);i++){
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
    }
    else if(state1==3){
        root.children[0] = variableTree(string);
    }


    return root;
}


Node constantTree(char * string){
    Node root;

    root.value = "C";
    root.children = (Node *) malloc(sizeof(Node) * 1);

    root.children[0].value = string;

    return root;
}

Node variableTree(char * string){
    Node root;

    root.value = "V";
    root.children = (Node *) malloc(sizeof(Node) * 1);

    root.children[0].value = string;

    return root;
}

int main() {

    FILE *file = fopen("test.txt", "rb");

    fseek (file, 0, SEEK_END);
    long length = ftell (file);
    fseek (file, 0, SEEK_SET);
    char * buffer = malloc (length);
    fread (buffer, 1, length, file);
    fclose (file);

    Node root;

    char * token = strtok(buffer,";");

    root = programTree(token);
    printf("something\n");
    //preorder traversal

    // this is a test - Its a trap
    // conflict checking

    return 0;
}



