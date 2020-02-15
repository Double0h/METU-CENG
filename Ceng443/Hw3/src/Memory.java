import java.util.Locale;

public class Memory extends Parts {

    private String socket;
    private int capacity;
    private double clockSpeed;

    public Memory(String type, String brand, String model, double price, String socket, int capacity, double clockSpeed) {
        super(type, brand, model, price);
        this.socket = socket;
        this.capacity = capacity;
        this.clockSpeed = clockSpeed;
    }

    @Override
    public String getSocket() { return socket; }

    public void setSocket(String socket) { this.socket = socket; }

    @Override
    public int getCapacity() { return capacity; }

    public void setCapacity(int capacity) { this.capacity = capacity; }

    @Override
    public double getClockSpeed() { return clockSpeed; }

    public void setClockSpeed(double clockSpeed) { this.clockSpeed = clockSpeed; }

    @Override
    public String toString(){
        return type + "," + brand + "," + model + "," + socket + "," + capacity + "GB," + clockSpeed + "MHz," + String.format(Locale.ROOT,"%.2f",price) + " USD";
    }
}
