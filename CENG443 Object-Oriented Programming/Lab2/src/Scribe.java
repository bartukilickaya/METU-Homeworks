import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class Scribe extends Thread{
    private final Semaphore pens;
    private final Semaphore inkBottles;
    private final Semaphore printLock;
    private final Semaphore books;
    private final int scribeNumber;

    public Scribe(int scribeNumber, Semaphore pens, Semaphore inkBottles,Semaphore books ,Semaphore printLock) {
        this.scribeNumber = scribeNumber;
        this.pens = pens;
        this.inkBottles = inkBottles;
        this.books = books;
        this.printLock = printLock;
    }

    @Override
    public void run() {
        while(true) {
            try {
                boolean hasPen = false;
                boolean hasInkBottle = false;
                boolean hasBook = false;

                // After 30 seconds of waiting to acquire the resource,
                // They release previously acquired resources in order to prevent starvation
                // 30 seconds can be changed arbitrarily.

                // Acquire pen first
                if( pens.tryAcquire(30,TimeUnit.SECONDS) ){
                    hasPen = true;
                    printLock.acquire();
                    System.out.println("Scribe " + scribeNumber + " takes a pen");
                    printLock.release();
                    Thread.sleep((int)(Math.random() * 10000));
                }

                // Try to acquire inkBottle
                if( inkBottles.tryAcquire(30,TimeUnit.SECONDS) ){
                    hasInkBottle = true;
                    printLock.acquire();
                    System.out.println("Scribe " + scribeNumber + " takes a bottle");
                    printLock.release();
                    Thread.sleep((int)(Math.random() * 10000));
                }

                // Try to acquire book
                if( books.tryAcquire(30,TimeUnit.SECONDS) ){
                    hasBook = true;
                    printLock.acquire();
                    System.out.println("Scribe " + scribeNumber + " takes a book");
                    printLock.release();
                    Thread.sleep((int)(Math.random() * 10000));
                }

                // If all three resources acquired, write a record
                if (hasPen && hasInkBottle && hasBook) {
                    printLock.acquire();
                    System.out.println("Scribe " + scribeNumber + " writes a record");
                    printLock.release();
                    Thread.sleep((int)(Math.random() * 10000));

                    printLock.acquire();
                    System.out.println("Scribe " + scribeNumber + " puts the book back");
                    printLock.release();
                    books.release();
                    Thread.sleep((int)(Math.random() * 10000));

                    printLock.acquire();
                    System.out.println("Scribe " + scribeNumber + " puts the bottle back");
                    printLock.release();
                    inkBottles.release();
                    Thread.sleep((int)(Math.random() * 10000));

                    printLock.acquire();
                    System.out.println("Scribe " + scribeNumber + " puts the pen back");
                    printLock.release();
                    pens.release();
                    Thread.sleep((int)(Math.random() * 10000));
                    continue;
                }
                if( hasBook ){ // book acquired but timeout, so put back (prevent starvation)
                    System.out.println("Scribe " + scribeNumber + " puts the book back");
                    printLock.release();
                    books.release();
                    Thread.sleep((int)(Math.random() * 10000));
                }
                if( hasInkBottle ){ // bottle acquired but timeout, so put back (prevent starvation)
                    printLock.acquire();
                    System.out.println("Scribe " + scribeNumber + " puts the bottle back");
                    printLock.release();
                    inkBottles.release();
                    Thread.sleep((int)(Math.random() * 10000));
                }
                if( hasPen ){ // pen acquired but timeout, so put back (prevent starvation)
                    printLock.acquire();
                    System.out.println("Scribe " + scribeNumber + " puts the pen back");
                    printLock.release();
                    pens.release();
                    Thread.sleep((int)(Math.random() * 10000));
                }
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}