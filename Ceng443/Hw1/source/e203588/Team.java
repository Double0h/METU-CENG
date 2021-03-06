package e203588;
import java.util.ArrayList;

public class Team {

    private String name;
    private String[] teamColors;
    private ArrayList<Car> carList;

    public Team() {
        // Fill this method
        this.name="";
        this.teamColors=new String[0];
        this.carList = new ArrayList<Car>();
    }

    public Team(String name, String[] colors, ArrayList<Car> carList) {
        // Fill this method
        this.name=name;
        this.teamColors=colors;
        this.carList = carList;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String[] getTeamColors() {
        return teamColors;
    }

    public void setTeamColors(String[] teamColors) {
        this.teamColors = teamColors;
    }

    public ArrayList<Car> getCarList() {
        return carList;
    }

    public void setCarList(ArrayList<Car> carList) {
        this.carList = carList;
    }

}