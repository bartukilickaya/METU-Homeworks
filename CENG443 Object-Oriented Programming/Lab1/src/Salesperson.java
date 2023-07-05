import java.awt.*;
import java.util.ArrayList;

public class Salesperson extends Entity{
    int stock;
    Product type;
    SalespersonStrategy strategy;
    int targetx,targety;
    public Salesperson(double x, double y, Product type, int stock,SalespersonStrategy strategy){
        super(x,y);
        this.type = type;
        this.stock = stock;
        this.strategy = strategy;
        this.targety = 0;
        this.targetx = 0;
    }
    public void sell() throws IllegalStateException{
        if(stock <= 0){
            throw new IllegalStateException("Out of Stock");
        }
        stock-=1;
        targety = 0;
        targetx = 0;
        // sold, now move to a new location
    }
    @Override
    public void draw(Graphics2D g2d) {
        String text;
        text = "" + type.abbreviate();
        text = text + "," + stock;
        switch (strategy) {
            case Random:
                text = text + ",R";
                break;
            default:
                text = text + ",M";
                break;
        }
        drawHelper(g2d,text,Color.BLUE);
    }

    @Override
    public void step() {

        if (stock == 0) {
            if (position.getIntX() < Common.getWindowWidth() - position.getIntX()) {
                position.setX(position.getX() - Common.getSalespersonMovementSpeed());
            } else {
                position.setX(position.getX() + Common.getSalespersonMovementSpeed());
            }
        }
        else if (targetx == 0 && targety == 0) {
            int cheapest = Integer.MAX_VALUE;
            double closest = Double.POSITIVE_INFINITY;
            double dist;
            switch (strategy) {
                case Random:
                    targetx = Common.randInt(0, Common.getWindowWidth() - 2 * Common.getEntityDiameter());
                    targety = Common.randInt(0, Common.getWindowHeight() - 2 * Common.getEntityDiameter());
                    break;
                case MiddleGround:
                    int x = 0;
                    int y = 0;
                    for (Customer c : Common.getCustomerList()) {
                        x += c.position.getIntX();
                        y += c.position.getIntY();
                    }
                    targetx = x / Common.getCustomerNo();
                    targety = y / Common.getCustomerNo();
                    break;
            }
        }
        else {
            double targetX = targetx;
            double targetY = targety;
            double dist = this.getPosition().distanceTo(targetX, targetY);
            if (dist <= 2 * Common.getEntityDiameter()) {
                // do nothing, wait here;
            }
            else {
                // move to target
                double x = targetX - this.getPosition().getX();
                double y = targetY - this.getPosition().getY();
                if (dist > Common.getCustomerMovementSpeed()) {
                    double vx = x / dist * Common.getCustomerMovementSpeed();
                    double vy = y / dist * Common.getCustomerMovementSpeed();
                    getPosition().setX(getPosition().getX() + vx);
                    getPosition().setY(getPosition().getY() + vy);
                } else {
                    getPosition().setX(x);
                    getPosition().setY(y);
                }

            }
        }
    }
}
