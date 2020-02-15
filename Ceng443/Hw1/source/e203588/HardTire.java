package e203588;
public class HardTire extends Tire {

    public HardTire() {
        this.speed = 275;
        this.degradation = 0;
    }

    public void tick(TrackFeature f){
        degradation+=f.getMultiplier()*f.getRoughness()*1.0;
        if(speed>=100)
            speed-=Math.min(75, degradation)*0.25;
    }

    public Tire getNextTire(){
        return new SoftTire();
    }
}