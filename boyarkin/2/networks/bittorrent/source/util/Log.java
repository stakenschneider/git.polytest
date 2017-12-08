package util;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.io.OutputStream;
import java.io.PrintStream;
import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * Class for logging into console or file.
 * @author Boyarkin Nikita.
 */
public class Log {
    /**
     * Output streams for logging messages.
     * Default values are System.out and System.err.
     */
    private static @Nullable PrintStream sOutputStream, sErrorStream;

    /**
     * Sets output streams.
     * @param outputStream - Stream for normal messages.
     * @param errorStream - Stream for error messages.
     */
    public static void setLogStreams(@NotNull final OutputStream outputStream, @NotNull final OutputStream errorStream) {
        sOutputStream = (outputStream instanceof PrintStream) ? (PrintStream) outputStream : new PrintStream(outputStream);
        sErrorStream = (errorStream instanceof PrintStream) ? (PrintStream) errorStream : new PrintStream(errorStream);
    }

    /**
     * @return Returns current time in HH:mm:ss.SSS format.
     */
    private static @NotNull String getCurrentTime() {
        final Calendar cal = Calendar.getInstance();
        final SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss.SSS");
        return sdf.format(cal.getTime());
    }

    /**
     * @return Returns caller function class name.
     */
    private static @NotNull String getCallerClass() {
        // A kind of hack.
        String callerClassName = new Exception().getStackTrace()[2].getClassName();
        final int firstDot = callerClassName.lastIndexOf(".");
        return callerClassName.substring((firstDot == -1) ? 0 : firstDot + 1);
    }

    /**
     * Prints message into error stream with ERROR level.
     * @param message - Logging message.
     */
    public static synchronized void e(@NotNull final String message) {
        if(sErrorStream == null)
            sErrorStream = System.err;

        sErrorStream.println(getCurrentTime() + " - 0x" + Long.toString(Thread.currentThread().getId(), 16) + " - ERROR - " + getCallerClass() + " - " + message);
    }

    /**
     * Prints message into output stream with INFO level.
     * @param message - Logging message.
     */
    public static synchronized void i(@NotNull final String message) {
        if(sOutputStream == null)
            sOutputStream = System.out;

        sOutputStream.println(getCurrentTime() + " - 0x" + Long.toString(Thread.currentThread().getId(), 16) + " - INFO - " + getCallerClass() + " - " + message);
    }

    /**
     * Prints message into output stream with DEBUG level.
     * @param message - Logging message.
     */
    public static synchronized void d(@NotNull final String message) {
        if(!Constants.DEBUG)
            return;

        if(sOutputStream == null)
            sOutputStream = System.out;

        sOutputStream.println(getCurrentTime() + " - 0x" + Long.toString(Thread.currentThread().getId(), 16) + " - DEBUG - " + getCallerClass() + " - " + message);
    }
}
