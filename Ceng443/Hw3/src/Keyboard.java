import java.util.Locale;

public class Keyboard extends Parts {
    private String connectionType;

    public Keyboard(String type, String brand, String model, double price, String connectionType) {
        super(type, brand, model, price);
        this.connectionType = connectionType;
    }

    @Override
    public String getConnectionType() { return connectionType; }

    public void setConnectionType(String connectionType) { this.connectionType = connectionType; }

    @Override
    public String toString(){
        return type + "," + brand + "," + model + "," + connectionType + "," + String.format(Locale.ROOT,"%.2f",price) + " USD";
    }
}
