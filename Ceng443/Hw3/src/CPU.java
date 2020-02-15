import java.util.Locale;

public class CPU extends Parts {
    private int coreCount;
    private double clockSpeed;


    public CPU(String type, String brand, String model, double price, int coreCount, double clockSpeed) {
        super(type, brand, model, price);
        this.coreCount = coreCount;
        this.clockSpeed = clockSpeed;
    }
    @Override
    public int getCoreCount() {
        return coreCount;
    }

    public void setCoreCount(int coreCount) {
        this.coreCount = coreCount;
    }

    @Override
    public double getClockSpeed() {
        return clockSpeed;
    }

    public void setClockSpeed(double clockSpeed) {
        this.clockSpeed = clockSpeed;
    }

    @Override
    public String toString(){
        return type + "," + brand + "," + model + "," + coreCount + "," + clockSpeed + "GHz," + String.format(Locale.ROOT,"%.2f",price) + " USD";
    }
}
