/**
 * CIS 120 Game HW
 * (c) University of Pennsylvania
 * @version 2.1, Apr 2017
 */

import java.awt.*;
import java.io.*;
import java.util.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.Timer;

/**
 * GameCourt
 * 
 * This class holds the primary game logic for how different objects interact with one another. Take
 * time to understand how the timer interacts with the different methods and how it repaints the GUI
 * on every tick().
 */
@SuppressWarnings("serial")
public class GameCourt extends JPanel {

    // the state of the game logic
    private Selector selector;
    private GridElement poison[][] = new GridElement[11][18]; // the Poison Mushroom, doesn't move
    private Top top;
    public boolean playing = false; // whether the game is running 
    private JLabel status; // Current status text, i.e. "Running..."
    private int gameTime = 300000; //total game time
    private ShowTime myTimer = new ShowTime("Test", 720, 740);
    // Game constants
    public static final int COURT_WIDTH = 720;
    public static final int COURT_HEIGHT = 740;
    public static final int SQUARE_VELOCITY = 40;
    private int bombCount = 0;
    private boolean openVar = false;
    private boolean markVar = false;
    private boolean unMarkVar = false;
    private int openPlaces = 0;
    private String myScoresFile = "src/scores.txt";

    // Update interval for timer, in milliseconds
    public static final int INTERVAL = 100;

    public GameCourt(JLabel status) {
        // creates border around the court area, JComponent method
        setBorder(BorderFactory.createLineBorder(Color.BLACK));

        // The timer is an object which triggers an action periodically with the given INTERVAL. We
        // register an ActionListener with this timer, whose actionPerformed() method is called each
        // time the timer triggers. We define a helper method called tick() that actually does
        // everything that should be done in a single timestep.
        Timer timer = new Timer(INTERVAL, new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                tick();
            }
        });
        timer.start(); // MAKE SURE TO START THE TIMER!

        // Enable keyboard focus on the court area.
        // When this component has the keyboard focus, key events are handled by its key listener.
        setFocusable(true);

        // This key listener allows the square to move as long as an arrow key is pressed, by
        // changing the square's velocity accordingly. (The tick method below actually moves the
        // square.)
        addKeyListener(new KeyAdapter() {
        	
        	public void keyReleased(KeyEvent e) {
                selector.setVx(0);
                selector.setVy(0);
                openVar = false;
                markVar = false;
                unMarkVar = false;
            }
        	
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_LEFT) {
                    selector.setVx(-SQUARE_VELOCITY);
                } else if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
                    selector.setVx(SQUARE_VELOCITY);
                } else if (e.getKeyCode() == KeyEvent.VK_DOWN) {
                    selector.setVy(SQUARE_VELOCITY);
                } else if (e.getKeyCode() == KeyEvent.VK_UP) {
                    selector.setVy(-SQUARE_VELOCITY);
                }
                else if (e.getKeyChar() == 's') {
                	openVar = true;
                }
                else if (e.getKeyChar() == 'd') {
                	markVar = true;
                }
                else if (e.getKeyChar() == 'f') {
                	unMarkVar = true;
                }
            }

        });

        this.status = status;
    }

    /**
     * (Re-)set the game to its initial state.
     */
    public void reset() {
    	openVar = false;
        markVar = false;
        unMarkVar = false;
    	bombCount = 0;
        selector = new Selector(COURT_WIDTH, COURT_HEIGHT, Color.RED);
        top = new Top(COURT_WIDTH, COURT_HEIGHT);
        gameTime = 300000;
        openPlaces = 0;
        for(int i = 0; i < 11; i++) {
        	for (int j = 0; j < 18; j++) {
        		poison[i][j] = new GridElement(40 * j,300 + i * 40, COURT_WIDTH, COURT_HEIGHT);
        	}
        }

        playing = true;
        status.setText("Running...");

        // Make sure that this component has the keyboard focus
        requestFocusInWindow();
    }
    
    public boolean recursionOverCoords(int i, int j, GridElement[][] matrixElement) {
    	if(!matrixElement[i][j].getIsOpen()) {
    		matrixElement[i][j].setIsOpen(true);
    		if(matrixElement[i][j].hasBomb()) {
    			return false;
    		}
    		openPlaces++;
    		boolean accumulator = true;
    		if(matrixElement[i][j].getNumNear() == 0) {
    			for(int k = Math.max(0, i-1); k <= Math.min(i + 1, matrixElement.length - 1); k++) {
					for(int l = Math.max(0, j-1); l <= Math.min(j + 1, matrixElement[0].length - 1); l++) {
						accumulator = accumulator && recursionOverCoords(k, l, matrixElement);
					}
				}
    		}
    		return accumulator;
    	}
    	return true;
    }

    /**
     * This method is called every time the timer defined in the constructor triggers.
     */
    void tick() {
        if (playing) {
        	if (gameTime == 300000) { //beginning. put bombs
        		//int cycles = 0;
        		while(bombCount != 20) {
        			int randomLine = (int) (11 * Math.random());
        			int randomColumn = (int) (18 * Math.random());
        			if (!poison[randomLine][randomColumn].hasBomb()) {
        				poison[randomLine][randomColumn].putBomb();
        				bombCount ++;
        			}
        			//cycles ++;
        		}
        		for(int i = 0; i < 11; i++) {
        			for(int j = 0; j < 18; j++) {
        				//for a given unit in i,j, go over neighbors
        				for(int k = Math.max(0, i-1); k <= Math.min(i + 1, 10); k++) {
        					for(int l = Math.max(0, j-1); l <= Math.min(j + 1, 17); l++) {
        						if(poison[k][l].hasBomb()) {
        							poison[i][j].addNumNear();
        						}
        					}
        				}
        			}
        		}
        	}
            selector.move();
            gameTime -= INTERVAL;
            myTimer = new ShowTime (Integer.toString(gameTime/1000), COURT_WIDTH, COURT_HEIGHT);

            if (gameTime == 0) {
                playing = false;
                status.setText("You lose!");
            }
            for(int i = 0; i < 11; i++) {
            	for(int j = 0; j < 18; j++) {
            		if(selector.intersects(poison[i][j]) && openVar == true) {
            			playing = recursionOverCoords(i, j, poison);
            			if (playing == false) {
            				status.setText("You lose!");
            			}
            		}
            	}
            }
            for(int i = 0; i < 11; i++) {
            	for(int j = 0; j < 18; j++) {
            		if(selector.intersects(poison[i][j]) && markVar == true && poison[i][j].getIsOpen() == false) {
            			poison[i][j].setIsMarked(true);
            		}
            	}
            }
            for(int i = 0; i < 11; i++) {
            	for(int j = 0; j < 18; j++) {
            		if(selector.intersects(poison[i][j]) && unMarkVar == true && poison[i][j].getIsOpen() == false) {
            			poison[i][j].setIsMarked(false);
            		}
            	}
            }
            
            if (openPlaces ==178) {
                playing = false;
                status.setText("You win!");
            }
            
            // update the display
            repaint();
            if (playing == false) {
            	String userName = "TYPE_YOUR_NAME";
            	String toPrint = null;
            	FileReader fileRead = null;
            	String oldFile = "";
            	int finalScore;
            	if (openPlaces != 178) {
            		finalScore = openPlaces;
            	}
            	else {
            		finalScore = openPlaces + (int) gameTime/1000;
            	}
            	ArrayList<String> data = new ArrayList<String>();
            	data.add(userName + " " + finalScore);
				try {
					fileRead = new FileReader(myScoresFile);
				} catch (FileNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            	BufferedReader buffRead = new BufferedReader(fileRead); 
            	try {
					toPrint = buffRead.readLine();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            	
            	while(toPrint != null) {//getting all the lines of code
            		data.add(toPrint);														
            		try {
    					toPrint = buffRead.readLine();
    				} catch (IOException e) {
    					// TODO Auto-generated catch block
    					e.printStackTrace();
    				}
            	}
            	
            	for(int i = 1; i < data.size(); i++) {
            		oldFile += data.get(i) + "\n";
            	}
            	String[] scoreData = new String[data.size()];
            	for(int i = 0; i < data.size(); i++) {
            		scoreData[i] = data.get(i);
            	}
            	int[] scoreNumbers = new int[data.size()];
            	char[][] niceData = new char[data.size()][];
            	for(int i = 0; i < data.size(); i++) {
            		niceData[i] = scoreData[i].toCharArray();
            	}
            	
            	for(int i = 0; i < data.size(); i++) {
            		for(int j = niceData[i].length - 1; niceData[i][j] != ' '; j--) {
            			scoreNumbers[i] += ((int) niceData[i][j] - '0') * (Math.pow(10, niceData[i].length - 1 - j));
            		}
            	}
            	
            	int[] indexOrder = new int[data.size()];
            	for(int i = 0; i < data.size(); i++) {
            		int indexHighScore = -1;
            		int highScore = -1;
            		for(int j = 0; j < data.size(); j++) {
            			if(scoreNumbers[j] > highScore) {
            				highScore = scoreNumbers[j];
            				indexHighScore = j;
            			}
            		}
            		scoreNumbers[indexHighScore] = -1;
            		indexOrder[i] = indexHighScore;
            	}
            	
            	toPrint = "HIGHSCORES \n Please, type your username \n\n";
            	for(int i = 0; i < Math.min(data.size(), 5); i++) {
            		toPrint += data.get(indexOrder[i]) + "\n";
            	}
            	
            	userName = JOptionPane.showInputDialog(toPrint);
            	oldFile += userName + " " + finalScore + "\n";
            	FileWriter fileWrite = null;
            	try {
					fileWrite = new FileWriter(myScoresFile);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            	
            	try {
					fileWrite.write(oldFile);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            	try {
					fileWrite.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            }
        }
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        
        for(int i = 0; i < 11; i++) {
        	for (int j = 0; j < 18; j++) {
        		poison[i][j].draw(g);;
        	}
        }
        selector.draw(g);
        top.draw(g);
        myTimer.draw(g);
    }

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(COURT_WIDTH, COURT_HEIGHT);
    }
}