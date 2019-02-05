import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedDeque;
import java.util.stream.IntStream;

public class convertBitStream {
    private Queue<Boolean>[] datastreams;

    @SuppressWarnings("unchecked")
    convertBitStream(){
        datastreams = new Queue[16];
        IntStream.range( 0, 16 ).forEach( i -> datastreams[i] = new ConcurrentLinkedDeque <>() );
    }

    synchronized void setData(int number) {
        if (!(number > 0) && !(number < 65535)) {
            System.out.println("Number is greater than 16bit space");
            System.exit(0);
        }
        int bitset = 1;
        int i = 0;
        while (i < 16) {
            boolean status;
            synchronized (datastreams[i]) {
                status = (number & bitset) == 0 ? datastreams[i].offer( Boolean.FALSE ) : datastreams[i].offer( Boolean.TRUE );
            }
            if (!status) System.out.println( "Unable to add data in streams" );
            bitset<<=1;
            i++;
        }
    }

    synchronized Queue<Boolean> getData(int bit) {
        return datastreams[bit];
    }

    public synchronized void displayData(){

        System.out.println("Printing data streams");

        IntStream.range( 0, 16 ).forEach( i -> {
            System.out.println();
            System.out.println( datastreams[i] );
        } );
    }
}