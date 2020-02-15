import java.util.Locale;

public class HardDrive extends Parts {
    private int capacity;

    public HardDrive(String type, String brand, String model, double price, int capacity) {
        super(type, brand, model, price);
        this.capacity = capacity;
    }

    @Override
    public int getCapacity() { return capacity; }

    public void setCapacity(int capacity) { this.capacity = capacity; }

    @Override
    public String toString(){
        return type + "," + brand + "," + model + "," + capacity + "GB," + String.format(Locale.ROOT,"%.2f",price) +" USD";
    }
}
