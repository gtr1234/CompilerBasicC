# CompilerBasicC

Group Members:

	           1) A.V.S Karthik(2014CSB1005)
	       	   2) M.Akhil Avinash(2014CSB1019)
	       	   3) Eswar Dev Harsha(2014CSB1023)
	           4) Nikhil Kumar Pulipeta(2014CSB1025)
	           5) Shiva Sutapalli(2014CSB1037)	

To compile: gcc pa3.c
To run: ./a.out [input filename]

Output: The Pre-order traversal of the tree constructed will be written to "output.txt" file 
		The Evaluated values/results from the input program file will be shown in command prompt.


The Grammar for while is updated now, it can have multiple statements.

Approach: We evaluated the input program using the Postorder traversal of the tree contructed where the postorder tranversal is implemented
		  using a stack. 

NOTE :     1. read can be used only with a variable whereas print can be used along with variables,expressions and constants.
	   2. We need to end the while loop with a ';'
	   3. We assumed the the input program is syntactically correct.
	   4. Please Do not use the Alphabets used for Grammar identification as Variables in the input file.
	   5. The Alphabets used for Grammar identification include as follows:
	   
	   	1. P - Program
	   	2. S - Statement
	   	3. R - Read
	   	4. Pr- Print
	   	5. A - Assignment
	   	6. W - While
	   	7. E - Expression
	   	8. E1- Expression1
	   	9. T - Term
	        10. F - Factor
	        11. C - Constant ({0-9}+.{0-9}k)
	        12. V - Variable ({a-z}+) 

Grammar:

P -> S;P|S
S -> A|W|R|Pr
R -> read V
Pr -> print E
A -> V = E
W -> while(E){P};
E -> E1 < E 
    |E1 == E 
    |E1
E1-> T + E1
    |T - E1
    |T
T -> F*T
    |F/T
    |F
F -> ( E ) | C | V
V -> {a-z}+
C -> {0-9}+.{0-9}k

 
