typedef int boolean;
#define FALSE 0
#define TRUE !FALSE
#define MAXSTR 80
#define MAXNUMQS 32
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "animal.h"
char Write(TreeType tree);

/* Represent the tree as an explicitly linked tree */

struct treeStruct {
    char *string;
    struct treeStruct *left, *right;
};

struct positionStruct {
    struct treeStruct* node;
};

int main (int argc, char *argv[])
{
    char *treefile = NULL;
    TreeType tree;
    PositionType pos;
    char *newQuestion, *newAnswer;
    
    if (argc > 1) {
        treefile = argv[1];
    }
    tree = InitTree (treefile);
    
    printf("%s", "Think of an animal. I will try to guess what it is.\n"
           "Please answer my questions with yes or no.\n");
    
    
    while (TRUE) {
        pos = Top (tree);
        while (!IsLeaf (tree, pos)) {
            pos = Answer(Question(tree,pos))?
            YesNode(tree,pos): NoNode(tree,pos);
        }
        if (Answer (Guess (tree, pos))) {
            printf ("I got it right!\n");
        } else {
            GetNewInfo (tree, pos, &newAnswer, &newQuestion);
            ReplaceNode (tree, pos, newAnswer, newQuestion);
        }
        if (!Answer ("Want to play again? ")) {
            WriteTree(tree, treefile);
            exit (0);
        }
    }
    
    
}


TreeType InitTree (char *file){
    int i;
    TreeType tree, left, right;
    FILE *inFile;
    inFile = fopen (file, "r") ;
    
    if (inFile == NULL) {
        
        tree = malloc(sizeof(TreeType) + MAXSTR);
        right = malloc(sizeof(TreeType) + MAXSTR);
        left = malloc(sizeof(TreeType) + MAXSTR);
        tree->string = malloc(MAXSTR + 1);
        right->string = malloc(MAXSTR + 1);
        left->string = malloc(MAXSTR + 1);
        
        strcpy(tree->string, "Is it funny?");
        strcpy(left->string, "giraffe");
        strcpy(right->string, "cat");
        
        tree->left = left;
        tree->right = right;
        tree->left->left = NULL;
        tree->right->left = NULL;
        tree->left->right = NULL;
        tree->right->right = NULL;
        
    }
    
    return tree;
}

void PrintTree(TreeType tree)
{
    if (tree->string==NULL){
    }
    else {
        printf("%s\n",tree->string);
        PrintTree(tree->left);
        PrintTree(tree->right);
    }
}

void WriteTree (TreeType tree, char *file){
    int i;
    FILE *inFile;
    inFile = fopen (file, "w") ;
    
    if (inFile == NULL){
    }
    
    else
    {
        if(tree->string != NULL){
            fprintf (inFile, "\n\n%s", tree->string);
            if(tree->left){
            WriteTree(tree->left,file);
            }
            if(tree->right){
            WriteTree(tree->right,file);
            }
        }
    }
    fclose(inFile);
}



PositionType Top (TreeType tree){
    
    PositionType pos;
    pos = malloc(sizeof(PositionType));
    
    pos->node = tree;
    
    return pos;
    
}

boolean IsLeaf (TreeType tree, PositionType pos){
    if ((pos->node->left == NULL)&&(pos->node->right == NULL)){ //if yes and no answers for string at pos are NULL, the string is a guess rather than question
		return 1;
	}
    else {
		return 0;
	}
}

boolean Answer (char *q){
    printf("%s\n", q);
    char ans[4];
    scanf("%s",ans);
    char yes[] = "y";
    if (ans[0] == yes[0]) {
		return 1;
	} else {
        return 0;
	}
}
char *Question (TreeType tree, PositionType pos){
    return pos->node->string;
}
char *Guess (TreeType tree, PositionType pos){
    printf("Is it a(n) ");
    return pos->node->string;
}
PositionType YesNode (TreeType tree, PositionType pos){
    
    pos->node = pos->node->left;
    
    return pos;
}
PositionType NoNode (TreeType tree, PositionType pos){
    
    pos->node = pos->node->right;
    
    return pos;
}

void ReplaceNode (TreeType tree, PositionType pos, char *newA, char *newQ){
    
    TreeType yesNode;
    yesNode = malloc(sizeof(TreeType) + MAXSTR);
    TreeType noNode;
    noNode = malloc(sizeof(TreeType) + MAXSTR);
    noNode->string = malloc(MAXSTR + 1);
    yesNode->string = malloc(MAXSTR + 1);
    
    strcpy(yesNode->string, pos->node->string);
    strcpy(noNode->string, newA);
    strcpy(pos->node->string, newQ);
    
    pos->node->left = yesNode;
    pos->node->right = noNode;
    yesNode->left = NULL;
    yesNode->right = NULL;
    noNode->left = NULL;
    noNode->right = NULL;
    
}

void GetNewInfo (TreeType tree, PositionType pos, char **newA, char **newQ) {
    *newA = (char *) malloc(MAXSTR);
    *newQ = (char *) malloc(MAXSTR);
    printf("I give up. What is it?\n");
    scanf("%s",*newA);
    fgets(*newQ, MAXSTR, stdin);
    printf("Provide a question whose answer is yes for %s and no for %s.\n",pos->node->string,*newA);
    fgets(*newQ, MAXSTR, stdin);
    
}
