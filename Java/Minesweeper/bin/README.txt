Rafael Gehrke  /-/  PennKey: gehrke

(Minesweeper)
1~
	A - (About the Game.java)
	The difference from the Demo file is that I included the instructions in this part. 
	When the game startsit prompts the instructions as a messageDialog box.
	The reset button was preserved.
	
	B - (About the GameCourt.java)
	It will randomly distribute the bombs at the beginning of the game. the gameStatus is
	updated regarding the remaining time left and the number of open tiles 
	(if time is over or the user opened a bomb or opened all the empty tiles he either won or lost).
	After winning or losing an InputDialog Box will open with current highscores (after the program
	read the txt file and ordered the scores) and will request the user to put the username. After this
	the txt file with highscore is updated. To play again the user press the reset button.
	
	C - (About the GameObj.java)
	The abstract class that was given in the Demo. I deleted some parts that I would not use. 
	The class supports setting x-y coordinates and vx-vy speeds for objects, detect intersection between
	objects (used to detect superposition of Selector and a GridElement) and move objects using their
	speeds.
	
	D - (About the Selector.java)
	It extends the GameObj and is responsible for the way that the user will interact with the game.
	We create one Selector object that will represent which tile the user wants to perform an action on.
	
	E - (About the GridElement.java)
	(Extends gameObj) It represents a tile in the MineSweeper grid. It stores variables to know if it is 
	marked/unmarked open/closed and	repaint itself accordingly. There are also setters and getters for the
	 different states previously mentioned.
	
	F - (About the ShowTime.java)
	(Extends gameObj) It is responsible for showing the remaining time of the game.
	
	G - (About Top.java)
	(Extends gameObj) It is responsible for showing the image above the Grid.
	
2~ Game concepts

	A - 2-D Arrays
	At the GameCourt class I create a 2-D array of GridElements in order to easily keep track of
	what tiles are open/closed marked/unmarked and apply recursion to open the tiles. 
	Also, since they are displaced side by side in the frame, a 2-D array is a great abstraction.
	
	B - Recursion
	When a tile with no near bombs is open, all the near (8 maximum) tiles should be opened.
	This condition also applies to the newly opened tiles. To implement this I used recursion in at the
	GameCourt.java.
	
	C - File I/O
	In order to keep track of past high scores and their owners, there is the scores.txt file. 
	After the game ends at the GameCourt, the code reads the txt file, include the current user's score,
    sort them and display to the user. After that it updates the scores.txt file. The scores.txt does not
    store the sorted data.
	
	
	D - Testable component
	
	

