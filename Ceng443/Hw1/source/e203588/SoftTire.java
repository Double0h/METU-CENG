package e203588;
public class SoftTire extends Tire {

    public SoftTire() {
        this.speed = 350;
        this.degradation = 0;
    }

    public void tick(TrackFeature f){
        degradation+=f.getMultiplier()*f.getRoughness()*1.2;
        if(speed>=100)
            speed-=Math.min(75, degradation)*0.25;
    }

    public Tire getNextTire(){
        return new MediumTire();
    }
}