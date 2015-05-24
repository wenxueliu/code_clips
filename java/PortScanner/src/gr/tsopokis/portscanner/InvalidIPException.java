package gr.tsopokis.portscanner;

/**
 * Signals that an InvalidIPException of some sort has occured. This class is a particular class of exceptions produced 
 * if the IPs given are not valid.
 * @author christos
 * @since 1.0
 */
public class InvalidIPException extends Exception {

    /**
     * Creates a new instance of <code>InvalidIPException</code> without detail message.
     */
    public InvalidIPException() {
    }


    /**
     * Constructs an instance of <code>InvalidIPException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public InvalidIPException(String msg) {
        super(msg);
    }
}
