import java.util.Deque;
import java.util.Iterator;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedDeque;

class DGIM {

    public class Bucket {
        int size;
        int pos;

        Bucket(int size, int position){
            this.size = size;
            this.pos = position;
        }
        void setPostion(int position){
            this.pos = position;
        }

        int getPostion(){
            return pos;
        }

        void setSize(int size){
            this.size = size;
        }

        int getSize(){ return size; }
    }

    private Queue<Boolean> data;
    private Deque<Bucket> bvalues;
    private int cp;

    DGIM(Queue <Boolean> ConvertBitStreamData){
        data = ConvertBitStreamData;
        bvalues = new ConcurrentLinkedDeque<>();
        cp=1;
    }

    private void addBucket(int position){
        bvalues.addFirst(new Bucket(1,position));
        Iterator<Bucket> iter = bvalues.iterator();
        if(MergeRequired(iter)){
            Iterator<Bucket> temp = bvalues.iterator();
            merge(temp);
        }
    }


    void updateBuckets(){
        while(!data.isEmpty()){
            if(data.poll()) addBucket(cp++);
            else cp++;
        }
    }

    int getBucketCount(int k){
        int answer = 0;
        int val = 0;
        if (bvalues.isEmpty()) return 0;
        int distance = cp - bvalues.getFirst().getPostion();
        if (distance > k) return 0;
        for (Bucket cur : bvalues)
            if (cur.getPostion() >= (cp - k)) {
                val = cur.getSize();
                answer += val;
            } else {
                break;
            }
        answer -= val / 2;
        return answer;
    }

    private boolean MergeRequired(Iterator<Bucket> iter) {
        Bucket first, next, nextTonext, last;
        final boolean b1 = false;
        if(iter.hasNext()){
            first = iter.next();
            if(iter.hasNext()){
                next = iter.next();
                if(first.getSize() != next.getSize()) return b1;
                else{
                    if(iter.hasNext()){
                        nextTonext = iter.next();
                        if(next.getSize() != nextTonext.getSize()) return b1;
                        else{
                            if(iter.hasNext()){
                                last = iter.next();
                                return nextTonext.getSize() == last.getSize();
                            } else return b1; }
                    } else return b1; }
            } else return b1; }
        else return b1;
    }

    private Iterator<Bucket> getIterator(int previousBucketSize) {
        Iterator<Bucket> iter = bvalues.iterator();
        int count = 0;
        while(iter.hasNext()){
            Bucket temp = iter.next();
            if(temp.getSize() == previousBucketSize) count++;
            if(count == 2) return iter;
        }
        return null;
    }

    private void merge(Iterator<Bucket> temp) {
        temp.next();temp.next();
        Bucket third = temp.next();
        Bucket fourth = temp.next();

        int previousBucketSize = third.getSize();
        third.setSize(third.getSize() + fourth.getSize());
        third.setPostion(third.getPostion());
        temp.remove();

        Iterator<Bucket> temp2 = getIterator(previousBucketSize);
        assert temp2 != null;
        if(MergeRequired( temp2)){
            Iterator<Bucket> temp3 = getIterator(previousBucketSize);
            assert temp3 != null;
            merge( temp3);
        }
    }

}
