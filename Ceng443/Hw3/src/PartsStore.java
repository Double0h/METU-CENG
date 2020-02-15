import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class PartsStore {
    private List<Parts> partsList = new ArrayList<>();

    public PartsStore() {
        String csvFile = "pcparts.csv";
        BufferedReader br = null;
        String line;
        String cvsSplitBy = ",";
        Parts part = null;
        try {
            br = new BufferedReader(new FileReader(csvFile));
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(cvsSplitBy);
                switch (parts[0]) {
                    case "CPU": {
                        part = new CPU(parts[0], parts[1], parts[2], Double.parseDouble(parts[5].split(" ")[0]),
                                Integer.parseInt(parts[3]), Double.parseDouble(parts[4].split("G")[0]));

                        break;
                    }
                    case "GPU": {
                        part = new GPU(parts[0], parts[1], parts[2], Double.parseDouble(parts[6].split(" ")[0]),
                                parts[3], Integer.parseInt(parts[4].split("G")[0]), Double.parseDouble(parts[5].split("M")[0]));
                        break;
                    }
                    case "Hard Drive": {
                        part = new HardDrive(parts[0], parts[1], parts[2], Double.parseDouble(parts[4].split(" ")[0]),
                                Integer.parseInt(parts[3].split("G")[0]));
                        break;
                    }
                    case "Keyboard": {
                        part = new Keyboard(parts[0], parts[1], parts[2], Double.parseDouble(parts[4].split(" ")[0]), parts[3]);
                        break;
                    }
                    case "Memory": {
                        part = new Memory(parts[0], parts[1], parts[2], Double.parseDouble(parts[6].split(" ")[0]),
                                parts[3], Integer.parseInt(parts[4].split("G")[0]), Double.parseDouble(parts[5].split("M")[0]));
                        break;
                    }
                    case "Monitor": {
                        part = new Monitor(parts[0], parts[1], parts[2], Double.parseDouble(parts[5].split(" ")[0]),
                                parts[3], Double.parseDouble(parts[4]));
                        break;
                    }
                    case "Motherboard": {
                        part = new Motherboard(parts[0], parts[1], parts[2], Double.parseDouble(parts[5].split(" ")[0]),
                                parts[3], Integer.parseInt(parts[4]));
                        break;
                    }
                    case "Mouse": {
                        part = new Mouse(parts[0], parts[1], parts[2], Double.parseDouble(parts[4].split(" ")[0]),
                                parts[3]);
                        break;
                    }
                    case "PSU": {
                        part = new PSU(parts[0], parts[1], parts[2], Double.parseDouble(parts[5].split(" ")[0]),
                                parts[3], Integer.parseInt(parts[4]));
                        break;
                    }
                    default:
                        System.out.println(parts[0] + " is not a valid PC part!");
                        break;
                }
                partsList.add(part);
            }

        } catch (IOException e) {
            e.printStackTrace();

        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

        public void FindPartsWithBrand(String type, String brand){
        Stream<Parts> resultStream = partsList.stream().filter(
                (parts)->parts.getBrand().equals(brand)
        );
        if(type != null){
            resultStream = resultStream.filter(
                    parts -> parts.getType().equals(type)
            );
        }
        resultStream.forEach(System.out::println);

    }

    public void TotalPrice(String type, String brand, String model){
        Stream<Parts> resultStream = partsList.stream();
        if(type != null){
            resultStream = resultStream.filter(
                    parts->parts.getType().equals(type)
            );
        }
        if(brand != null){
            resultStream = resultStream.filter(
                    parts -> parts.getBrand().equals(brand)
            );
        }
        if(model != null){
            resultStream = resultStream.filter(
                    parts -> parts.getModel().equals(model)
            );
        }
        double sum = resultStream.map(
                Parts::getPrice
        ).reduce(
                Double::sum
        ).orElse((double) 0);
        System.out.print(String.format(Locale.ROOT,"%.2f",sum)+ " USD\n");

    }

    public void UpdateStock(){
        System.out.println(partsList.stream().filter(
                parts->parts.getPrice() == 0
        ).count() +" items removed.");

        setPartsList(partsList.stream().filter(
                parts->parts.getPrice() != 0
        ).collect(Collectors.toList()));
    }

    public void FindCheapestMemory(int capacity){
        Stream<Parts> filteredStream = partsList.stream().filter(
                parts -> parts.getType().equals("Memory")
        ).filter(
                parts -> parts.getCapacity() >= capacity
        );

        double cheapest = filteredStream
                .map(
                Parts::getPrice
        ).reduce(
                Math::min
        ).orElse((double) 0);

        partsList.stream().filter(
                parts -> parts.getType().equals("Memory")
        ).filter(
                parts -> parts.getCapacity() >= capacity
        ).filter(
                parts -> parts.getPrice() == cheapest
        ).forEach(System.out::println);
    }

    public void FindFastestCPU(){
        double fastest = partsList.stream().filter(
                parts -> parts.getType().equals("CPU")
        ).map(
                parts -> parts.getCoreCount()*parts.getClockSpeed()
        ).reduce(
                Math::max
        ).orElse((double) 0);

        partsList.stream().filter(
                parts -> parts.getType().equals("CPU")
        ).filter(
                parts -> parts.getCoreCount()*parts.getClockSpeed() == fastest
        ).forEach(System.out::println);
    }

    public List<Parts> getPartsList() {
        return partsList;
    }

    public void setPartsList(List<Parts> partsList) {
        this.partsList = partsList;
    }
}