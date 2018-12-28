/**
 * CIS 120 Game HW
 * (c) University of Pennsylvania
 * @version 2.1, Apr 2017
 */

import java.awt.*;

/**
 * A game object displayed using an image.
 * 
 * Note that the image is read from the file when the object is constructed, and that all objects
 * created by this constructor share the same image data (i.e. img is static). This is important for
 * efficiency: your program will go very slowly if you try to create a new BufferedImage every time
 * the draw method is invoked.
 */
public class ShowTime extends GameObj {
    public static final int SIZE = 30;
    public static final int INIT_POS_X = 40;
    public static final int INIT_POS_Y = 40;
    public static final int INIT_VEL_X = 0;
    public static final int INIT_VEL_Y = 0;
    public static String toPrint= "";

    public ShowTime(String toPrint, int courtWidth, int courtHeight) {
        super(INIT_VEL_X, INIT_VEL_Y, INIT_POS_X, INIT_POS_Y, SIZE, SIZE, courtWidth, courtHeight);
        ShowTime.toPrint = toPrint;
    }

    @Override
    public void draw(Graphics g) {
    	Graphics2D g2d = (Graphics2D) g;
    	g2d.setStroke(new BasicStroke(10));
    	g2d.setColor(Color.WHITE);
    	g2d.setFont(new Font("Arial", Font.BOLD, 100));
        g2d.drawString(toPrint, 500, 210);
    }
}
