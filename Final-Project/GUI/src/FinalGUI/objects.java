package FinalGUI;

public class objects {

    private String type;
    private int x;
    private int y;


    public objects(int givenx, int giveny, String type){
        this.x=givenx;
        this.y=giveny;
        this.type=type;
    }

    public String getType() {
        return type;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    /**
     * 0->xmin
     * 1->xmax
     * 2->ymin
     * 3->ymax
     * @return
     */
    public int[] range(){
        int[] rangeOfObject=new int[4];
        if(type.equals("short")){
            rangeOfObject[0]=x+20;
            rangeOfObject[2]=x-15;
            rangeOfObject[1]=y+15;
            rangeOfObject[3]=y-20;
        }
        else if(type.equals("tall")){
            rangeOfObject[0]=x+20;
            rangeOfObject[2]=x-15;
            rangeOfObject[1]=y+15;
            rangeOfObject[3]=y-20;
        }
        else {
            rangeOfObject[0]=x+10;
            rangeOfObject[2]=x;
            rangeOfObject[1]=y+10;
            rangeOfObject[3]=y;
        }

        return rangeOfObject;
    }


    public boolean canPlace(int xcoord, int ycoord){
        int[] ranges=range();
        int xmax=ranges[0];
        int xmin=ranges[2];
        int ymax=ranges[1];
        int ymin=ranges[3];
        int xcoordmax=xcoord+15;
        int ycoordmax=ycoord+15;
        if((xcoord>=xmin&&xcoord<=xmax&&ycoord>=ymin&&ycoord<=ymax)||(xcoordmax>=xmin&&xcoordmax<=xmax&&ycoordmax>=ymin&&ycoordmax<=ymax))
            return false;
        else
            return true;
    }

    public boolean canPlaceObj(int xcoord, int ycoord){
        int[] ranges=range();
        int xmax=ranges[0];
        int xmin=ranges[2];
        int ymax=ranges[1];
        int ymin=ranges[3];


        if((xcoord>=xmin&&xcoord<=xmax&&ycoord>=ymin&&ycoord<=ymax))
            return false;
        else
            return true;
    }

}
