Animal-Game
===========

Animal is a computer game in which the program tries to guess what animal the player is thinking of. If the program's guess is correct, it will print a suitably celebratory message. If it guessed wrong, it asks for a question that will differentiate between its guess and the correct answer. The program then updates its list of questions and answers appropriately. 

The animal program internally maintains a tree of questions and answers. 

Implementation 1 (animal1.c): Represents the tree in conventional C fashion as an array of character pointers.

Implementation 2 (animal2.c): Represent the tree as an explicitly linked tree.
