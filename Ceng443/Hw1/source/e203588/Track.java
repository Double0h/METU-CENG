package e203588;
import java.util.ArrayList;

public class Track {

    private String trackName;
    private ArrayList<TrackFeature> featureList;
    private boolean isClockwise;
    private int currentFeatureIndex;

    public Track() {
        // Fill this method
        this.trackName = "";
        this.featureList = new ArrayList<TrackFeature>();
        this.isClockwise = false;
        this.currentFeatureIndex = -1;

    }

    public Track(String trackName, ArrayList<TrackFeature> featureList, boolean isClockwise) {
        // Fill this method
        this.trackName = trackName;
        this.featureList = featureList;
        this.isClockwise = isClockwise;
        this.currentFeatureIndex = -1;
    }

    public String getTrackName() {
        return trackName;
    }

    public void setTrackName(String trackName) {
        this.trackName = trackName;
    }

    public ArrayList<TrackFeature> getFeatureList() {
        return featureList;
    }

    public void setFeatureList(ArrayList<TrackFeature> featureList) {
        this.featureList = featureList;
    }

    public boolean isClockwise() {
        return isClockwise;
    }

    public void setClockwise(boolean clockwise) {
        isClockwise = clockwise;
    }

    public int getTrackLength() {
        // Fill this method
        return featureList.size();
    }

    public TrackFeature getNextFeature() {
        // Fill this method
        if((currentFeatureIndex == -1) || (currentFeatureIndex == (featureList.size() - 1))) {
            currentFeatureIndex = 0;
            return featureList.get(currentFeatureIndex);
        }
        currentFeatureIndex++;
        return featureList.get(currentFeatureIndex);
    }

    public void addFeature(TrackFeature feature) {
        // Fill this method
        featureList.add(feature);
    }

    public boolean isValidTrack() {
        // Fill this method
        int numberOfRights = 0;
        int numberOfLefts = 0;
        for(int i=0;i<featureList.size();i++){
            if(featureList.get(i).getTurnDirection()==TurnDirection.RIGHT)
                numberOfRights++;
            if(featureList.get(i).getTurnDirection()==TurnDirection.LEFT)
                numberOfLefts++;

        }
        if(featureList.get(0).getTurnDirection()==TurnDirection.STRAIGHT && featureList.get(featureList.size()-1).getTurnDirection()==TurnDirection.STRAIGHT){
            int checker = numberOfLefts-numberOfRights;
            if(this.isClockwise && checker==-4)
                return true;
            if((!this.isClockwise) && checker==4)
                return true;

        }
        return false;
    }
}