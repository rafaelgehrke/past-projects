/**
 * CIS 120 Game HW
 * (c) University of Pennsylvania
 * @version 2.1, Apr 2017
 */

// imports necessary libraries for Java swing
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/**
 * Game Main class that specifies the frame and widgets of the GUI
 */
public class Game implements Runnable {
    public void run() {
        final JFrame frame = new JFrame("MineSweeper!");
        frame.setLocation(1000, 600);
        JOptionPane.showMessageDialog(frame, "Instructions: \n\n - This is a MineSweeper-type game,"
        		+ " if you already played it, go to Step 2, otherwise, start from Step 1."
        		+ "\n \n Step 1: You will have a grid, each square of the grid can hide a bomb. \n"
        		+ "Your mission is to open all the squares with no bombs. After opening a square, it will reveal"
        		+ " the number of bombs in the 8 squares surrounding it.\n "
        		+ "\n Step 2: \n The difference in this game is that you have 300 seconds to finish it. Your "
        		+ "final score will be the number of opened squares plus the remaining seconds you had. \n If you step on a bomb or your time "
        		+ "is over, you automatically get a score of only the number of opened places. There are 20 bombs randomly placed."
        		+ "\n\n CONTROLS: use the arrows to move the yellow frame over the square you want to open,"
        		+ " then, press \"s\". To mark the square, press \"d\", and to unmark it press \"f\"");

        // Status panel
        final JPanel status_panel = new JPanel();
        frame.add(status_panel, BorderLayout.SOUTH);
        final JLabel status = new JLabel("Running...");
        status_panel.add(status);

        // Main playing area
        final GameCourt court = new GameCourt(status);
        frame.add(court, BorderLayout.CENTER);

        // Reset button
        final JPanel control_panel = new JPanel();
        frame.add(control_panel, BorderLayout.NORTH);

        final JButton reset = new JButton("reset");
        reset.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                court.reset();
            }
        });
        control_panel.add(reset);

        // Put the frame on the screen
        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);

        // Start game
        court.reset();
    }

    /**
     * Main method run to start and run the game. Initializes the GUI elements specified in Game and
     * runs it. IMPORTANT: Do NOT delete! You MUST include this in your final submission.
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Game());
    }
}