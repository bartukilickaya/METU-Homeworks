import java.util.concurrent.Semaphore;

public class Lab2 {
    public static void main(String[] args) {
        int scribesCount = Integer.parseInt(args[0]);
        int pensCount = Integer.parseInt(args[1]);
        int bottlesCount = Integer.parseInt(args[2]);
        int bookCount = Integer.parseInt(args[3]);

        Semaphore pens = new Semaphore(pensCount);
        Semaphore inkBottles = new Semaphore(bottlesCount);
        Semaphore books = new Semaphore(bookCount);
        Semaphore printLock = new Semaphore(1);

        for(int i = 1; i < scribesCount; i++){
            new Scribe(i,pens,inkBottles,books,printLock).start();
        }
    }
}