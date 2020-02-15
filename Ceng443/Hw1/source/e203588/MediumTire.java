package e203588;
public class MediumTire extends Tire {

    public MediumTire() {
        this.speed = 310;
        this.degradation = 0;
    }
    public void tick(TrackFeature f){
        degradation+=f.getMultiplier()*f.getRoughness()*1.1;
        if(speed>=100)
            speed-=Math.min(75, degradation)*0.25;
    }

    public Tire getNextTire(){
        return new SoftTire();
    }
}