import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {

    private final ArrayList<ArrayList<Seat>> seatGrid;
    private ArrayList<User> users;

    public Main(int row, int column, int numberOfUsers){
        seatGrid = new ArrayList<>(row);
        for(int k = 0; k < row; k++)  {
            seatGrid.add(new ArrayList<>(column));
        }
        for(int i=0;i<row;i++){
            for(int j=0;j<column;j++){
                int b = 65 + i;
                String c = Character.toString((char)b);
                seatGrid.get(i).add(new Seat(c + j));
            }
        }
        users = new ArrayList<>(numberOfUsers);
    }

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);
        Scanner sc2 = new Scanner(System.in);
        int row = sc.nextInt();
        int column = sc.nextInt();
        int numberOfUser = sc.nextInt();
        Main simulate = new Main(row,column,numberOfUser);
        for(int i = 0;i<numberOfUser;i++) {
            String inp = sc2.nextLine();
            String[] words = inp.split(" ");
            String userName = words[0];
            ArrayList<Seat> wantedSeats = new ArrayList<>(words.length-1);
            for(int j=1;j<words.length;j++){
                wantedSeats.add(simulate.getSeatFromLocation(words[j]));
            }
            simulate.users.add(new User(userName,wantedSeats));
        }
        ExecutorService executor = Executors.newFixedThreadPool(numberOfUser);
        Logger.InitLogger();
        for(int k=0;k<numberOfUser;k++){
            executor.execute(simulate.users.get(k));
        }
        executor.shutdown();
        while(!executor.isTerminated());
        System.out.print(simulate);
    }

    @Override
    public String toString() {
        StringBuilder printGrid = new StringBuilder();
        for (ArrayList<Seat> seats : seatGrid) {
            for (int j = 0; j < seatGrid.get(0).size(); j++) {
                printGrid.append(seats.get(j).getStatus()).append(":").append(seats.get(j).getReservedUser());
                if (j < seatGrid.get(0).size() - 1) {
                    printGrid.append(" ");
                }
            }
            printGrid.append("\n");

        }
        return printGrid.toString();
    }

    public Seat getSeatFromLocation(String location){
        char rowLetter = location.charAt(0);
        int columnNumber = Integer.parseInt(Character.toString(location.charAt(1)));
        return seatGrid.get(rowLetter - 65).get(columnNumber);
    }

}
