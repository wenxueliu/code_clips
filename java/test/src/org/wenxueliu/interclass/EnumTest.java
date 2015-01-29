package org.wenxueliu.interclass;

public class EnumTest{
    public enum WeekDay {
        MON(0),
        TUES(1);

        private int day;

        WeekDay(int day){
            this.day = day;
        }

        public static WeekDay valueOf(int day) {
            switch(day){
                case 0:
                    return WeekDay.MON;
                case 1:
                    return WeekDay.TUES;
                default:
                    throw new IllegalArgumentException("day is not expected(only 0,1)");
            }
        }
    }

    private WeekDay day;
    public EnumTest(int d){
        this.day = WeekDay.valueOf(d);
    }

    public EnumTest(WeekDay d){
        this.day = d;
    }

    public WeekDay getDay(){
        return day;
    }

    public void setDay(WeekDay d){
        day = d;
    }

    public static void info(){
        System.out.println("hello weekday");
    }

    @Override
    public String toString(){
        if (this.day == WeekDay.MON){
            return "Monday";
        } else {
            return "Tuesday";
        }
    }
}
