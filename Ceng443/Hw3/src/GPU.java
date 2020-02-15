import java.util.Locale;

public class GPU extends Parts {
    private String chipSet;
    private int capacity;
    private double clockSpeed;

    public GPU(String type, String brand, String model, double price, String chipSet, int capacity, double clockSpeed) {
        super(type, brand, model, price);
        this.chipSet = chipSet;
        this.capacity = capacity;
        this.clockSpeed = clockSpeed;
    }

    @Override
    public String getChipSet() { return chipSet; }

    public void setChipSet(String chipSet) { this.chipSet = chipSet; }

    @Override
    public int getCapacity() { return capacity; }

    public void setCapacity(int capacity) { this.capacity = capacity; }

    @Override
    public double getClockSpeed() { return clockSpeed; }

    public void setClockSpeed(double clockSpeed) { this.clockSpeed = clockSpeed; }

    @Override
    public String toString(){
        return type + "," + brand + "," + model + "," + chipSet + "," + capacity + "GB," + clockSpeed + "MHz," + String.format(Locale.ROOT,"%.2f",price) +" USD";
    }
}
