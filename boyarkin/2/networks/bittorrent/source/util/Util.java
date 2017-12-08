package util;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import javafx.application.Platform;
import javafx.scene.control.Alert;

import java.io.IOException;
import java.net.DatagramSocket;
import java.net.ServerSocket;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Useful functions for widely usage.
 * @author Boyarkin Nikita.
 */
public class Util {
    private static @NotNull final String OS = System.getProperty("os.name").toLowerCase();

    /**
     * Checks, that OS is windows.
     * @return Returns true if OS is windows.
     */
    public static @NotNull boolean isWindows() {
        return OS.contains("win");
    }

    /**
     * Checks, that OS is unix.
     * @return Returns true if OS is unix.
     */
    public static @NotNull boolean isUnix() {
        return OS.contains("nix") || OS.contains("nux") || OS.indexOf("aix") > 0;
    }

    /**
     * Checks, that OS is mac.
     * @return Returns true if OS is mac.
     */
    public static @NotNull boolean isMac() {
        return OS.contains("mac");
    }

    /**
     * Checks, that OS is solaris.
     * @return Returns true if OS is solaris.
     */
    public static @NotNull boolean isSolaris() {
        return OS.contains("sunos");
    }


    /**
     * Checks, that file name is valid for current OS..
     * @param fileName - String file name.
     * @return Returns true, if file name is valid.
     */
    public static @NotNull boolean fileNameIsValid(@NotNull final String fileName) {
        Pattern pattern;

        if(isWindows()) {
            pattern = Pattern.compile(
                    "# Match a valid Windows filename (unspecified file system).          \n" +
                    "^                                # Anchor to start of string.        \n" +
                    "(?!                              # Assert filename is not: CON, PRN, \n" +
                    "  (?:                            # AUX, NUL, COM1, COM2, COM3, COM4, \n" +
                    "    CON|PRN|AUX|NUL|             # COM5, COM6, COM7, COM8, COM9,     \n" +
                    "    COM[1-9]|LPT[1-9]            # LPT1, LPT2, LPT3, LPT4, LPT5,     \n" +
                    "  )                              # LPT6, LPT7, LPT8, and LPT9...     \n" +
                    "  (?:\\.[^.]*)?                  # followed by optional extension    \n" +
                    "  $                              # and end of string                 \n" +
                    ")                                # End negative lookahead assertion. \n" +
                    "[^<>:\"/\\\\|?*\\x00-\\x1F]*     # Zero or more valid filename chars.\n" +
                    "[^<>:\"/\\\\|?*\\x00-\\x1F\\ .]  # Last char is not a space or dot.  \n" +
                    "$                                # Anchor to end of string.            ",
                    Pattern.CASE_INSENSITIVE | Pattern.UNICODE_CASE | Pattern.COMMENTS);
        }
        else
            pattern = Pattern.compile("^[^*&%\\s]+$");

        final Matcher matcher = pattern.matcher(fileName);
        return matcher.matches();
    }

    /**
     * Checks, that directory name is valid for current OS.
     * @param directoryName - String directory name.
     * @return Returns true, if directory name is valid.
     */
    public static @NotNull boolean directoryNameIsValid(@NotNull final String directoryName) {
        // Sad, but true.
        return fileNameIsValid(directoryName);
    }

    /**
     * Convert file length to human readable format.
     * @param length - Length of the file.
     * @param si - International System of Units.
     * @return Length string in human readable format.
     */
    public static String lengthToHumanReadableFormat(@NotNull final long length, @NotNull final boolean si) {
        final int unit = si ? 1000 : 1024;

        if (length < unit)
            return length + " B";

        final int exp = (int) (Math.log(length) / Math.log(unit));
        final String prefix = (si ? "kMGTPE" : "KMGTPE").charAt(exp - 1) + (si ? "" : "i");
        return String.format("%.1f %sB", length / Math.pow(unit, exp), prefix);
    }

    /**
     * Shows message dialog.
     * @param type - Type of dialog.
     * @param title - Title of dialog window.
     * @param header - Header string of dialog window.
     * @param message - Message string.
     * @throws InterruptedException If thread is interrupted.
     */
    public static void showMessageDialog(@NotNull final Alert.AlertType type, @Nullable final String title,
                                         @Nullable final String header, @Nullable final String message) throws InterruptedException {
        checkInterrupted(null);

        Platform.runLater(() -> {
            Alert alertDialog = new Alert(type);
            alertDialog.setTitle(title);
            alertDialog.setHeaderText(header);
            alertDialog.setContentText(message);
            alertDialog.showAndWait();
        });
    }

    /**
     * @author Boyarkin Nikita.
     */
    public interface InterruptRelease {
        /**
         * Function, which calls if thread is interrupted.
         */
        void release();
    }

    /**
     * Calls release function and throws an exception if thread is interrupted.
     * @param releaseFunction - Function, which calls if thread is interrupted.
     * @throws InterruptedException If thread is interrupted.
     */
    public static void checkInterrupted(@Nullable final InterruptRelease releaseFunction) throws InterruptedException {
        if(Thread.interrupted()) {
            if(releaseFunction != null)
                releaseFunction.release();

            throw new InterruptedException();
        }
    }

    /**
     * Checks port for availability.
     * @param port - Port for availability check.
     * @return Returns true, if port is available.
     * @throws IllegalArgumentException If integer value out of limits.
     */
    public static boolean isPortAvailable(@NotNull final int port) throws IllegalArgumentException {
        if (port < 0x0 || port >= 0x10000)
            throw new IllegalArgumentException();

        ServerSocket serverSocket = null;
        DatagramSocket datagramSocket = null;
        try {
            // Try to create server socket.
            serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);

            // Try to create datagram socket.
            datagramSocket = new DatagramSocket(port);
            datagramSocket.setReuseAddress(true);

            return true;
        }
        catch (IOException exception) { }
        finally {
            // Close sockets.
            if (datagramSocket != null)
                datagramSocket.close();

            if (serverSocket != null) {
                try { serverSocket.close(); }
                catch (IOException exception) { }
            }
        }

        return false;
    }
}
