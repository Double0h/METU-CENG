package e203588;
public class Straight extends TrackFeature {

    public Straight(int turnNo, TurnDirection direction, double distance, double roughness) {
        // Fill this method
        this.featureNo = turnNo;
        this.turnDirection = direction;
        this.distance = distance;
        this.roughness = roughness;
        this.multiplier = 1.0;
    }
}