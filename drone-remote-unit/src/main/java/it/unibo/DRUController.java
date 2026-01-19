package it.unibo;

import javax.swing.SwingUtilities;

/**
 * Controller class for the Drone Remote Unit (DRU).
 */
public final class DRUController extends Thread {
    private CommChannel channel;
    private final View view;
        
    private volatile boolean stop;
    private volatile boolean isConnected;

    private static final int BAUD_RATE = 115200;

    /**
     * Constructor.
     * @param view The view associated with this controller.
     */
    public DRUController(final View view) {
        this.view = view;
        this.stop = false;
        this.isConnected = false;
        this.view.setListener(this);
    }

    @Override
    public void run() {
        while (!stop) {
            try {
                if (isConnected && channel != null) {
                    if (channel.isMsgAvailable()) {
                        final String msg = channel.receiveMsg();
                        processMessage(msg);
                    }
                }
                Thread.sleep(20); 
            } catch (final Exception ex) {
                ex.printStackTrace();
            }
        }
    }

    private void processMessage(final String msg) {
        SwingUtilities.invokeLater(() -> {
            if (msg.startsWith("STATE:")) {
                final String state = msg.substring(6);
                view.updateState(state);
                
                if (!state.equals("ALARM")) {
                    view.setAlarm(false);
                }
            } 
            else if (msg.startsWith("DIST:")) {
                view.updateDistance(msg.substring(5));
            } 
            else if (msg.startsWith("TEMP:")) {
                view.updateTemperature(msg.substring(5));
            } 
            else if (msg.startsWith("MOV:")) {
                view.updateMotion(msg.substring(4));
            } 
            else if (msg.startsWith("ALARM:")) {
                final String val = msg.substring(6);
                view.setAlarm(val.equals("ON"));
            }
        });
    }

    public void onConnect(final String portName) {
        try {
            channel = new SerialCommChannel(portName, BAUD_RATE);
            System.out.println("Controller: Connesso a " + portName);
            isConnected = true;
            view.setConnectionStatus(true);
        } catch (final Exception e) {
            view.showError("Impossibile connettersi: " + e.getMessage());
        }
    }

    public void onDisconnect() {
        try {
            if (channel != null && channel instanceof SerialCommChannel) {
                ((SerialCommChannel) channel).close();
            }
            isConnected = false;
            view.setConnectionStatus(false);
            System.out.println("Controller: Disconnesso.");
        } catch (final Exception e) {
            e.printStackTrace();
        }
    }

    public void onTakeoff() {
        if (isConnected && channel != null) {
            channel.sendMsg("CMD:TAKEOFF");
        }    
    }

    public void onLand() {
        if (isConnected && channel != null) {
            channel.sendMsg("CMD:LAND");
        }    
    }
    
    public void stopController() {
        this.stop = true;
    }
}
