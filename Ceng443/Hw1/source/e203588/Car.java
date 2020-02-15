package e203588;
import java.util.Comparator;

public class Car {

    private int carNo;
    private String driverName;
    private double totalTime;
    private Tire tire;

    public Car() {
        // Fill this method
        carNo = -1;
        driverName = "";
        totalTime = 0;
        tire = new SoftTire();
    }

    public Car(String driverName, int carNo, Tire tire) {
        // Fill this method
        this.driverName = driverName;
        this.carNo=carNo;
        this.tire=tire;
        this.totalTime = 0;
    }

    public Tire getTire() {
        return tire;
    }

    public void setTire(Tire tire) {
        this.tire = tire;
    }

    public String getDriverName() {
        return driverName;
    }

    public void setDriverName(String driverName) {
        this.driverName = driverName;
    }

    public int getCarNo() {
        return carNo;
    }

    public void setCarNo(int carNo) {
        this.carNo = carNo;
    }

    public double getTotalTime() {
        return totalTime;
    }

    public void tick(TrackFeature feature) {
        // Fill this method
        double rand=Math.random();
        this.totalTime += feature.getDistance()/this.tire.getSpeed() + rand;
        this.tire.tick(feature);
        if(this.tire.getDegradation()>70){
            this.tire = this.tire.getNextTire();
            this.totalTime+=25;
        }
    }

    public static Comparator<Car> timeComparator = new Comparator<Car>() {
        @Override
        public int compare(Car car_1, Car car_2) {
            return Double.compare(car_1.getTotalTime(), car_2.getTotalTime());
        }
    };

}