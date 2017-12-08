import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Scanner;

public class Client {
    /**
     * Client IO streams.
     */
    private final PrintStream mOutput, mError;
    private final InputStream mInput;


    /**
     * Interruption flag. If mInterrupt = true all loops
     */
    private boolean mInterrupt;


    /**
     * Client socket channel.
     */
    private final SocketChannel mClientSocketChannel;


    private Scanner mScanner;


    /**
     * Client class constructor. Opens new socket channel for client and bind it.
     *
     * @param ip Server ip address, can't be null.
     * @param port Server port integer value [0, 65535], can't be null.
     * @param input Input stream, can be null (default value is System.in).
     * @param output Output stream, can be null (default value is System.out).
     * @param error Error stream, can be null (default value is System.err)
     * @throws IOException If impossible to open socket channel or bind it.
     */
    Client(@NotNull final InetAddress ip, @NotNull final int port, @Nullable final InputStream input, @Nullable final PrintStream output, @Nullable final PrintStream error) throws IOException {
        // Standard IO streams for server.
        mInput = (input == null) ? System.in: input;
        mOutput = (output == null) ? System.out: output;
        mError = (error == null) ? System.err: error;

        mInterrupt = false;

        // Get address by ip and port.
        InetSocketAddress inetSocketAddress = new InetSocketAddress(ip, port);

        // Open server socket channel.
        mClientSocketChannel =  SocketChannel.open(inetSocketAddress);

        // Set non-blocking mode for client channel.
        mClientSocketChannel.configureBlocking(false);

        // Wait for the connection to finalize.
        while(!mInterrupt && !mClientSocketChannel.finishConnect());

        log(mOutput, null, "Client started on socket " + mClientSocketChannel.getLocalAddress() + ".");
    }


    /***
     * Start scanning input stream.
     */
    void start() {
        mScanner = new Scanner(mInput);

        try {
            // Open new selector and register client channel on it.
            Selector selector = Selector.open();
            mClientSocketChannel.register(selector, mClientSocketChannel.validOps(), null);

            while(!mInterrupt) {
                selector.select(Constants.SELECTION_DELAY);

                Iterator iterator = selector.selectedKeys().iterator();
                while(iterator.hasNext()) {
                    SelectionKey selectionKey = (SelectionKey) iterator.next();

                    iterator.remove();

                    if(!selectionKey.isValid())
                        continue;

                    // Read messages from server.
                    if(selectionKey.isReadable())
                        readHandler(selectionKey);
                    // Write messages to server.
                    else if(selectionKey.isWritable())
                        writeHandler(selectionKey);
                }
            }
        }
        catch(final IOException exception) {
            log(mError, null, "Server is not available.");
        }
        finally {
            finish();
        }
    }


    /***
     * Read messages from server.
     */
    private synchronized void readHandler(@NotNull SelectionKey selectionKey) {
        ByteBuffer buffer = ByteBuffer.allocate(Constants.BUFFER_SIZE);

        // Try to get count of bytes in buffer.
        final int countOfBytesRead;
        try {
            countOfBytesRead = mClientSocketChannel.read(buffer);
        }
        catch(final IOException exception) {
            // Server is not available, try to close socket.
            selectionKey.cancel();
            finish();
            return;
        }

        if(countOfBytesRead < 0) {
            // Server is not available, try to close socket.
            selectionKey.cancel();
            finish();
        }
        else if(countOfBytesRead > 0) {
            // Get message from buffer.
            String message = new String(buffer.array()).trim();
            if(!message.isEmpty())
                log(mOutput, mClientSocketChannel, "Receive message: " + message);

            selectionKey.interestOps(SelectionKey.OP_WRITE);
        }
        else
            selectionKey.interestOps(SelectionKey.OP_WRITE);

    }


    /***
     * Write messages to server.
     */
    private synchronized void writeHandler(@NotNull SelectionKey selectionKey) {
        while(!mInterrupt && !mScanner.hasNextLine());

        final String message = mScanner.nextLine();
        ByteBuffer buffer = ByteBuffer.wrap(message.getBytes());
        try {
            mClientSocketChannel.write(buffer);
        }
        catch(final IOException exception) {
            // Server is not available, try to close socket.
            selectionKey.cancel();
            finish();
            return;
        }

        buffer.clear();
        selectionKey.interestOps(SelectionKey.OP_READ);
    }


    /***
     * Print messages to selected print stream.
     */
    private synchronized void log(@NotNull final PrintStream stream, @Nullable final SocketChannel clientSocket, @NotNull final String message) {
        boolean condition = (clientSocket != null);

        // Get prefix by client socket.
        String prefix = "";
        if(condition) {
            try {
                prefix = clientSocket.getRemoteAddress().toString() + " | ";
            }
            catch(final ClosedChannelException exception) {}
            catch(final IOException exception) {
                exception.printStackTrace();
            }
        }

        stream.println(prefix + message);
    }


    /***
     * Close client socket.
     */
    private void finish() {
        mInterrupt = true;

        log(mOutput, mClientSocketChannel, "Disconnected.");

        try {
            mClientSocketChannel.close();
        }
        catch(final IOException exception) {
            log(mError, null, "It's impossible to close client socket.");
            exception.printStackTrace();
        }
    }


    /***
     * Close client socket.
     */
    protected void finalize() {
        finish();
    }
}
