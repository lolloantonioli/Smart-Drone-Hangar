package it.unibo;

import jssc.*;
import java.util.concurrent.*;

/**
 * Serial communication channel implementation using jSSC library.
 */
public final class SerialCommChannel implements CommChannel, SerialPortEventListener {

    private final BlockingQueue<String> queue;
    private SerialPort serialPort;
    private StringBuffer currentMsg = new StringBuffer("");

    /**
     * Constructor.
     * @param port The serial port name.
     * @param rate The baud rate.
     * @throws SerialPortException If an error occurs during serial port operations.
     */
    public SerialCommChannel(final String port, final int rate) throws SerialPortException {
        queue = new LinkedBlockingQueue<>();
        try {
            serialPort = new SerialPort(port);
            serialPort.openPort();
            serialPort.setParams(rate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
            serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | SerialPort.FLOWCONTROL_RTSCTS_OUT);
            serialPort.addEventListener(this);
        } catch (final SerialPortException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void sendMsg(final String msg) {
        try {
            serialPort.writeString(msg + "\n");
        } catch (final SerialPortException ex) {
            ex.printStackTrace();
        }
    }

    @Override
    public String receiveMsg() throws InterruptedException {
        return queue.take();
    }

    @Override
    public boolean isMsgAvailable() {
        return !queue.isEmpty();
    }

    public void close() {
        try {
            if (serialPort != null) {
                serialPort.removeEventListener();
                serialPort.closePort();
            }
        } catch (final Exception ex) {
            ex.printStackTrace();
        }
    }

    @Override
    public void serialEvent(final SerialPortEvent event) {
        if (event.isRXCHAR()) {
            try {
                String msg = serialPort.readString(event.getEventValue());
                msg = msg.replaceAll("\r", "");
                
                currentMsg.append(msg);
                
                int goAhead = currentMsg.indexOf("\n");
                while (goAhead >= 0) {
                    final String temp = currentMsg.substring(0, goAhead);
                    queue.put(temp);
                    
                    currentMsg = new StringBuffer(currentMsg.substring(goAhead + 1));
                    goAhead = currentMsg.indexOf("\n");
                }
            } catch (final Exception ex) {
                ex.printStackTrace();
            }
        }
    }
}