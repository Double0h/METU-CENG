import java.util.Locale;

public class Monitor extends Parts {
    private String aspectRatio;
    private double size;

    public Monitor(String type, String brand, String model, double price, String aspectRatio, double size) {
        super(type, brand, model, price);
        this.aspectRatio = aspectRatio;
        this.size = size;
    }

    @Override
    public String getAspectRatio() { return aspectRatio; }

    public void setAspectRatio(String aspectRatio) { this.aspectRatio = aspectRatio; }

    @Override
    public double getSize() { return size; }

    public void setSize(double size) { this.size = size; }

    @Override
    public String toString(){
        return type + "," + brand + "," + model + "," + aspectRatio + "," + size + "," + String.format(Locale.ROOT,"%.2f",price) + " USD";
    }
}
