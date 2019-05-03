package FinalGUI;

public class moves {
    private String type;
    private int amount;

    public moves(int amount, String type){
        this.type=type;
        this.amount=amount;

    }

    public String getType() {
        return type;
    }

    public int getAmount() {
        return amount;
    }

    public String getCommand(){

        if(type.equals("turn")){
            int angTmp=-amount;
            return "turn("+angTmp+")";
        }
        else{
            int movTmp=-amount;
            return "move("+movTmp+")";
        }
    }
}
