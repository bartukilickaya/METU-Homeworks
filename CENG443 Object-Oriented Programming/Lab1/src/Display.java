import javax.swing.*;
import java.awt.*;

public class Display extends JPanel {
    public Display() { this.setBackground(new Color(220, 220, 220)); }

    @Override
    public Dimension getPreferredSize() { return super.getPreferredSize(); }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        for (Store store: Common.getStoreList()) store.draw((Graphics2D) g);
        for (Customer customer: Common.getCustomerList()) customer.draw((Graphics2D) g);
        for(Supplier supplier:Common.getSupplierList()) supplier.draw((Graphics2D) g);
        for(Salesperson salesperson:Common.getSalespersonList()) salesperson.draw((Graphics2D) g);
    }
}