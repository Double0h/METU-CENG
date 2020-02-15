import java.util.Locale;

public class Motherboard extends Parts {
    private String socket;
    private int ramSlots;

    public Motherboard(String type, String brand, String model, double price, String socket, int ramSlots) {
        super(type, brand, model, price);
        this.socket = socket;
        this.ramSlots = ramSlots;
    }

    @Override
    public String getSocket() { return socket; }

    public void setSocket(String socket) { this.socket = socket; }

    @Override
    public int getRamSlots() { return ramSlots; }

    public void setRamSlots(int ramSlots) { this.ramSlots = ramSlots; }

    @Override
    public String toString(){
        return type + "," + brand + "," + model + "," + socket + "," + ramSlots + "," + String.format(Locale.ROOT,"%.2f",price) + " USD";
    }
}
