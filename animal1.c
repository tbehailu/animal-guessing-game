typedef int boolean;
#define FALSE 0
#define TRUE !FALSE
#define MAXSTR 80
#define MAXNUMQS 32
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "animal.h"

//represent the tree in conventional C fashion as an array of character pointers

struct treeStruct {
    char *nodes[MAXNUMQS];
};

struct positionStruct {
    int nodeIndex;
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

TreeType InitTree (char *file) {
    int i;
    FILE *inFile;
    inFile = fopen (file, "r") ;
    TreeType tree;
    tree = malloc(MAXNUMQS*sizeof(TreeType));
    
    if (inFile == NULL) {
        
        for(i = 0; i < MAXNUMQS; ++i)
        {
            tree->nodes[i] = (char *) malloc(MAXSTR+1);
            strcpy(tree->nodes[i], "\0");
        }
        
        strcpy(tree->nodes[0], "Is it funny?");
        strcpy(tree->nodes[1], "giraffe");
        strcpy(tree->nodes[2], "cat");
        
        return tree;
    }
    
    else {
        char line[MAXSTR];
        int i=0;
        int j=0;
        
        for(i = 0; i < MAXNUMQS; ++i)
        {
            tree->nodes[i] = (char *) malloc(MAXSTR+1);
            strcpy(tree->nodes[i], "\0");
        }
        
        while ((!feof(inFile))) {
            tree->nodes[j] = (char *) malloc(MAXSTR+1);
            fscanf(inFile, "%[^\n]\n", line);
            strcpy(tree->nodes[j], line);
            j++;
        }
        
        for (i=0; i<MAXNUMQS; i++) {
//            printf("TREE %s\n",tree->nodes[i]);
        }
        
        fclose(inFile);
        
        return tree;
        
        
    }
    
}

/*
 * Writes a tree to the given file (if specified).
 */

void WriteTree (TreeType tree, char *file){
    int i;
    FILE *inFile;
    inFile = fopen (file, "w") ;
    
    if (inFile == NULL) {
    }
    
    else
    {
        for(i = 0; i < MAXNUMQS; ++i)
        {
            fprintf (inFile, "%s\n", tree->nodes[i]);
        }
    }
    
}

void PrintTree(TreeType tree)
{
    int i;
    
    for(i = 0; i < MAXNUMQS; ++i)
    {
        printf("%s\n",tree->nodes[i]);
    }
}

PositionType Top (TreeType tree)
{
    PositionType pos;
    pos = malloc(sizeof(PositionType));
    (*pos).nodeIndex = 0;
    return pos;
}

/*
 *	Return true exactly when pos is a "leaf" of the animal tree,
 *	that is, when the string stored at pos is a guess rather than
 *	a question.
 */

boolean IsLeaf (TreeType tree, PositionType pos) {
    int i,j;
    i = (pos->nodeIndex)*2+1;
    j = (pos->nodeIndex)*2+2;
    if ((strcmp(tree->nodes[i], "\0") == 0)&&(strcmp(tree->nodes[j], "\0") == 0)){ //if yes and no answers for string at pos are NULL, the string is a guess rather than question
		return 1;
	}
    else {
		return 0;
	}
}

boolean Answer (char *q) {
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
    return (*tree).nodes[(*pos).nodeIndex];
}

char *Guess (TreeType tree, PositionType pos) {
    printf("Is it a(n) ");
    return (*tree).nodes[(*pos).nodeIndex];
}

PositionType YesNode (TreeType tree, PositionType pos) {
    pos->nodeIndex = (pos->nodeIndex)*2+1;
    return pos;
}

PositionType NoNode (TreeType tree, PositionType pos) {
    pos->nodeIndex = (pos->nodeIndex)*2+2;
    return pos;
}

void ReplaceNode (TreeType tree, PositionType pos, char *newA, char *newQ) {
    int i,j,k;
    i = (pos->nodeIndex)*2+1;
    j = (pos->nodeIndex)*2+2;
    k = pos->nodeIndex;
    tree->nodes[i] = tree->nodes[k]; //yes answer is original quess
    tree->nodes[k] = newQ; //original position is new question
    tree->nodes[j] = newA; //no answer is new answer
}

void GetNewInfo (TreeType tree, PositionType pos, char **newA, char **newQ) {
    *newA = (char *) malloc(MAXSTR);
    *newQ = (char *) malloc(MAXSTR);
    printf("I give up. What is it?\n");
    scanf("%s",*newA);
    fgets(*newQ, MAXSTR, stdin);
    printf("Provide a question whose answer is yes for %s and no for %s.\n",(*tree).nodes[(*pos).nodeIndex],*newA);
    fgets(*newQ, MAXSTR, stdin);
    
}