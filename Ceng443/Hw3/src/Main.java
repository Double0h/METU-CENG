public class Main {
    public static void main(String[] args) {
        PartsStore ps = new PartsStore();
        ps.FindPartsWithBrand("Monitor","Acer");
        ps.TotalPrice("CPU","Intel",null);
        ps.UpdateStock();
        ps.FindCheapestMemory(2);
        ps.FindFastestCPU();
    }
}
