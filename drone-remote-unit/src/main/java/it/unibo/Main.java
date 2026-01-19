package it.unibo;

import javax.swing.SwingUtilities;

/**
 * Main class to start the Drone Remote Unit (DRU) application.
 */
public final class Main {
    public static void main(final String[] args) {
        SwingUtilities.invokeLater(() -> {
            final View view = new DRUView();
            final DRUController controller = new DRUController(view);
            controller.start();
            view.start();
        });
    }
}
