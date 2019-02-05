import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Scanner;

public class P2 {

    private static int rval = 0;
    private static String hostname = null;
    private static int port = 0;
    private static final Object checkpos = new Object();
    private static final Object dgimObject = new Object();
    private static long queryval = 1;
    private static long pckcount = 1;



    public static void main(String[] args){
        DGIM[] dgims = new DGIM[16];
        convertBitStream datastreams = new convertBitStream();
        Scanner in = new Scanner(new BufferedReader(new InputStreamReader(System.in)));
        String inputAddress = "";
        String percentError = "";

        try{
            percentError = in.nextLine();
//            rval = in.nextInt();
            percentError = percentError.trim();
            percentError = percentError.replace( "%","" );
            rval = (Math.round(100/(Integer.parseInt( percentError ))));
//            in.nextLine();
            inputAddress = in.nextLine();
        }

        catch (Exception e){
            System.out.println("Unable to read r value and localhost:port pair");
            System.exit(-1);
        }

        if(rval < 0){
            System.out.println("Invalid value of r. Please try again...");
            System.exit(-1);
        }

        if (!inputAddress.contains(":")) {
            System.out.println("Invalid host IP and port. Format is host ip:port");
            System.exit(0);
        }

        else {
            String[] ipAddress = inputAddress.trim().split(":");
            String ipport = ipAddress[1];
            for (int i = 0; i < ipport.length(); i++) {
                if (!Character.isDigit(ipport.charAt(i))) {
                    System.out.println("[Invalid port Number");
                    System.exit(0);
                }
            }

            hostname = ipAddress[0];
            port = Integer.valueOf(ipport);
        }

        // Displaying R , IP and Port number
        System.out.println("R value is :" + rval);
        System.out.println("Ip Address is :"+ hostname);
        System.out.println("Port number is :"+ port);

        Thread queryReader = new Thread(() -> {
            try{
                while(in.hasNextLine()){
                    String query = in.nextLine();
                    query = query.toLowerCase();

                    if(query.trim().equals("end")){
                        System.out.println("End of input Commands. Closing the connection");
                        System.exit(-1);
                    }
//                    System.out.println(query);
                    else {

                        if(!(query.trim().contains("what is the sum for last ") && query.trim().contains("integers"))){
                            System.out.println("Invalid query format");
                            System.out.println(query.trim());
                            System.out.println("Query Format: what is the sum for last <k> integers ");
                            System.exit(-1);
                        }
                        else{
                            String tempval1 = query.substring(24,(query.trim().length()-9));
                            queryval = Long.valueOf(tempval1.trim());
                            System.out.println(query);

                            if(pckcount <= queryval){
                                Thread.sleep(10);
                            }

                            synchronized (checkpos){
                                checkpos.wait();
                            }
                        }
                    }


                }
            }
            catch (Exception e){
                e.printStackTrace();
            }
        });

        queryReader.start();


        Thread packetInterceptor= new Thread(() -> {
            try{
                Socket socket;
                BufferedReader br;

                InetAddress host = InetAddress.getByName(hostname);
                socket = new Socket(host.getHostName(), port);
                String s;
                br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                while ((s=br.readLine())!=null)
                {
                    datastreams.setData(Integer.valueOf(s));
                    System.out.println(s);

                    if ((pckcount >= queryval)) {
                        synchronized (dgimObject) {
                            dgimObject.notify();
                        }
                    }

                    if ((pckcount >= queryval)) {
                        synchronized (checkpos) {
                            checkpos.wait(1);
                        }
                    }
                    pckcount++;
                }
            }

            catch (Exception e){
                e.printStackTrace();
            }
        });
        packetInterceptor.start();

        Thread calcDGIM;
        calcDGIM = new Thread(() -> {
            try {
                for (int i = 0; i < 16; i++) {
                    dgims[i] = new DGIM(datastreams.getData(i));
                }
                synchronized (dgimObject){
                    while(true){

                        dgimObject.wait();

                        for(int i=0;i<16;i++){
                            dgims[i].updateBuckets();
                        }

                        double sum = 0;
                        for (int i = 0; i <16; i++) {
                            sum += dgims[i].getBucketCount(Math.toIntExact(queryval))* Math.pow(2, i);
                        }

                        if(queryval > 0 && pckcount >= queryval){
                            System.out.println("The sum of last " + queryval + " integers is " + sum);
                            queryval = 0;
                            //ansComp = true;
                            synchronized (checkpos){
                                checkpos.notifyAll();
                            }
                        }

                    }
                }


            }
            catch ( Exception e){
                e.printStackTrace();
            }
        });
        calcDGIM.start();

    }
}

