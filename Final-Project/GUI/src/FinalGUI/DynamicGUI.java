package FinalGUI;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Stack;
import java.util.concurrent.TimeUnit;



/**
 * @author Adis Osmankic
 * This class creates a GUI representation of the roomba and the field it will drive on. This class contains the main
 * method, so this class must be the one that is run. The program can switch the Roomba from manual mode to automatic
 * mode by pressing the Automate  button, then back again by pressing the Manual button. When in automatic mode, the
 * Forward, Backward, Left, and Right buttons are used to adjust the calibration of the Roomba. When in manual mode,
 * buttons can be pressed to move the actual roomba. The return button sends the roomba back to its original position.
 *
 * Note: make sure that the computer is connected to the Wifi of the Roomba
 *
 * Note: Run the GUI before starting the roomba
 *
 *
 */
public class DynamicGUI {

    public static void main(String[] args) throws ClassNotFoundException, UnsupportedLookAndFeelException, InstantiationException, IllegalAccessException, IOException {
        DynamicGUI form=new DynamicGUI();
    }

    private JFrame GUI;
    private Container contentpane;
    private JButton leftButton;
    private JButton buttonForward;
    private JButton backwardButton;
    private JButton rightButton;
    private JButton musicButton;
    private JSlider distSlider;
    private JLabel img;
    private JSeparator bottomSeperator;
    private JButton reset;
    private JButton automate;
    private JLabel position;
    private JLabel pos;
    private JSlider angSlider;
    private JLabel curAngle;
    private JLabel curAngleVal;
    private ImageIcon imgIcon;
    private RotatedIcon img2;
    private Timer timer3;
    private double angle=0;
    private boolean isManual=true;
    private Socket socket;
    private boolean wascalled=false;
    private int x, y;
    private static final int MAX_DIST=30;
    private static final int MIN_DIST=5;
    private static final int MIN_ANG=0;
    private static final int MAX_ANG=90;
    private static final double RATIO=1;
    private static final int DIST_INIT=10;
    private static final int ROOMBASIZE=33;
    private static final int GUIWIDTH=787;
    private Stack<moves> stk=new Stack<>();
    private boolean canMove=true;
    private ArrayList<objects> obj=new ArrayList<>();
    private JLabel warning;

    /**
     *
     *
     * This class creates a GUI representation of the roomba and the field it will drive on. This class contains the main
     * method, so this class must be the one that is run. The program can switch the Roomba from manual mode to automatic
     * mode by pressing the Automate  button, then back again by pressing the Manual button. When in automatic mode, the
     * Forward, Backward, Left, and Right buttons are used to adjust the calibration of the Roomba. When in manual mode,
     * buttons can be pressed to move the actual roomba. The return button sends the roomba back to its original position.
     *
     * Note: make sure that the computer is connected to the Wifi of the Roomba
     *
     * Note: Run the GUI before starting the roomba
     * The consturctor creates the GUI by initializing the JFRAME and populating it. The program then just continues to
     * focus on the GUI until it is closed, or the Roomba states that it has finished the course.
     * @throws ClassNotFoundException
     * @throws UnsupportedLookAndFeelException
     * @throws InstantiationException
     * @throws IllegalAccessException
     */
    public DynamicGUI() throws ClassNotFoundException, UnsupportedLookAndFeelException, InstantiationException, IllegalAccessException, IOException {
        GUI=new JFrame("GUI");
        contentpane=GUI.getContentPane();
        contentpane.setLayout(null);
        GUI.setSize(GUIWIDTH,GUIWIDTH+200);

        GUI.setLocation(100, 50);
        GUI.setResizable(false);

        GUI.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        seperatorInit();
        connect();
        roombaInit();
        buttonInit();
        posInit();
        angleInit();
        SlidersInit();



        GUI.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {
                int xgui = e.getX();
                int ygui = e.getY();
                System.out.println(xgui + ", " + ygui);
            }

            @Override
            public void mousePressed(MouseEvent e) {

            }

            @Override
            public void mouseReleased(MouseEvent e) {

            }

            @Override
            public void mouseEntered(MouseEvent e) {

            }

            @Override
            public void mouseExited(MouseEvent e) {

            }
        });



        Timer timer1 = new Timer(1000, new ActionListener() {
            /**
             * A timer that checks for commands from the roomba every second.
             * @param e The event that occurs every second
             */
            @Override
            public void actionPerformed(ActionEvent e) {


                //A new thread is made to prevent the GUI from freezing.
                Thread t = new Thread(() -> {
                    try {
                        String command = readString();
                        System.out.println(command);
                        String str[] = command.split("\\s+|\\)");

                        for (int i = 0; i < str.length; i++) {
                            executeCommand(str[i]);
                        }

                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                });
                t.start();
            }
        });
        timer1.start();
        leftButton.addActionListener(e -> {

            String cmd="a"+angSlider.getValue()+"\0";
            sendString(cmd);

        });



        rightButton.addActionListener(e -> {
            String cmd="d"+angSlider.getValue()+"\0";
            sendString(cmd);

        });

        buttonForward.addActionListener(e -> {

            String cmd="w"+distSlider.getValue()+"\0";
            sendString(cmd);

        });
        musicButton.addActionListener(e -> {
            sendString("m\0");
        });
        backwardButton.addActionListener(e -> {
            String cmd="s"+distSlider.getValue()+"\0";
            sendString(cmd);
        });

        reset.addActionListener(e -> {
            warning.setVisible(true);
            timer3.start();
            sendString("p\0");

        });

        timer3=new Timer(13000, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                warning.setVisible(false);
                timer3.stop();
                GUI.revalidate();
                GUI.repaint();
            }
        });

        automate.addActionListener(e -> {
            sendString("x\0");
        });
        GUI.setVisible(true);

    }
    /**
     * This method is called on a timer. Whenever the method is called, it checks if a command is valid, and if it is,
     * it runs a method that relates to the given command.
     * @param command Commands send from the roomba.
     */
    public void executeCommand(String command){
        if(command.length()>0) {
            System.out.println("Command:"+command);
            char first=command.charAt(0);
            ArrayList<Integer> param=new ArrayList<>();
            switch (first){
                case 'm':
                    String moveCommands[]=command.split("\\(|\\)");
                    param.add(Integer.valueOf(moveCommands[1]));
                    move(param.get(0));

                    break;
                case 'o':
                    String objectCommands[]=command.split("\\(|\\)|,");
                    for (int i = 1; i <objectCommands.length ; i++) {
                        param.add(Integer.valueOf(objectCommands[i]));
                    }
//                    calls object method
                    object(param.get(0),param.get(1),param.get(2));
                    break;
                case 't':
                    String turnCommands[]=command.split("\\(|\\)");
                    //calls the turn method
                    turn(Double.valueOf(turnCommands[1]));
                    if(canMove) {
                        moves mov = new moves(Integer.valueOf(turnCommands[1]), "turn");
                        stk.push(mov);
                    }
                    break;
                case 'c':
                    //cliff has no parameters, so it just calls the Cliff method
                    Cliff();
                    break;
                case 'b':
                    String bumpCommands[]=command.split("\\(|\\)");
                    param.add(Integer.valueOf(bumpCommands[1]));
                    bump(param.get(0));
                    break;
                case 'e':
                    edge();
                    break;
                case 'a':
                    String cangocommands[]=command.split("\\(|\\)");
                    param.add(Integer.valueOf(cangocommands[1]));
                    canGo(param.get(0));
                    break;
                case 'w':
                    warning.setVisible(true);
                    timer3.start();
                break;



            }
        }
    }

    /**
     * This method is called whenever the roomba sends a turn commands(aka the roomba turns). It takes the angel that
     * the roomba is to turn then turns the image and updates the current angle of the roomba.
     *
     * Note: this method does not turn the actual roomba, it just turns the GUI representation of the Roomba.
     * @param giventAngle the degrees it turns relative to its current position.
     */
    public void turn(double giventAngle){

    	timer3.start();
    	warning.setVisible(true);
        if(angle<0){ //right
            angle=img2.getDegrees()+Math.abs(giventAngle);
            if(angle>=360)
                angle=angle-360;

        }
        else{
            angle=img2.getDegrees()-giventAngle;
            if(angle<0)
                angle=360-Math.abs(angle)%360;
        }

        if(angle>=360)
            angle=angle-360;
        if(angle<0)
            angle=360-Math.abs(angle)%360;

        img2.setDegrees(angle);
        curAngleVal.setText(Double.toString(angle));
        RotatedIcon img3= new RotatedIcon(imgIcon, angle);
        img.setIcon(img3);
        img.revalidate();
        GUI.revalidate();
        GUI.repaint();
    }
    /**
     * The move method is called whenever the roomba sends a move() command. The method calculates where to move the
     * roomba on the GUI. Note-this method does move the actual roomba, this just calculates where the roomba should
     * appear on the GUI.
     * @param angle the current angle the roomba is facing
     * @param dist the distance the roomba traveled
     */
    public void move(int dist){
        double angleHold;
        double angleTmp=angle;

        if(canMove) {
            moves mov = new moves(dist, "move");
            stk.push(mov);
        }


        if(dist<0){
            angleTmp=angleTmp-180;
            if(angleTmp<0){
                angleTmp=360+angleTmp;
            }
            dist=Math.abs(dist);
        }


        angleHold=angleTmp*Math.PI/180;


        double newx;
        double newy;

        double xhold=Math.cos(angleHold)*dist*RATIO;
        if(angleTmp>180)
            newx=x-Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold,2));
        else
            newx=x+ Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold,2));
        newy=y- xhold;


        x=(int)newx;
        y=(int)newy;


        if(x<0)
            x=0;
        if(x>GUIWIDTH-ROOMBASIZE)
            x=GUIWIDTH-ROOMBASIZE;
        if(y<0)
            y=0;
        if(y>GUIWIDTH-ROOMBASIZE)
            y=GUIWIDTH-ROOMBASIZE;
        img.setBounds(x, y,ROOMBASIZE,ROOMBASIZE);
        img.revalidate();
        warning.setVisible(true);
        GUI.revalidate();
        GUI.repaint();
        pos.setText("("+(x)+", "+(y)+")");
        timer3.start();
    }

    /**
     * This method is called when the roommba sends a cliff command. This method places an image representing the hole
     * on the GUI. Since the hole takes up an entire and there are 4X7 tiles, the method calculates where to place the
     * hole on the gui by taking the current angle then finding the nearest tile and populating that tile with the hole
     * image.
     */
    public void Cliff(){

        if(!wascalled){
            int ratio=60;
            int horizontalVal=0;
            int verticleVal=0;
            double anglehold=angle;
            double angleRad=0;
            int xhold=x+ROOMBASIZE/2;
            int yhold=y+ROOMBASIZE/2;
            double xhold1;
            int dist=(ROOMBASIZE/2)+5;

            angleRad=anglehold*Math.PI/180;
            xhold1=Math.cos(angleRad)*dist*RATIO;
            if(anglehold>180)
                xhold-=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
            else
                xhold+=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
            yhold-=(int) xhold1;
            horizontalVal=Math.round(xhold/ratio);
            verticleVal=Math.round(yhold/ratio);


            if(verticleVal>13) {
                verticleVal=13;
            }
            if(horizontalVal>13) {
                horizontalVal=13;
            }
            ImageIcon imageIcon=new ImageIcon("hole.png");
            Image image=imageIcon.getImage();
            Image newImg=image.getScaledInstance(ratio,ratio, Image.SCALE_SMOOTH);
            imageIcon=new ImageIcon(newImg);
            JLabel hole = new JLabel(imageIcon);
            hole.setBounds(horizontalVal*ratio,verticleVal*ratio,ratio,ratio);
            contentpane.add(hole);
            GUI.revalidate();
            GUI.repaint();
            wascalled=true;
        }
    }


    /**
     * This connects the java program to the Roomba and prints a message to the console when a connection is successful.
     * This method should only be called once.
     * @throws IOException
     */
    public void connect() throws IOException {
        socket=new Socket("192.168.1.1",288);
        System.out.println("connected");
    }

    /**
     * This method disconnects the roomba from the java program if, say one wanted to connect to putty instead of the
     * gui and the roomba only allows one connection. This method is not necessary for the program to function.
     * @throws IOException
     */
    public void disconnect() throws IOException {
        socket.close();
    }

    //todo
    /**
     * This method is thanks to Adam Corpstein on piazza. This method sends a command to the roomba, such as
     * moveRoomba() or turnRoomba().
     * @param string Command to be sent to the roomba.
     * @throws IOException
     * @throws InterruptedException
     */
    public void sendString(String string) {
        OutputStream outputStream= null;
        try {
            outputStream = socket.getOutputStream();
            OutputStreamWriter writer=new OutputStreamWriter(outputStream, StandardCharsets.US_ASCII);
            byte[] bytes=string.getBytes(StandardCharsets.US_ASCII);
            for (int i = 0; i < bytes.length; i++) {
                outputStream.write(bytes,i,1);
                outputStream.flush();
                Thread.sleep(100);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }


    }

    /**
     * This method reads the commands sent from the roomba and sends them to timer1 to be processed
     * @return Command sent from the roomba
     * @throws IOException
     */
    public String readString() throws IOException {
        InputStream inputStream=socket.getInputStream();
        BufferedReader reader=new BufferedReader(new InputStreamReader(inputStream, StandardCharsets.US_ASCII));
        char[] bytes = new char[256];
        int size=reader.read(bytes);
        return new String(bytes);
    }
    /**
     * This method is called whenever the roomba sends a bump() command. The method intakes which bumper was hit and
     * places an image representation of a small object on the GUI relative to the roomba's current position.
     * @param sensor this is an int number representing which bumper was hit (0=> left, 1=> right, and 2=> both)
     */
    public void bump(int sensor){
        ImageIcon imageIcon=new ImageIcon("small.png");
        Image image=imageIcon.getImage();
        Image newImg=image.getScaledInstance(10,10, Image.SCALE_SMOOTH);
        imageIcon=new ImageIcon(newImg);
        JLabel shortObject = new JLabel(imageIcon);
        double anglehold=angle;
        double angleRad;
        int xhold=x+15;
        int yhold=y+15;
        double xhold1;
        int dist=ROOMBASIZE/2+1;
        if(sensor==0){
            anglehold=angle-30;

        }
        else if(sensor==1) {
            anglehold=angle+60;
        }

        if(anglehold<0){
            anglehold=360+anglehold;
        }
        else if(anglehold>=360) {
            anglehold=anglehold-360;
        }


        angleRad=anglehold*Math.PI/180;
        xhold1=Math.cos(angleRad)*dist*RATIO;
        if(anglehold>180)
            xhold-=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
        else
            xhold+=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
        yhold-=(int) xhold1;

//        xhold=xhold-5;
//        yhold=yhold

        boolean place=true;
//        for (int i = 0; i < obj.size(); i++) {
//            if(obj.get(i).getType().equals("short")){
//                place=obj.get(i).canPlace(xhold,yhold);
//                if(!place){
//                    return;
//                }
//            }
//        }

        objects o=new objects(xhold,yhold,"short");
        obj.add(o);


        xhold=xhold-5;
        yhold=yhold-5;

        shortObject.setBounds(xhold,yhold,10,10);
        contentpane.add(shortObject);
        GUI.revalidate();
        GUI.repaint();

    }



    public void edge(){

        double anglehold=angle;
        double angleRad=0;
        int xhold=x+15;
        int yhold=y+15;
        double xhold1;
        int dist=ROOMBASIZE/2-8;
        angleRad=anglehold*Math.PI/180;
        xhold1=Math.cos(angleRad)*dist*RATIO;
        if(anglehold>180)
            xhold-=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
        else
            xhold+=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
        yhold-=(int) xhold1;


        JSeparator EdgeSeperatorVerticle= new JSeparator();
        JSeparator EdgeSeperatorHorizontal=new JSeparator();
        EdgeSeperatorVerticle.setOrientation(SwingConstants.VERTICAL);
        EdgeSeperatorVerticle.setBounds(xhold,0, 5, 787);
        EdgeSeperatorVerticle.setForeground(Color.RED);


        EdgeSeperatorHorizontal.setOrientation(SwingConstants.HORIZONTAL);
        EdgeSeperatorHorizontal.setBounds(0,yhold, 787, 5);
        EdgeSeperatorHorizontal.setForeground(Color.GREEN);

        contentpane.add(EdgeSeperatorHorizontal);
        contentpane.add(EdgeSeperatorVerticle);
        GUI.revalidate();
        GUI.repaint();


    }


    /**
     * This method is called whenever the roomba sends the object() commands, meaning the roomba has found an object
     * using its IR and Ping sensor. This method then places the object's image representation on the GUI.
     * @param dist  This distance of the object from the roomba
     * @param angleObj The angle of the object relative to the roomba
     * @param type an int representing if the object is a tall object, pillar, or final destination zone. (0=>tall, 1=> pillar)
     */
    public void object(int dist, int angleObj, int type){
        //todo Have to redo a few calculations, should be the same sort of logic as bump().
        //todo Have to add pillar... what does the pillar mean?




        double anglehold1=angle;
        double angleRad1;
        int xhold3=x+16;
        int yhold3=y+16;
        double xhold4;
        int dist1=ROOMBASIZE/2;



        if(anglehold1<0){
            anglehold1=360+anglehold1;
        }
        else if(anglehold1>=360) {
            anglehold1=anglehold1-360;
        }


        angleRad1=anglehold1*Math.PI/180;
        xhold4=Math.cos(angleRad1)*dist1*RATIO;
        if(anglehold1>180)
            xhold3-=(int) Math.sqrt(Math.pow(dist1*RATIO,2)-Math.pow(xhold4,2));
        else
            xhold3+=(int) Math.sqrt(Math.pow(dist1*RATIO,2)-Math.pow(xhold4,2));
        yhold3-=(int) xhold4;

        double anglehold;
        double angleRad;
        int xhold=xhold3;
        int yhold=yhold3;
        double xhold1;


        if(angleObj<90){

            anglehold=angle-(90-angleObj);

        }

        else {
            anglehold=angle+(angleObj-90);

        }
        if(anglehold<0){
            anglehold=360+anglehold;
        }
        else if(anglehold>=360) {
            anglehold=anglehold-360;
        }



        angleRad=anglehold*Math.PI/180;
        xhold1=Math.cos(angleRad)*dist*RATIO;
        if(anglehold>180)
            xhold-=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
        else
            xhold+=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
        yhold-=(int) xhold1;

        if(type==0){
//            boolean place=true;
//            for (int i = 0; i < obj.size(); i++) {
//                if(obj.get(i).getType().equals("pillar")){
//                    place=obj.get(i).canPlace(xhold,yhold);
//                    if(!place){
//                        return;
//                    }
//                }
//            }
            objects o=new objects(xhold,yhold,"pillar");
            obj.add(o);

            ImageIcon imageIcon=new ImageIcon("pillar.png");
            Image image=imageIcon.getImage();
            Image newImg=image.getScaledInstance(10,10, Image.SCALE_SMOOTH);
            imageIcon=new ImageIcon(newImg);
            JLabel shortObject = new JLabel(imageIcon);
            xhold=xhold-5;
            yhold=yhold-5;

            shortObject.setBounds(xhold,yhold,10,10);
            contentpane.add(shortObject);
            GUI.revalidate();
            GUI.repaint();


        }
        else if(type==1){

//            boolean place=true;
//            for (int i = 0; i < obj.size(); i++) {
//                if(obj.get(i).getType().equals("tall")){
//                    place=obj.get(i).canPlace(xhold,yhold);
//                    if(!place){
//                        System.out.println("cannot place");
//                        return;
//                    }
//                }
//            }
            objects o=new objects(xhold,yhold,"tall");
            obj.add(o);
            checkDest();
            ImageIcon imageIcon1=new ImageIcon("tall.png");
            Image image=imageIcon1.getImage();
            Image newImg=image.getScaledInstance(15,15, Image.SCALE_SMOOTH);
            imageIcon1=new ImageIcon(newImg);
            JLabel pillarObject = new JLabel(imageIcon1);
            xhold=xhold-8;
            yhold=yhold-8;
            pillarObject.setBounds(xhold,yhold,15,15);
            contentpane.add(pillarObject);
            GUI.revalidate();
            GUI.repaint();

        }
    }


    public void checkDest(){

        ArrayList<objects> list=new ArrayList<>();
        for (int i = 0; i < obj.size(); i++) {
            if(obj.get(i).getType().equals("pillar")){
                list.add(obj.get(i));
            }
        }

        for (int i = 0; i < list.size()-1; i++) {
            for (int j = i+1; j < list.size(); j++) {

                int x1=obj.get(i).getX();
                int x2=obj.get(j).getX();
                int y1=obj.get(i).getY();
                int y2=obj.get(j).getY();



                double distance=Math.sqrt(Math.pow((x2-x1),2)+Math.pow((y2-y1),2));


                if(distance<=65){
                    destination();
                }

            }
        }
    }


    public void destination(){
//        int ratio=60;
//        int horizontalVal=0;
//        int verticleVal=0;
//        double anglehold=angle;
//        double angleRad=0;
//        int xhold=x+ROOMBASIZE/2;
//        int yhold=y+ROOMBASIZE/2;
//        double xhold1;
//        int dist=(ROOMBASIZE/2)+5;
//
//        angleRad=anglehold*Math.PI/180;
//        xhold1=Math.cos(angleRad)*dist*RATIO;
//        if(anglehold>180)
//            xhold-=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
//        else
//            xhold+=(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold1,2));
//        yhold-=(int) xhold1;
//        horizontalVal=Math.round(xhold/ratio);
//        verticleVal=Math.round(yhold/ratio);
//
//
//        if(verticleVal>13) {
//            verticleVal=13;
//        }
//        if(horizontalVal>13) {
//            horizontalVal=13;
//        }
//        ImageIcon imageIcon=new ImageIcon("Destination.png");
//        Image image=imageIcon.getImage();
//        Image newImg=image.getScaledInstance(ratio,ratio, Image.SCALE_SMOOTH);
//        imageIcon=new ImageIcon(newImg);
//        JLabel hole = new JLabel(imageIcon);
//        hole.setBounds(horizontalVal*ratio,verticleVal*ratio,ratio,ratio);
//        contentpane.add(hole);
//        GUI.revalidate();
//        GUI.repaint();
//        wascalled=true;
    }

//    public void moveToPosition(int xdest,int ydest){
//        while (true){
//            int xprev=x;
//
////                while()
//            move(10);
//
//
//
//        }
//
//
//    }


    public void returnToOrigin() throws InterruptedException {
        canMove=false;
        while(!stk.isEmpty()){
            moves str = stk.pop();
            executeCommand(str.getCommand());
            GUI.repaint();
            GUI.revalidate();
            contentpane.revalidate();
            contentpane.repaint();
            TimeUnit.SECONDS.sleep(16);
        }
        if(stk.isEmpty())
            canMove=true;
    }


    public boolean canGo(int dist){

        double angleHold;
        double angleTmp=angle;

        if(dist<0){
            angleTmp=angleTmp-180;
            if(angleTmp<0){
                angleTmp=360+angleTmp;
            }
            dist=Math.abs(dist);
        }


        angleHold=angleTmp*Math.PI/180;


        int newx=0;
        int newy=0;

        double xhold=Math.cos(angleHold)*dist*RATIO;
        if(angleTmp>180)
            newx=x-(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold,2));
        else
            newx=x+(int) Math.sqrt(Math.pow(dist*RATIO,2)-Math.pow(xhold,2));
        newy=y-(int) xhold;
        boolean canplace=true;
        for (int i = 0; i < obj.size(); i++) {
            canplace=obj.get(i).canPlace(newx,newy);
            if(!canplace){
//                sendString("n\0");
                return false;
            }
        }
//        sendString("y\0");
        return true;

    }



/*
----------------------------------------------------------------------------------------------------------------
------------------------------------------GUI initializations---------------------------------------------------
----------------------------------------------------------------------------------------------------------------
 */


    public void SlidersInit(){
        JLabel sliderText = new JLabel("Distance per click", JLabel.CENTER);
        sliderText.setAlignmentX(Component.CENTER_ALIGNMENT);
        distSlider=new JSlider(JSlider.HORIZONTAL, MIN_DIST,MAX_DIST,DIST_INIT);
        distSlider.setMajorTickSpacing(5);
        distSlider.setMinorTickSpacing(1);
        distSlider.setPaintLabels(true);
        distSlider.setPaintTicks(true);
        distSlider.setSnapToTicks(true);
        distSlider.setBounds(350,887,155,50);
        distSlider.setBorder(BorderFactory.createEmptyBorder(0,0,10,0));
        sliderText.setBounds(350,857,150,50);
        JLabel angleText = new JLabel("Angle to turn", JLabel.CENTER);
        angleText.setAlignmentX(Component.CENTER_ALIGNMENT);
        angSlider=new JSlider(JSlider.HORIZONTAL,MIN_ANG,MAX_ANG,MAX_ANG);
        angSlider.setMajorTickSpacing(10);
        angSlider.setMinorTickSpacing(5);
        angSlider.setPaintLabels(true);
        angSlider.setPaintTicks(true);
        angSlider.setSnapToTicks(true);
        angSlider.setBounds(350,812,155,50);
        angleText.setBounds(350,787,150,50);
        contentpane.add(sliderText);
        contentpane.add(distSlider);
        contentpane.add(angleText);
        contentpane.add(angSlider);

        GUI.revalidate();
        GUI.repaint();
    }


    public void seperatorInit(){
        JSeparator bottomSeperator = new JSeparator();
        bottomSeperator.setOrientation(SwingConstants.HORIZONTAL);
        bottomSeperator.setBounds(0,787, 1500, 9);
        bottomSeperator.setForeground(Color.BLACK);

        contentpane.add(bottomSeperator);
        GUI.revalidate();
        GUI.repaint();

    }

    public void buttonInit(){automate=new JButton("KILL!");
        leftButton=new JButton("Left");
        rightButton=new JButton("Right");
        buttonForward=new JButton("Forward");
        backwardButton=new JButton("Backward");
        reset=new JButton("Scan");
        musicButton=new JButton("Delivered");
        musicButton.setBounds(670,887,100,50);

        leftButton.setBounds(10, 887, 100, 50);
        backwardButton.setBounds(120,887,100,50);
        buttonForward.setBounds(120,812,100,50);
        rightButton.setBounds(230,887,100,50);
        reset.setBounds(10,812,100,50);
        automate.setBounds(230,812,100,50);
        //bind keys to buttons
        rightButton.setMnemonic(KeyEvent.VK_D);
        leftButton.setMnemonic(KeyEvent.VK_A);
        buttonForward.setMnemonic(KeyEvent.VK_W);
        backwardButton.setMnemonic(KeyEvent.VK_S);
        automate.setMnemonic(KeyEvent.VK_E);
        reset.setMnemonic(KeyEvent.VK_Q);

        contentpane.add(leftButton);
        contentpane.add(backwardButton);
        contentpane.add(buttonForward);
        contentpane.add(rightButton);
        contentpane.add(reset);
        contentpane.add(automate);
        contentpane.add(musicButton);

        GUI.revalidate();
        GUI.repaint();
    }


    public void roombaInit(){
        x=(GUIWIDTH/2)-(ROOMBASIZE/2);
        y=x;
        imgIcon=new ImageIcon("roomba.png");
        Image image=imgIcon.getImage();
        Image newImg=image.getScaledInstance(ROOMBASIZE,ROOMBASIZE,Image.SCALE_SMOOTH);
        imgIcon=new ImageIcon(newImg);
        img2=new RotatedIcon(imgIcon,0);
        img=new JLabel(img2);
        img.setBounds(x, y, ROOMBASIZE, ROOMBASIZE);
        ImageIcon imageIcon1=new ImageIcon("red.png");
        Image image1 =imageIcon1.getImage();
        Image newImg1=image1.getScaledInstance(50,50,Image.SCALE_SMOOTH);
        imageIcon1=new ImageIcon(newImg1);
        warning=new JLabel(imageIcon1);
        warning.setVisible(false);
        warning.setBounds(675+25,812,50,50);
        contentpane.add(warning);
        contentpane.add(img);
        GUI.revalidate();
        GUI.repaint();
    }


    /**
     * Initialize the current position label.
     */
    public void posInit(){
        position=new JLabel("Current Position");
        pos=new JLabel();
        position.setAlignmentX(Component.CENTER_ALIGNMENT);
        position.setBounds(550,872, 100,50);
        pos.setAlignmentX(Component.CENTER_ALIGNMENT);
        pos.setBounds(545,887,110,50 );
        Border border= BorderFactory.createBevelBorder(1);
        pos.setBorder(border);
        pos.setText("("+x+", "+y+")");
        pos.setHorizontalAlignment(JLabel.CENTER);
        pos.setVerticalAlignment(JLabel.CENTER);

        contentpane.add(position);
        contentpane.add(pos);
        GUI.revalidate();
        GUI.repaint();
    }

    /**
     * Initialize the Angle label to print the current angle. Note that the positive  angle on the GUI is clockwise,
     * while the positive angle direction for the actual roomba is counterclockwise
     */
    public void angleInit(){
        curAngle=new JLabel("Current Angle");
        curAngle.setBounds(550,797,100,50);
        curAngleVal=new JLabel();
        curAngleVal.setAlignmentX(Component.CENTER_ALIGNMENT);
        curAngleVal.setBounds(545,812,110,50);
        Border border= BorderFactory.createBevelBorder(1);
        curAngleVal.setBorder(border);
        curAngleVal.setText(Double.toString(angle));
        curAngleVal.setHorizontalAlignment(JLabel.CENTER);
        curAngleVal.setVerticalAlignment(JLabel.CENTER);
        curAngle.setHorizontalAlignment(JLabel.CENTER);
        curAngle.setVerticalAlignment(JLabel.CENTER);
        contentpane.add(curAngle);
        contentpane.add(curAngleVal);
        GUI.revalidate();
        GUI.repaint();
    }
}
