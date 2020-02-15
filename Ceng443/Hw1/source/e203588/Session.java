package e203588;
import java.util.ArrayList;

public class Session {

    private Track track;
    private ArrayList<Team> teamList;
    private int totalLaps;
    private Team winnerTeam;
    private ArrayList<Car> sortedCars;

    public Session() {
        // Fill this method
        this.track = new Track();
        this.teamList = new ArrayList<Team>();
        this.totalLaps = 0;
    }

    public Session(Track track, ArrayList<Team> teamList, int totalLaps) {
        // Fill this method
        this.track = track;
        this.teamList = teamList;
        this.totalLaps = totalLaps;
    }

    public Track getTrack() {
        return track;
    }

    public void setTrack(Track track) {
        this.track = track;
    }

    public ArrayList<Team> getTeamList() {
        return teamList;
    }

    public void setTeamList(ArrayList<Team> teamList) {
        this.teamList = teamList;
    }

    public int getTotalLaps() {
        return totalLaps;
    }

    public void setTotalLaps(int totalLaps) {
        this.totalLaps = totalLaps;
    }

    public void simulate() {
        // Fill this method
        ArrayList<Car> Cars = new ArrayList<Car>();
        if(!track.isValidTrack())
            System.out.println("Track is invalid.Simulation aborted!");
        else{
            System.out.println("Track is valid.Starting simulation on "+ track.getTrackName() +" for "+ totalLaps +" laps.");
            for (int k = 0; k<teamList.size();k++){
                Team t = teamList.get(k);
                for (int m = 0; m< t.getCarList().size();m++){
                    Car c = t.getCarList().get(m);
                    for (int i = 0; i < totalLaps; i++){
                        for (int j = 0; j < track.getTrackLength(); j++) {
                            c.tick(track.getNextFeature());
                        }
                    }
                    Cars.add(c);
                    //System.out.println(t.getName()+"->"+c.getDriverName()+"->"+c.getCarNo()+"->"+c.getTotalTime());
                }
            }
            sortedCars=getSortedCars(Cars);
            for (Team team_:teamList
            ) {
                for (Car car_:team_.getCarList()
                ) {
                    if (Cars.get(0).getCarNo() == car_.getCarNo()) {
                        winnerTeam = team_;
                        break;
                    }
                }
            }
            System.out.println(printWinnerTeam());
            System.out.println(printTimingTable());
        }

    }

    public String printWinnerTeam() {
        // Fill this method
        String teamInfo="Team "+winnerTeam.getName()+" wins.";
        for(int i = 0;i<winnerTeam.getTeamColors().length;i++){
            if(winnerTeam.getTeamColors().length == 1)
                teamInfo+=winnerTeam.getTeamColors()[i];
            else if(i == (winnerTeam.getTeamColors().length - 1))
                teamInfo+="and " +winnerTeam.getTeamColors()[i];
            else
                teamInfo+=winnerTeam.getTeamColors()[i]+", ";
        }
        teamInfo+=" flags are waving everywhere.";
        return teamInfo;
    }

    private String printTimingTable() {
        // Fill this method
        String timingTable="";
        for(int i = 0;i<sortedCars.size();i++){
            timingTable+=sortedCars.get(i).getDriverName()+"("+sortedCars.get(i).getCarNo()+")"+":  "+doubletoTime(sortedCars.get(i).getTotalTime());
            if(i<sortedCars.size()-1)
                timingTable+="\n";
        }
        return timingTable;
    }

    public ArrayList<Car> getSortedCars(ArrayList<Car> cars){
        cars.sort(Car.timeComparator);
        return cars;
    }

    public String doubletoTime(double t){
        int hour= (int) (t/3600);
        int min = (int) ((t-hour*3600)/60);
        int sec = (int) (t-(hour*3600+min*60));
        int ms = (int) Math.round((t-hour*3600-min*60-sec)*1000);
        String st = hour +":"+min+":"+sec+"."+ms;
        return st;
    }
}