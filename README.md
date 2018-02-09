# Chess

As a school project, I created a chess program which could run on an Arduino microcontroller and output on a small LCD screen.
The object of the program was to be able to allow two human players to play through a standard game of chess by inputting their
moves through inputs on the screen using a set of buttons.

The program needed to be able to determine whether a given move held up under the rules of chess. This became the bulk of the
program, with the program needing to keep track of all the different pieces, determine whether the move pattern was legal given
the piece type, determine whether a player was in check, etc.

A natural progression from this is to extend the capability from allowing a legal game between two human players to the ability for
the program to select moves on its own. To this end, I have worked on moving the program from C++ to Python, making it more efficient
and outfitting it to make selections for itself. From there, I will use neural networks to give the program an artificial intelligence
which can improve through play.
