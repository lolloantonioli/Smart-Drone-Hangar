package it.unibo;

/**
 * View interface for the Drone Remote Unit (DRU) application.
 */
public interface View {
    /**
     * Sets the controller as the listener for user actions.
     * @param controller the DRUController instance to handle user actions.
     */
    void setListener(final DRUController controller);
    
    /**
     * Updates the connection status in the UI.
     * @param connected true if connected, false otherwise.
     */
    void setConnectionStatus(final boolean connected);

    /**
     * Updates the hangar's state display.
     * @param state the current state of the hangar.
     */
    void updateState(final String state);

    /**
     * Updates the distance display.
     * @param distance the current distance reading.
     */
    void updateDistance(final String distance);

    /**
     * Updates the temperature display.
     * @param temperature the current temperature reading.
     */
    void updateTemperature(final String temperature);

    /**
     * Updates the motion status display.
     * @param motion the current motion status.
     */
    void updateMotion(final String motion);

    /**
     * Sets the alarm status in the UI.
     * @param active true if the alarm is active, false otherwise.
     */
    void setAlarm(final boolean active);
    
    /**
     * Displays an error message to the user.
     * @param msg the error message to display.
     */
    void showError(final String msg);

    /**
     * Starts the view by making it visible.
     */
    void start();
}
