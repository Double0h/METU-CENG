import java.util.Locale;

public class PSU extends Parts {

    private String formFactor;
    private int wattage;

    public PSU(String type, String brand, String model, double price, String formFactor, int wattage) {
        super(type, brand, model, price);
        this.formFactor = formFactor;
        this.wattage = wattage;
    }

    @Override
    public String getFormFactor() { return formFactor; }

    public void setFormFactor(String formFactor) { this.formFactor = formFactor; }

    @Override
    public int getWattage() { return wattage; }

    public void setWattage(int wattage) { this.wattage = wattage; }

    @Override
    public String toString(){
        return type + "," + brand + "," + model + "," + formFactor + "," + wattage + "," + String.format(Locale.ROOT,"%.2f",price) + " USD";
    }
}
