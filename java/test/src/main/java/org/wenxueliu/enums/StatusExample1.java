package org.wenxueliu.enums;

public class StatusExample1 {

    public enum Process {
        STATUS_OPEN,
        STATUS_STARTED,
        STATUS_INPROGRESS,
        STATUS_ONHOLD,
        STATUS_COMPLETED,
        STATUS_CLOSED;
    }

    public enum Status {
        STATUS_OPEN(0),
        STATUS_STARTED(1),
        STATUS_INPROGRESS(2),
        STATUS_ONHOLD(3),
        STATUS_COMPLETED(4),
        STATUS_CLOSED(5);

        private final int status;

        Status(int aStatus){
            this.status = aStatus;
        }
        public int status(){
            return this.status;
        }
    }

    private Status status;
    private Process process;

    StatusExample1(Status st, Process pr){
        this.status = st;
        this.process = pr;
    }

    StatusExample1(Process pr){
        this.status = Status.STATUS_OPEN;
        this.process = pr;
    }

    public static void main(String[] args){
        for (Status stat: Status.values()){
            System.out.println(stat + "value is "+ new Integer(stat.status()));
        }
    }
}
