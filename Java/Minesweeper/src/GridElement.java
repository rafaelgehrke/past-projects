/**
 * CIS 120 Game HW
 * (c) University of Pennsylvania
 * @version 2.1, Apr 2017
 */

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

/**
 * A game object displayed using an image.
 * 
 * Note that the image is read from the file when the object is constructed, and that all objects
 * created by this constructor share the same image data (i.e. img is static). This is important for
 * efficiency: your program will go very slowly if you try to create a new BufferedImage every time
 * the draw method is invoked.
 */
public class GridElement extends GameObj {
    public static final String IMG_FILE = "src/mark3.png";
    public static final String IMG_FILE_BOMB = "src/poison.png";
    public static final String IMG_FILE_MARK = "src/block.png";
    public static final String IMG_FILE_ZERO = "src/lifeMush.png";
    public static final int SIZE = 40;
    public static final int INIT_POS_X = 0;
    public static final int INIT_POS_Y = 300;
    public static final int INIT_VEL_X = 0;
    public static final int INIT_VEL_Y = 0;
    private boolean hasBomb = false;
    private int numNear = 0;
    private boolean isOpen = false;
    private boolean isMarked = false;
    
    private static BufferedImage img;
    private static BufferedImage img_bomb;
    private static BufferedImage img_mark;
    private static BufferedImage img_zero;

    public GridElement(int initX, int initY, int courtWidth, int courtHeight) {
        super(INIT_VEL_X, INIT_VEL_Y, initX, initY, SIZE, SIZE, courtWidth, courtHeight);

        try {
            if (img == null) {
                img = ImageIO.read(new File(IMG_FILE));
            }
        } catch (IOException e) {
            System.out.println("Internal Error:" + e.getMessage());
        }
        
        try {
            if (img_bomb == null) {
                img_bomb = ImageIO.read(new File(IMG_FILE_BOMB));
            }
        } catch (IOException e) {
            System.out.println("Internal Error:" + e.getMessage());
        }
        
        try {
            if (img_mark == null) {
                img_mark = ImageIO.read(new File(IMG_FILE_MARK));
            }
        } catch (IOException e) {
            System.out.println("Internal Error:" + e.getMessage());
        }
        
        try {
            if (img_zero == null) {
                img_zero = ImageIO.read(new File(IMG_FILE_ZERO));
            }
        } catch (IOException e) {
            System.out.println("Internal Error:" + e.getMessage());
        }
    }
    public boolean hasBomb() {
    	return hasBomb;
    }
    public void putBomb() {
    	this.hasBomb = true;
    }
    
    public int getNumNear() {
    	return numNear;
    }
    public void addNumNear() {
    	numNear++;
    }
    public boolean getIsOpen() {
    	return isOpen;
    }
    public void setIsOpen (boolean var) {
    	isOpen = var;
    }
    public boolean getIsMarked() {
    	return isOpen;
    }
    public void setIsMarked(boolean var) {
    	isMarked = var;
    }

    @Override
    public void draw(Graphics g) {
    	if(!isOpen) {
    		if(!isMarked) {
    			g.drawImage(img, this.getPx(), this.getPy(), this.getWidth(), this.getHeight(), null);
    		}
    		else {
    			g.drawImage(img_mark, this.getPx(), this.getPy(), this.getWidth(), this.getHeight(), null);
    		}
    	}
    	else {
    		if(!hasBomb) {
    			if(numNear == 0) {
    				g.drawImage(img_zero, this.getPx(), this.getPy(), this.getWidth(), this.getHeight(), null);
    			}
    			else {
			    	Graphics2D g2d = (Graphics2D) g;
			    	//g2d.setStroke(new BasicStroke(10));
			    	g2d.setColor(Color.BLACK);
			    	g2d.setFont(new Font("Arial", Font.BOLD, 35));
			        g2d.drawString(Integer.toString(numNear), this.getPx()+10, this.getPy() + 33);
			    	g2d.setStroke(new BasicStroke(4));
			        g2d.drawRect(this.getPx() + 1, this.getPy() + 1, this.getWidth()-2, this.getHeight()-2);
    			}
    		}
    		else {
    			g.drawImage(img_bomb, this.getPx(), this.getPy(), this.getWidth(), this.getHeight(), null);
    		}
    	}
    }
}
