import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Seat {
    private String reservedUser;
    private SeatStatus status;
    private String location;
    private final Lock lock = new ReentrantLock(true);


    public String getReservedUser() {
        return reservedUser;
    }

    public void setReservedUser(String reservedUser) {
        this.reservedUser = reservedUser;
    }

    Seat(String location){
        this.location = location;
        this.status = SeatStatus.E;
        this.reservedUser ="";
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public SeatStatus getStatus() {
        return status;
    }

    public void setStatus(SeatStatus status) {
        this.status = status;
    }

    public boolean reserve(String userName){
        lock.lock();
        try{
            if(status==SeatStatus.E){
                status = SeatStatus.T;
                reservedUser = userName;
                return true;
            }
        }
        finally {
            lock.unlock();
        }
        return false;
    }

    @Override
    public String toString() {
        return location;
    }
}
