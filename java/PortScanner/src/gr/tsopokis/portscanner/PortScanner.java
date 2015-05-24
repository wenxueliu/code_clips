package gr.tsopokis.portscanner;

import java.util.ArrayList;
import javax.swing.JTextArea;

/**
 * An impelementation of a TCP portscanner. PortScanner Class is a library defining constant variables, fields and methods 
 * used by the program. A range of IP addresses, a list of ports and the number of threads to run are passed. 
 * The Class implements every control and transformation of the data passed by the user. If no syntax errors occur or no 
 * exceptions are thrown the scanning is carried out by the related methods as described further down. In other case a 
 * related message is printed to the user. 
 * After the portscanning is finished a related message is printed in the TextArea of the Graphical User Interface.
 * 
 * @author Christos Tsopokis
 * @version 1.0
 */

public class PortScanner {

    /**
    * A constant holding the maximum value of threads the application can run.
    */
    static final int MAX_THREADS = 10;
    /**
    * A constant holding the size of bits in a byte.
    */
    static final int SIZE_OF_BYTE = 8;
    /**
    * A constant holding the time (in millisecond) that each thread will wait.
    */
    static final int PATIENCE = 500;
    /**
    * A field holding a long represantation of the first IP given by the user.
    */
    private long firstIP;
    /**
    * A field holding a long represantation of the last IP given by the user.
    */
    private long lastIP;
    /**
    * A field holding an ArrayList in which ports passed by the user are stored as integers.
    */
    private ArrayList<Integer> portList;
    /**
    * A field holding an integral represantation of threads the user passed.
    */
    private int noOfThreads;
    /**
    * A field holding a boolean value. It decides whether down hosts will be printed.
    */
    private boolean verbose = false;

    public boolean isVerbose() {
        return verbose;
    }

    /**
    * A method setting value to verbose field in the GUIScanner Class.
    */
    public void setVerbose(boolean verbose) {
        this.verbose = verbose;
    }

    /**
      * Transform the long integer holding the value of an IP into the respective textual representation (xxx.xxx.xxx.xxx).
      * 
      * @param ip	long integer representation of the IP 
      * @return 	s - textual representation of the address 
      *
    */
    protected static String encodeIP(long ip) {
        String s = "";
        long mask = 255; // A mask of 8 bits equal to 255("11111111") is used
        for (int i = 0; i < 4; i++) {
            int shift_bits = SIZE_OF_BYTE * (3 - i); 
            long temp1 = mask << shift_bits; // Each time the mask is shifted left [SIZE_OF_BYTE * (3 - i)] times
            long temp2 = temp1 & ip; // A login AND takes place between the mask and the long IP 
            long temp3 = temp2 >> shift_bits; // The result is shifted right [SIZE_OF_BYTE * (3 - i)] times
            s = s + temp3; // Then is added in the String s followed by a "."
            if (i < 3) { // The last component is not followed by the dot ".".
                s = s + ".";
            }
        }
        return s;
    }

    /**
      * Checks the range of the IP given by the user. The character in the String must be a textual representation of two 
      * IP addresses seperated by a dash (-). All spaces between the IPs and the dash are ignored.
      * 
      * @param addr	textual representation of the IP range given by the user
      * @return 	true if the range of the IP is valid
      * @throws 	InvalidIPException - if the addr is not a good textual representation of the address range 
      *						e.g. 192.168.1.1-192.168.1.12-192.168.3.33
      * 
    */
    public static boolean checkIPRange(String addr) throws InvalidIPException{
        String[] ipList = addr.split("-");

        if (ipList.length < 1 || ipList.length > 2) {
            throw new InvalidIPException();
        }

        for (int i = 0; i < ipList.length; i++) {
            try {
                checkIP(ipList[i].trim());
            } catch (InvalidIPException ex) {
                System.err.println("Your IP is either syntactically wrong (more than 4 fields) or" +
                        " your IP is out of limits: 0 - 255");
                System.exit(-1);
            }
        }
        return true;
    }

    /**
      * Checks the validity of the IPs given by the user. The character in the String must be a textual representation of one 
      * IP address.
      * 
      * @param addr	textual representation of IP
      * @return 	true if the IP is valid
      * @throws 	InvalidIPException - if the addr is not a good textual representation of the address 
      *						e.g. 192.168.1024.1 or 192.168.1.12.55 or 192.168.a.22
      * 
    */
    public static boolean checkIP(String addr) throws InvalidIPException{

        String[] addrArray = addr.split("\\.");

        if (addrArray.length != 4) {
            throw new InvalidIPException();
        }
        for (int i = 0; i < 4; i++) {
            int num = -1;
            num = Integer.parseInt(addrArray[i]);
            if (num < 0 || num > 255) {
                throw new InvalidIPException();
            }
        }
        return true;
    }

    /**
      * Checks the validity of the Ports given by the user. The character in the String must be a textual representation of the 
      * ports seperated by one space or more.
      * 
      * @param ports	textual representation the ports
      * @return 	true if the ports are valid
      * @throws 	InvalidPortException - if a ports is invalid either numbers out of the limits (1-65535) or characters
      *						e.g. "a 0 65535 88, 443"
      * 
    */
    public static boolean checkPorts(String ports) throws InvalidPortException{
        String[] portList = ports.split("[\\ ]* ");

        for (String str : portList) {
            if (str.trim().length() == 0) {
                continue;
            }
            int port = -1;
            port = Integer.parseInt(str.trim());
            if (port < 1 || port > 65535) {
                throw new InvalidPortException();
            }
        }
        return true;
    }

    /**
      * Transforms the IP given by the user. The character in the String must be a textual representation of one 
      * IP address.
      * 
      * @param addr	textual representation of IP
      * @return 	IP - long integer representation of the IP passed as a String
      * 
    */
    private static long decodeIP(String addr) {
        long IP = 0;

        try {
            checkIP(addr);
        } catch (InvalidIPException ex) {
                System.err.println("Your IP is either syntactically wrong (more than 4 fields) or" +
                        " your IP is out of limits: 0 - 255");
                System.exit(-1);
        }
        String[] addrArray = addr.split("\\.");
        for (int i = 0; i < 4; i++) {
            int num = 0;
            try {
                num = Integer.parseInt(addrArray[i]);
            } catch (NumberFormatException ex) {
                System.err.println("Address should be an integral value!");
                System.exit(-1);
            }
            IP += num << (SIZE_OF_BYTE * (3 - i));
        }
        return IP;
    }

    /**
      * Transforms the Ports given by the user. The character in the String must be a textual representation of the 
      * ports seperated by spaces (" ").
      * 
      * @param ports	textual representation of the ports
      * @return 	an ArrayList of Integers representing the ports
      * 
    */
    private static ArrayList<Integer> decodePorts(String ports) {
        try {
            checkPorts(ports);
        } catch (InvalidPortException ex) {
        }

        String[] portList = ports.split(" "); //parse port input
        ArrayList<Integer> portsArray = new ArrayList<Integer>();
        for (String str : portList) {
            if (str.trim().length() == 0) {
                continue;
            }

            int port = 0;
            try {
                port = Integer.parseInt(str.trim());
            } catch (NumberFormatException ex) {
                System.err.println("Ports should be an integral value!");
            }
            if (!(portsArray.contains(new Integer(port)))) {
                portsArray.add(port);
            }
        }
        return portsArray;
    }

    /**
      * Scans each combination of IPs and Ports using multithreading. Instances of the TreadScanSocket Class are 
      *	created for each combination if the MAX_THREADS to run are not violated. When all threads created finish 
      *	execution Scan finished is printed.
      * 
      * @param jTextArea	The area in the GUI that the result of the PortScanning will be printed
      * 
    */
    public void doScan(JTextArea jTextArea) {
        int threadno = Thread.activeCount(); // threadno hold the number of threads running including the main thread
					     // this is because there are maybe more threads than the main one 
					     // like the threads run by the GUI
        for (int port : portList) {
            for (long ip = this.firstIP; ip <= this.lastIP; ip++) {
                while (Thread.activeCount() >= (noOfThreads + threadno)) {
                    try {
                        Thread.sleep(PATIENCE);
                    } catch (InterruptedException ex) {
                        System.err.println("doScan, wait(): " + ex.getMessage());
                        System.exit(-1);
                    }
                }
                ThreadScanSocket threadScanSocket = new ThreadScanSocket(ip, port, verbose, jTextArea);
                new Thread(threadScanSocket).start();
            }
        }
        while (Thread.activeCount() > threadno) {
            try {
                Thread.sleep(PATIENCE);
            } catch (InterruptedException ex) {
                System.err.println("doScan, wait(): " + ex.getMessage());
                System.exit(-1);
            }
        }
        jTextArea.append("Scan finished.-" + "\n");
        System.out.println("Scan finished.-" + "\n");
    }

    /**
      * Constructs a newly allocated PortScanner object. Firstly the class decodes the parameters passed to the Constructor
      * and then initiates the 4 fields of the Class using and making the necessary methods and ckecks respectively.
      * 
      * @param firstIP		textual representation of the first IP given by the user
      * @param lastIP		textual representation of the last IP given by the user
      * @param portList		textual representation of the list of ports given by the user
      * @param noOfThreads	textual representation of the threads that will run the portscanning
      * 
    */

    public PortScanner(String firstIP, String lastIP, String ports, int noOfThreads) {

        this.firstIP = decodeIP(firstIP);
        this.lastIP = decodeIP(lastIP);
        portList = decodePorts(ports);
        if (portList.size() < 1) {
            System.err.println("No ports given!");
            System.exit(-2);
        }
        this.noOfThreads = (noOfThreads > MAX_THREADS) ? MAX_THREADS : noOfThreads;
        this.noOfThreads = (noOfThreads < 1) ? 1 : this.noOfThreads;
    }
}
