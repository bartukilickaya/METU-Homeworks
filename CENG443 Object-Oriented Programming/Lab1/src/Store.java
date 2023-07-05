import java.awt.*;

public class Store extends Entity {
    private Product type;
    private int price;
    private int maxStock;
    private int stock;

    public Store(double x, double y, Product type, int price, int maxStock) {
        super(x, y);
        this.type = type;
        this.price = price;
        this.maxStock = maxStock;
        this.stock = maxStock;

    }

    @Override
    public void draw(Graphics2D g2d) {
        String text = type.abbreviate() + ","+String.valueOf(stock)+","+String.valueOf(price);
        drawHelper(g2d,text,Color.ORANGE);
    }

    public Product getType() {
        return type;
    }

    public int getPrice() {
        return price;
    }

    @Override
    public void step() {

    }

    public void sell() throws IllegalStateException{
        if(stock == 0){
            throw new IllegalStateException("Out of Stock");
        }
        stock-=1;
    }
    public void add() throws IllegalStateException{
        stock = maxStock;

    }

    public void replenish() {
        stock = maxStock;
    }
}