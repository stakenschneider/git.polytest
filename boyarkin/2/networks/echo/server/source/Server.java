import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;


import java.io.*;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.*;
import java.nio.channels.*;
import java.util.Iterator;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ConcurrentHashMap;


public class Server {
    /**
     * Server IO streams.
     */
    private final PrintStream mOutput, mError;
    private final InputStream mInput;


    /**
     * Interruption flag. If mInterrupt = true all loops
     */
    private boolean mInterrupt;


    /**
     * Server socket channel.
     */
    private final ServerSocketChannel mServerSocketChannel;


    /**
     * Hash map, that contains the pair SocketChannel and Message.
     */
    private final ConcurrentHashMap<SocketChannel, String> mClientSockets;


    /**
     * Server class constructor. Opens new socket channel for server and bind it.
     *
     * @param port Server port integer value [0, 65535], can't be null.
     * @param input Input stream, can be null (default value is System.in).
     * @param output Output stream, can be null (default value is System.out).
     * @param error Error stream, can be null (default value is System.err)
     * @throws IOException If impossible to open socket channel or bind it.
     */
    Server(@NotNull final int port, @Nullable final InputStream input, @Nullable final PrintStream output, @Nullable final PrintStream error) throws IOException {
        // Standard IO streams for server.
        mInput = (input == null) ? System.in: input;
        mOutput = (output == null) ? System.out: output;
        mError = (error == null) ? System.err: error;

        mClientSockets = new ConcurrentHashMap<>();

        // Open server socket channel.
        mServerSocketChannel = ServerSocketChannel.open();

        // Bind socket channel on selected port.
        InetSocketAddress inetSocketAddress = new InetSocketAddress(port);
        mServerSocketChannel.bind(inetSocketAddress);

        // Set non-blocking mode for server channel.
        mServerSocketChannel.configureBlocking(false);

        log(mOutput, null, "Server started on socket " + mServerSocketChannel.getLocalAddress() + ".");
    }


    /***
     * Start clients accepting.
     */
    void start() {
        mInterrupt = false;

        Thread commandThread = new CommandThread();
        commandThread.start();

        try {
            // Open new selector and register server channel on it.
            Selector selector = Selector.open();
            mServerSocketChannel.register(selector, mServerSocketChannel.validOps(), null);

            while(!mInterrupt) {
                selector.select(Constants.SELECTION_DELAY);

                Iterator iterator = selector.selectedKeys().iterator();

                while(iterator.hasNext()) {
                    SelectionKey selectionKey = (SelectionKey) iterator.next();

                    iterator.remove();

                    if(!selectionKey.isValid())
                        continue;

                    // Accept new clients.
                    if(selectionKey.isAcceptable())
                        acceptHandler(selector);
                    // Read messages from client.
                    else if(selectionKey.isReadable())
                        readHandler(selectionKey);
                    // Write messages to client.
                    else if(selectionKey.isWritable())
                        writeHandler(selectionKey);
                }
            }
        }
        catch(final IOException exception) {
            log(mError, null, "It's impossible to use selector.");
            exception.printStackTrace();
        }
        finally {
            finish();
        }
    }


    /***
     * Execute server commands.
     */
    public class CommandThread extends Thread {
        private static final String EXIT_COMMAND = "exit";
        private static final String DETACH_COMMAND = "detach";

        public void run() {
            Scanner scanner = new Scanner(mInput);

            while(!mInterrupt) {
                // Wait commands.
                while(!mInterrupt && !scanner.hasNextLine());

                final String message = scanner.nextLine().trim().toLowerCase();

                if(message.equals(EXIT_COMMAND))
                    finish();
                else if(message.startsWith(DETACH_COMMAND)) {
                    final String[] split = message.split(" ");

                    if(split.length != 2)
                        log(mError, null, "Wrong command.");

                    try {
                        // Try to find client by remote address.
                        boolean find = false;

                        for(final Map.Entry<SocketChannel, String> current : mClientSockets.entrySet()) {
                            final SocketChannel socketChannel = current.getKey();

                            if(socketChannel.getRemoteAddress().toString().equals(split[1])) {
                                find = true;
                                terminateClient(socketChannel);
                                break;
                            }
                        }

                        if(!find)
                            throw new Exception();

                    }
                    catch(final Exception exception) {
                        log(mError, null, "It's impossible to find client by socket.");
                    }
                }
            }
        }

    }


    /***
     * Accept client function.
     */
    private synchronized void acceptHandler(@NotNull Selector selector) {
        // Try to accept new client and set non-blocking mode for him.
        final SocketChannel clientSocketChannel;
        try {
            clientSocketChannel = mServerSocketChannel.accept();
            clientSocketChannel.configureBlocking(false);
            clientSocketChannel.register(selector, SelectionKey.OP_READ);
        }
        catch(final IOException exception) {
            log(mError, null, "It's impossible to accept client.");
            exception.printStackTrace();
            return;
        }

        mClientSockets.put(clientSocketChannel, "");
        log(mOutput, clientSocketChannel, "Client connected.");
    }


    /***
     * Read messages from clients.
     */
    private synchronized void readHandler(@NotNull SelectionKey selectionKey) {
        SocketChannel clientSocketChannel = (SocketChannel) selectionKey.channel();
        ByteBuffer buffer = ByteBuffer.allocate(Constants.BUFFER_SIZE);

        // Try to get count of bytes in buffer.
        final int countOfBytesRead;
        try {
            countOfBytesRead = clientSocketChannel.read(buffer);
        }
        catch(final IOException exception) {
            // Client disconnected, try to close socket.
            selectionKey.cancel();
            terminateClient(clientSocketChannel);
            return;
        }

        if(countOfBytesRead < 0) {
            // Client disconnected, try to close socket.
            selectionKey.cancel();
            terminateClient(clientSocketChannel);
        }
        else if(countOfBytesRead > 0) {
            // Get message from buffer.
            String message = new String(buffer.array()).trim();
            if(!message.isEmpty())
                log(mOutput, clientSocketChannel, "Receive message: " + message);

            mClientSockets.put(clientSocketChannel, message);

            selectionKey.interestOps(SelectionKey.OP_WRITE);
        }
        else
            selectionKey.interestOps(SelectionKey.OP_READ);
    }


    /***
     * Write messages to clients.
     */
    private synchronized void writeHandler(@NotNull SelectionKey selectionKey) {
        SocketChannel clientSocketChannel = (SocketChannel) selectionKey.channel();

        // Try to get message from reader.
        String message = mClientSockets.get(clientSocketChannel);
        if(!message.isEmpty()) {
            try {
                ByteBuffer buffer = ByteBuffer.wrap(message.getBytes());
                clientSocketChannel.write(buffer);
            }
            catch(final IOException exception) {
                // Client disconnected, try to close socket.
                selectionKey.cancel();
                terminateClient(clientSocketChannel);
            }
        }

        mClientSockets.put(clientSocketChannel, "");
        selectionKey.interestOps(SelectionKey.OP_READ);
    }


    /***
     * Close socket channel and remove it from hash map.
     */
    private synchronized void terminateClient(@NotNull final SocketChannel clientSocketChannel) {
        log(mOutput, clientSocketChannel, "Client disconnected.");

        // Remove socket channel from hash map.
        mClientSockets.remove(clientSocketChannel);

        // Try to close socket channel.
        try {
            clientSocketChannel.close();
        }
        catch(final IOException exception) {
            log(mError, clientSocketChannel, "It's impossible to close socket.");
            exception.printStackTrace();
        }
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
            catch(final ClosedChannelException exceptions) {}
            catch(final IOException exception) {
                exception.printStackTrace();
            }
        }

        stream.println(prefix + message);
    }


    /***
     * Close all sockets.
     */
    private void finish() {
        mInterrupt = true;

        for(final Map.Entry<SocketChannel, String> current: mClientSockets.entrySet())
            terminateClient(current.getKey());

        try {
            mServerSocketChannel.close();
        }
        catch(final IOException exception) {
            log(mError, null, "It's impossible to close server socket.");
            exception.printStackTrace();
        }
    }


    /***
     * Close all sockets.
     */
    protected void finalize() {
        finish();
    }
}
