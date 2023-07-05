import java.awt.*;
import java.util.ArrayList;

class Customer extends Entity {
    private Strategy strategy;
    private ArrayList<Product> shoppingList;
    private Store target;
    private ArrayList<Store> visited;

    public enum Strategy {

        Cheapest,
        Closest,
        Travelling,
    }

    public Customer(double x, double y, Strategy strategy, ArrayList<Product> shoppingList) {
        super(x, y);
        this.strategy = strategy;
        this.shoppingList = shoppingList;
        target = null;
        visited = new ArrayList<Store>();
    }

    @Override
    public void draw(Graphics2D g2d) {
        String text;
        switch (strategy) {
            case Closest:
                text = "Cl";
                break;
            case Cheapest:
                text = "Ch";
                break;
            default:
                text = "Tr";
                break;
        }
        for (int i = 0; i < 3; i++) {
            try {
                text += "," + shoppingList.get(i).abbreviate();
            } catch (IndexOutOfBoundsException e) {
                break;
            }
        }
        drawHelper(g2d,text,Color.GRAY);
    }

    @Override
    public void step() {

        if (shoppingList.isEmpty()) {
            if (position.getIntX() < Common.getWindowWidth() - position.getIntX()) {
                position.setX(position.getX() - Common.getCustomerMovementSpeed());
            } else {
                position.setX(position.getX() + Common.getCustomerMovementSpeed());
            }
        } else if (target == null) {
            int cheapest = Integer.MAX_VALUE;
            double closest = Double.POSITIVE_INFINITY;
            double dist;
            switch (strategy) {
                case Cheapest:

                    for (Store s : Common.getStoreList()) {
                        if (s.getType() == shoppingList.get(0) && s.getPrice() < cheapest) {
                            cheapest = s.getPrice();
                            target = s;
                        }
                    }
                    break;
                case Closest:
                    for (Store s : Common.getStoreList()) {
                        if (s.getType() == shoppingList.get(0) && !visited.contains(s)) {
                            dist = this.getPosition().distanceTo(s.getPosition().getX(), s.getPosition().getY());
                            if (dist < closest) {
                                closest = dist;
                                target = s;
                            }
                        }
                    }
                    break;
                case Travelling:
                    for (Store s : Common.getStoreList()) {
                        if (!visited.contains(s)) {
                            dist = this.getPosition().distanceTo(s.getPosition().getX(), s.getPosition().getY());
                            if (dist < closest) {
                                closest = dist;
                                target = s;
                            }
                        }
                    }
                    if (closest == Double.POSITIVE_INFINITY) {
                        visited = new ArrayList<Store>();
                    }
                    break;
            }
        } else {
            double targetX = target.getPosition().getX();
            double targetY = target.getPosition().getY();
            double dist = this.getPosition().distanceTo(targetX, targetY);
            if (dist <= 2 * Common.getEntityDiameter()) {
                switch (strategy) {
                    case Cheapest:
                        try {
                            target.sell();
                            shoppingList.remove(0);
                            target = null;
                        } catch (IllegalStateException e) {
                        }
                        break;
                    case Closest:
                        try {
                            target.sell();
                            shoppingList.remove(0);
                        } catch (IllegalStateException e) {
                            visited.add(target);
                            while (visited.size() > 1) {
                                visited.remove(0);
                            }
                        }
                        target = null;
                        break;
                    case Travelling:
                        int index = shoppingList.indexOf(target.getType());
                        if (index != -1) {
                            try {
                                target.sell();
                                shoppingList.remove(index);
                            } catch (IllegalStateException e) {
                                visited.add(target);
                                target = null;
                            }
                        } else {
                            visited.add(target);
                            target = null;
                        }
                        break;
                }
            } else {
                for( Salesperson sa: Common.getSalespersonList() ){
                    double salesx = sa.getPosition().getX();
                    double salesy = sa.getPosition().getY();
                    double dist2 = this.getPosition().distanceTo(salesx, salesy);
                    if (dist2 <= 2 * Common.getEntityDiameter() && this.shoppingList.contains(sa.type) && sa.stock > 0){ // acrossed a salesperson
                        // if stock available and products matches, sell it to customer
                        this.shoppingList.remove(sa.type);
                        sa.sell();
                    }
                }
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