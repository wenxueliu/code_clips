package gr.tsopokis.portscanner;

/**
 * Signals that an InvalidPortException of some sort has occured. This class is a particular class of exceptions produced 
 * if the ports given are not valid.
 * @author christos
 * @since 1.0
 */
public class InvalidPortException extends Exception {

    /**
     * Creates a new instance of <code>InvalidPortException</code> without detail message.
     */
    public InvalidPortException() {
    }


    /**
     * Constructs an instance of <code>InvalidPortException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public InvalidPortException(String msg) {
        super(msg);
    }
}
