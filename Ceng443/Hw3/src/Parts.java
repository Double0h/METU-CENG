public class Parts {

    protected String type;
    protected String brand;
    protected String model;
    protected double price;

    public Parts(String type, String brand, String model, double price){
        this.type = type;
        this.brand = brand;
        this.model = model;
        this.price = price;
    }

    public int getCoreCount() { return 0; }
    public double getClockSpeed() {
        return 0;
    }
    public String getChipSet() { return null; }
    public String getConnectionType() { return null; }
    public String getSocket() { return null; }
    public int getCapacity(){ return 0; }
    public String getAspectRatio() { return null; }
    public double getSize() { return 0; }
    public int getRamSlots() { return 0; }
    public String getFormFactor() { return null; }
    public int getWattage() { return 0; }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getBrand() {
        return brand;
    }

    public void setBrand(String brand) {
        this.brand = brand;
    }

    public String getModel() {
        return model;
    }

    public void setModel(String model) { this.model = model; }

    public double getPrice() { return price; }

    public void setPrice(double price) { this.price = price; }
}
