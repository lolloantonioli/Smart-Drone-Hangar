package it.unibo;

/**
 * Interface representing a communication channel for sending and receiving messages from Arduino.
 */
public interface CommChannel {
    /**
     * Sends a message to the Arduino.
     * @param msg The message to send.
     */
    void sendMsg(final String msg);

    /**
     * Receives a message from the Arduino.
     * @return The received message.
     * @throws InterruptedException if the thread is interrupted while waiting for a message.
     */
    String receiveMsg() throws InterruptedException;

    /**
     * Checks if there is a message available to read from the Arduino.
     * @return true if a message is available, false otherwise.
     */
    boolean isMsgAvailable();
}