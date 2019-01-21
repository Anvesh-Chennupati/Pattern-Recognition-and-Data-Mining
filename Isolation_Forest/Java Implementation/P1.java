import iforest.IForest;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Scanner;


public class P1 {

    public static void main(String[] args) {

        //reading data from stdin dat file
        Scanner in = new Scanner(new BufferedReader(new InputStreamReader(System.in)));
        int k =0;

        try{
            k = in.nextInt();
        }
        catch(Exception e){
            System.out.println("Error  input data might be empty");
            System.exit(-1);
        }

        if(k <1){
            System.out.println("Invalid k value");
            System.exit(0);
        }

        // System.out.println("value of k " + String.valueOf(k));
        ArrayList<String> s = new ArrayList<String>();
        while (in.hasNextLine()) {
                String line = in.nextLine();
                //handling empty strings
                if(!(line.length() <1)){
                    s.add(line);
                }
        }
        in.close();

        // Converting input data from string to double 
        ArrayList<ArrayList<Double>> s1 = new ArrayList<ArrayList<Double>>();

        for(String ss: s){

            ArrayList<Double> temp = new ArrayList<Double>();
            String temp1[] = ss.split(",");

            for(int i =0;i < temp1.length;i++){
                try{
                    temp.add(Double.parseDouble(temp1[i]));
                }
                catch(Exception e){
                    System.out.println("Invalid input data");
                    System.exit(1);
                }
            }
            s1.add(temp);
        }

        // Checking for invalid dimensional data
        int dimencheck = s1.get(0).size();
        for(int i =0; i< s1.size(); i ++){
            if(dimencheck != s1.get(i).size()){
                System.out.println("Invalid dimensions of input data");
                System.exit(-1);
            }
        }

        // Converting array list objects to double
        double[][] samples = new double[s1.size()][s1.get(0).size()];


        for(int i=0; i<s1.size(); i++){
            for(int j=0; j<s1.get(i).size(); j++){
                samples[i][j] = s1.get(i).get(j);
            }
        }

        //checking if K value is greater than given input size
        if( k > samples.length){
            System.out.println("K value is greater than input size");
            System.exit(-1);
        }

        int nTrees = samples.length / 10;
        IForest iForest = new IForest();
        try {
            // getting the anomaly score of each input 
            double[] calculateAnomaly = iForest.build(samples, nTrees);

            //storing the anomaly score with index to fetch the top k anomaly
            double[][] anomalyList = new double[calculateAnomaly.length][2];

            for(int i =0; i <calculateAnomaly.length; i++){
                anomalyList[i][0] = calculateAnomaly[i];
                anomalyList[i][1] = i;
            }

            java.util.Arrays.sort(anomalyList, new java.util.Comparator<double[]>() {
                public int compare(double[] a, double[] b) {
                    return Double.compare(b[0], a[0]);
                }
            });

            for(int i =0; i< k;i++){
                for(int j =0; j < samples[(int)anomalyList[i][1]].length;j++){
                    System.out.print(samples[(int)anomalyList[i][1]][j]);
                    System.out.print(" ");
                }
                System.out.println();
            }
            

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}