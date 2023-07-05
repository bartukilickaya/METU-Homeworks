import java.awt.*;
import java.util.ArrayList;

public class Supplier extends Entity{
    private ArrayList<Store> visited;
    private Store target;
    private int store_size;
    private int visited_stores;
    public Supplier(double x, double y,int storeNo){
        super(x,y);
        visited = new ArrayList<Store>();
        target = null;
        store_size = storeNo;
        visited_stores = 0;
    }

    @Override
    public void draw(Graphics2D g2d) {
        drawHelper(g2d,"",Color.GREEN);
    }
    @Override
    public void step() {
        if (visited_stores == store_size) {
            if (position.getIntX() < Common.getWindowWidth() - position.getIntX()) {
                position.setX(position.getX() - Common.getCustomerMovementSpeed());
            } else {
                position.setX(position.getX() + Common.getCustomerMovementSpeed());
            }
        }
        else if (target == null) {
            int cheapest = Integer.MAX_VALUE;
            double closest = Double.POSITIVE_INFINITY;
            double dist;
            for (Store s : Common.getStoreList()) {
                if ( !visited.contains(s)) {
                    dist = this.getPosition().distanceTo(s.getPosition().getX(), s.getPosition().getY());
                    if (dist < closest) {
                        closest = dist;
                        target = s;
                    }
                }
            }
        }
        else {
            double targetX = target.getPosition().getX();
            double targetY = target.getPosition().getY();
            double dist = this.getPosition().distanceTo(targetX, targetY);
            if (dist <= 2 * Common.getEntityDiameter()) {
                try {
                    target.add();
                    visited.add(target);
                    visited_stores++;
                } catch (IllegalStateException e) {
                    visited.add(target);
                    while (visited.size() > 1) {
                        visited.remove(0);
                    }
                }
                target = null;
            }
            else {
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
