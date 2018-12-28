/**
 * CIS 120 Game HW
 * (c) University of Pennsylvania
 * @version 2.1, Apr 2017
 */

import java.awt.*;

/**
 * A basic game object starting in the upper left corner of the game court. It is displayed as a
 * square of a specified color.
 */
public class Selector extends GameObj {
    public static final int SIZE = 40;
    public static final int INIT_POS_X = 0;
    public static final int INIT_POS_Y = 300;
    public static final int INIT_VEL_X = 0;
    public static final int INIT_VEL_Y = 0;

    private Color color;

    /**
    * Note that, because we don't need to do anything special when constructing a Square, we simply
    * use the superclass constructor called with the correct parameters.
    */
    public Selector(int courtWidth, int courtHeight, Color color) {
        super(INIT_VEL_X, INIT_VEL_Y, INIT_POS_X, INIT_POS_Y, SIZE, SIZE, courtWidth, courtHeight);

        this.color = color;
    }

    @Override
    public void draw(Graphics g) {
    	Graphics2D g2d = (Graphics2D) g;
    	g2d.setStroke(new BasicStroke(10));
        g2d.setColor(this.color);
        g2d.drawRect(this.getPx(), this.getPy(), this.getWidth(), this.getHeight());
    }
}