package it.unibo;

import javax.swing.*;
import java.awt.*;
import jssc.SerialPortList;

/**
 * View class for the Drone Remote Unit (DRU).
 */
public final class DRUView extends JFrame implements View {

    private final JButton btnTakeoff;
    private final JButton btnLand;
    private final JButton btnConnect;
    private final JButton btnDisconnect;
    private final JComboBox<String> portList;
    private final JLabel lblStatus;
    private final JLabel lblDist;
    private final JLabel lblTemp;
    private final JLabel lblMotion;
    private final JLabel lblAlarm;
    private DRUController controller;

    /**
     * Constructor.
     */
    public DRUView() {
        setTitle("Drone Remote Unit (DRU)");
        setSize(450, 450);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        final JPanel pnlTop = new JPanel();
        portList = new JComboBox<>(SerialPortList.getPortNames());
        btnConnect = new JButton("Connect");
        btnDisconnect = new JButton("Disconnect");
        btnDisconnect.setEnabled(false);
        
        pnlTop.add(new JLabel("Port:"));
        pnlTop.add(portList);
        pnlTop.add(btnConnect);
        pnlTop.add(btnDisconnect);
        add(pnlTop, BorderLayout.NORTH);

        final JPanel pnlCenter = new JPanel(new GridLayout(5, 1));
        pnlCenter.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        lblStatus = new JLabel("State: DISCONNECTED");
        lblDist = new JLabel("Distance (Cm): -");
        lblTemp = new JLabel("Temperature (°C): -");
        lblMotion = new JLabel("Motion: -");
        lblAlarm = new JLabel("Alarm: OFF");
        lblAlarm.setForeground(Color.GREEN);
        
        pnlCenter.add(lblStatus);
        pnlCenter.add(lblDist);
        pnlCenter.add(lblTemp);
        pnlCenter.add(lblMotion);
        pnlCenter.add(lblAlarm);
        add(pnlCenter, BorderLayout.CENTER);

        final JPanel pnlBottom = new JPanel();
        btnTakeoff = new JButton("Take Off");
        btnLand = new JButton("Land");
        btnTakeoff.setEnabled(false);
        btnLand.setEnabled(false);
        pnlBottom.add(btnTakeoff);
        pnlBottom.add(btnLand);
        add(pnlBottom, BorderLayout.SOUTH);

        btnConnect.addActionListener(e -> {
            final String port = (String) portList.getSelectedItem();
            if (port != null && controller != null) {
                controller.onConnect(port);
            } else if (port == null) {
                showError("Select a valid port.");
            }
        });

        btnDisconnect.addActionListener(e -> {
            controller.onDisconnect();
        });

        btnTakeoff.addActionListener(e -> {
            controller.onTakeoff();
        });

        btnLand.addActionListener(e -> {
            controller.onLand();
        });
    }

    @Override
    public void setListener(final DRUController controller) {
        this.controller = controller;
    }

    @Override
    public void setConnectionStatus(final boolean connected) {
        btnConnect.setEnabled(!connected);
        portList.setEnabled(!connected);
        btnDisconnect.setEnabled(connected);
        btnTakeoff.setEnabled(connected);
        btnLand.setEnabled(connected);
        
        if (!connected) {
            lblStatus.setText("State: DISCONNECTED");
            lblAlarm.setText("Alarm: OFF");
            lblAlarm.setForeground(Color.GREEN);
        } else {
            lblStatus.setText("State: INSIDE");
        }
    }

    @Override
    public void updateState(final String state) {
        lblStatus.setText("State: " + state);
    }

    @Override
    public void updateDistance(final String dist) {
        lblDist.setText("Distance (Cm): " + dist);
    }

    @Override
    public void updateTemperature(final String temp) {
        lblTemp.setText("Temperature (°C): " + temp);
    }

    @Override
    public void updateMotion(final String motion) {
        lblMotion.setText("Motion: " + motion);
    }

    @Override
    public void setAlarm(final boolean active) {
        if (active) {
            lblAlarm.setText("Alarm: ON");
            lblAlarm.setForeground(Color.RED);
        } else {
            lblAlarm.setText("Alarm: OFF");
            lblAlarm.setForeground(Color.GREEN);
        }
    }

    @Override
    public void showError(final String msg) {
        JOptionPane.showMessageDialog(this, msg, "Errore", JOptionPane.ERROR_MESSAGE);
    }
    
    @Override
    public void start() {
        setVisible(true);
    }
}