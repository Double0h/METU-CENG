package e203588;
public class HighSpeedTurn extends TrackFeature {

    public HighSpeedTurn(int turnNo, TurnDirection direction, double distance, double roughness) {
        // Fill this method
        this.featureNo = turnNo;
        this.turnDirection = direction;
        this.distance = distance;
        this.roughness = roughness;
        this.multiplier = 1.55;
    }
}