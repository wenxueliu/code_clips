package gr.tsopokis.portscanner;

/**
 *
 * @author christos
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {

     /*   String addrInput = "192.168.1.1-192.168.1.12";
        String portInput = "80 88 443";
        String threadInput = "8";
    */
    /*    if (PortScanner.checkIPRange(addrInput) != 0) {
            System.err.println("Wrong IP given.");
        }
        if (PortScanner.checkPorts(portInput) != 0) {
            System.err.println("Wrong ports given.");
        }

        int noOfThreads = Integer.parseInt(threadInput);
        if (noOfThreads > PortScanner.MAX_THREADS) {
            System.out.println("No more than " + PortScanner.MAX_THREADS +
                    "allowed. The program will run with the Maximum value.");
            noOfThreads = PortScanner.MAX_THREADS;
        }
        String[] ipList = addrInput.split("-");
        String firstIP = ipList[0].trim();
        String lastIP = (ipList.length == 1) ? firstIP : ipList[1].trim();

        PortScanner portScanner = new PortScanner(firstIP, lastIP, portInput, noOfThreads);
        portScanner.setVerbose(false);
        portScanner.doScan();*/
    }
}
