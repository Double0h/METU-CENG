import java.util.ArrayList;
import java.util.Random;

public class User implements Runnable {
    private final String name;
    private final ArrayList<Seat> wantedSeats;
    private boolean shouldRetry;
    private boolean allTaken = true;

    User(String name, ArrayList<Seat> wantedSeats){

        this.name = name;
        this.wantedSeats = wantedSeats;
        this.shouldRetry = true;
    }


    public String getName() {
        return name;
    }

    public ArrayList<Seat> getWantedSeats() {
        return wantedSeats;
    }

    @Override
    public void run() {
        while(shouldRetry){
            if(isSeatsEmpty(wantedSeats)){
                if(!isDatabaseFailed()){
                    for (Seat seat:wantedSeats) {
                        allTaken=allTaken && seat.reserve(name);
                        if(!allTaken){
                            for(Seat seat1:wantedSeats){
                                if(name.equals(seat1.getReservedUser())){
                                    seat1.setReservedUser("");
                                    seat1.setStatus(SeatStatus.E);
                                }
                            }
                        }
                    }
                    if(allTaken){
                        try {
                            Thread.sleep(50);

                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        Logger.LogSuccessfulReservation(this.name, String.valueOf(wantedSeats), System.nanoTime(),"Success!!");
                    }
                    else{
                        Logger.LogFailedReservation(this.name,String.valueOf(wantedSeats),System.nanoTime(),"Reservation Failed!!");
                    }
                    shouldRetry = false;

                }
                else{
                    Logger.LogDatabaseFailiure(this.name, String.valueOf(wantedSeats), System.nanoTime(),"Database Failed!!");
                    shouldRetry=true;
                    try {

                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
            else{
                Logger.LogFailedReservation(this.name,String.valueOf(wantedSeats),System.nanoTime(),"Reservation Failed!!");
                shouldRetry=false;
            }
        }
    }

    private static boolean isSeatsEmpty(ArrayList<Seat> seats){
        for (Seat seat:seats
             ) {
            if(seat.getStatus()!=SeatStatus.E)
                return false;
        }
        return true;
    }

    private static boolean isDatabaseFailed(){
        Random rand = new Random();
        int n = rand.nextInt(10);
        return n == 7;
    }




}
