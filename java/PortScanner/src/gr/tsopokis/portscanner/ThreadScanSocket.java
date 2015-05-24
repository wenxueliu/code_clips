package gr.tsopokis.portscanner;


import java.io.IOException;
import java.net.*;
import javax.swing.JTextArea;

/**
 * This Class is a library that defines the code to be run by each tread.
 * 
 * @author Christos Tsopokis
 * @version 1.0
 */

public class ThreadScanSocket implements Runnable {

    /**
    * A constant holding the TIME_OUT time (in milliseconds) for the socket to be connected.
    */
    static final int TIME_OUT = 5000; // milliseconds

    /**
    * A field holding a textual represantation of the IP passed by the scanner.
    */
    private String ip;
    /**
    * A field holding an integral represantation of the Port passed by the scanner.
    */
    private int port;
    /**
    * A field holding mode of verbose checkbox.
    */
    private boolean verbose;
    /**
    * A field holding the area to be printed the Output.
    */
    private JTextArea jTextArea;
    
    /**
      *	Code run by the threads. Each thread creates an unconnected Socket and then connects it with the combination
      *	of IP and port stored ing the fields with a particular TIME_OUT. If the connection is successful then the port
      *	in this IP is open. If the connection is not successful an exception is catched and the corresponding message
      * is printed.
      * 
    */
    public void run(){
        try
        {
            Socket socket = new Socket();
            socket.connect(new InetSocketAddress(InetAddress.getByName(ip),port),TIME_OUT);
            jTextArea.append(ip + ":" + port + " open" + "\n");
            System.out.println(ip + ":" + port + " open");
        }
        catch (NoRouteToHostException ex)
        {
            if (verbose){
                jTextArea.append("host:" + ip + " down" + "\n");
                System.out.println("host:" + ip + " down");
            }
        }
        catch (SocketTimeoutException ex)
        {
            jTextArea.append(ip + ":" + port + " closed (live host)" + "\n"); // port inactive
            System.out.println(ip + ":" + port + " closed (live host)");
        }
        catch (ConnectException ex)
        {
            jTextArea.append(ip + ":" + port + " filtered (live host)" + "\n"); // port protected maybe by a firewall
            System.out.println(ip + ":" + port + " filtered (live host)");
        }
        catch (IOException ex)
        {
            System.err.println(ip + ":" + port + " socket error: " + ex.getMessage());
            System.exit(-1);
        }
    }

    /**
      * Constructs a newly allocated ThreadScanSocket object. Firstly the class encodes the first parameter passed 
      * to the Constructor and then initiates the 4 fields of the Class.
      * 
      * @param firstIP		textual representation of the first IP given by the user
      * @param lastIP		textual representation of the last IP given by the user
      * @param portList		textual representation of the list of ports given by the user
      * @param noOfThreads	textual representation of the threads that will run the portscanning
      * 
    */
    public ThreadScanSocket (long ip, int port, boolean verbose, JTextArea jTextArea){

        this.ip = PortScanner.encodeIP(ip);
        this.port = port;
        this.verbose = verbose;
        this.jTextArea = jTextArea;
    }
}
