package interaction.peerToPeer;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import gui.PeerTableData;
import interaction.fileAnalyser.FileAnalyzer;
import interaction.fileAnalyser.FilePartition;
import interaction.fileAnalyser.OnePieceChangeable;
import interaction.torrentParser.TorrentParser;
import interaction.trackerService.OnePeerChangeable;
import interaction.trackerService.Peer;
import interaction.trackerService.TrackerInfo;
import interaction.trackerService.TrackerService;
import javafx.application.Platform;
import javafx.collections.ObservableList;
import util.Log;
import util.Util;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.*;
import java.util.concurrent.*;

/**
 * Class, that manages all peer to peer connections.
 * @author Boyarkin Nikita.
 */
public class PeerToPeerManager implements OperationalPieceAllocatable, OnePieceChangeable, OnePeerChangeable {
    /** Peer reconnection timeout (in seconds). */
    public static int PEER_RECONNECT_INTERVAL = 20;

    /** Speed measure timeout (in seconds). */
    public static int SPEED_MEASURE_INTERVAL = 1;

    /** Selection delay (in milliseconds). */
    public static int SELECTION_DELAY = 1;

    /** Transmittable buffer size (in bytes). */
    public static int BUFFER_SIZE = 0x20000;

    /**
     * TorrentParser object.
     * @see TorrentParser
     */
    private @NotNull final TorrentParser mTorrentParser;

    /**
     * FileAnalyzer object.
     * @see FileAnalyzer
     */
    private @NotNull final FileAnalyzer mFileAnalyzer;

    /**
     * TrackerService object.
     * @see TrackerService
     */
    private @NotNull final TrackerService mTrackerService;

    /**
     * OperationalPieces object.
     * @see OperationalPieces
     */
    private @NotNull final OperationalPieces mOperationalPieces;

    /**
     * PeerTableView list.
     * @see gui.Controller
     */
    private @NotNull final ObservableList<PeerTableData> mPeersList;

    /**
     * Map, that contains all peer to peer information.
     * @see Peer
     * @see PeerToPeerInformation
     */
    private @NotNull final Map<Peer, PeerToPeerInformation> mConnections;

    /** Server socket channel, opened on mPeerPort port. */
    private @NotNull final ServerSocketChannel mServerSocketChannel;

    /** Peer port number. */
    private @NotNull final int mPeerPort;

    /**
     * Thread pool executor for PeerToPeerRunnable threads.
     * @see PeerToPeerRunnable
     */
    private @NotNull final ExecutorService mThreadExecutor;

    /**
     * Timer for ReconnectTask, that reconnect with one second interval.
     * @see ReconnectTask
     */
    private @NotNull final Timer mReconnectTimer;

    /**
     * Timer for SpeedMeasureTask, that reconnect with SPEED_MEASURE_INTERVAL interval.
     * @see SpeedMeasureTask
     */
    private @NotNull final Timer mSpeedMeasureTimer;

    /** Callback, that calls, when uploading or sharing speed is changed. */
    private @Nullable final SpeedChangeable mSpeedChangeable;

    /** Buffer with BUFFER_SIZE length. */
    private @NotNull final ByteBuffer mNioByteBuffer;

    /** True, if new peer was created. */
    private boolean mConnectionNeeded;

    /**
     * Connection states.
     */
    enum ConnectionState {
        INITIAL, CONNECTED, DISCONNECTED
    }

    /**
     * Information about peer.
     * @author Boyarkin Nikita.
     */
    class PeerToPeerInformation {
        /** Socket channel for current peer. */
        public @Nullable SocketChannel channel;

        /** Connection state for current peer. */
        public @NotNull ConnectionState state;

        /**
         * If state != ConnectionState.DISCONNECTED equals null,
         * if state == ConnectionState.DISCONNECTED equals time to reconnect (in seconds).
         */
        public @Nullable Integer reconnect;

        /**
         * PeerToPeerConnection object.
         * @see PeerToPeerConnection
         */
        public @NotNull final PeerToPeerConnection connection;

        /**
         * Data model for current peer.
         * @see PeerTableData
         */
        public @NotNull final PeerTableData peerTableData;

        /**
         * Creates PeerToPeerInformation object.
         * @param connection - PeerToPeerConnection object.
         * @see PeerToPeerConnection
         * @param peerTableData - Data model for current peer.
         * @see PeerTableData
         */
        public PeerToPeerInformation(@NotNull final PeerToPeerConnection connection, @NotNull final PeerTableData peerTableData) {
            this.channel = null;
            this.state = ConnectionState.INITIAL;
            this.reconnect = null;
            this.connection = connection;
            this.peerTableData = peerTableData;
            peerTableData.setState("Initial");
        }
    }

    /**
     * Creates PeerToPeerManager object.
     * @param peersList - PeerTableView list.
     * @see gui.Controller
     * @param speedChangeable - SpeedChangeable callback (can be null).
     * @param torrentParser - TorrentParser object.
     * @see TorrentParser
     * @param fileAnalyzer - FileAnalyzer object.
     * @see FileAnalyzer
     * @param trackerService - TrackerService object.
     * @see TrackerService
     * @param peerPort - Peer port number.
     * @throws IOException If impossible to open server socket channel.
     * @throws InterruptedException If thread is interrupted.
     */
    public PeerToPeerManager(@NotNull final ObservableList<PeerTableData> peersList, @Nullable final SpeedChangeable speedChangeable, @NotNull final TorrentParser torrentParser, @NotNull final FileAnalyzer fileAnalyzer,
                             @NotNull final TrackerService trackerService, @NotNull final int peerPort) throws IOException, InterruptedException {
        mTorrentParser = torrentParser;
        mFileAnalyzer = fileAnalyzer;
        mTrackerService = trackerService;

        mOperationalPieces = new OperationalPieces(mFileAnalyzer, mTorrentParser, this);
        mPeerPort = peerPort;

        mPeersList = peersList;
        mSpeedChangeable = speedChangeable;

        // Create PeerToPeerInformation objects for every peer, and put it into map.
        mConnections = new HashMap<>();
        for(final Peer currentPeer: mTrackerService.getPeerSet()) {
            // Add peer table data into list.
            final PeerTableData peerTableData = new PeerTableData(-1, currentPeer.ip, currentPeer.port, "", "", "");
            Platform.runLater(() -> mPeersList.add(peerTableData));

            // Create PeerToPeerInformation object.
            final PeerToPeerConnection connection = new PeerToPeerConnection(mTorrentParser, mFileAnalyzer, mTrackerService, currentPeer, mOperationalPieces);
            final PeerToPeerInformation information = new PeerToPeerInformation(connection, peerTableData);

            mConnections.put(currentPeer, information);
        }

        Util.checkInterrupted(null);

        mFileAnalyzer.setOnOnePieceChanged(this);
        mTrackerService.setOnOnePeerChanged(this);

        // Open server TCP channel.
        mServerSocketChannel = ServerSocketChannel.open();

        try {
            // Bind socket channel on selected port.
            InetSocketAddress inetSocketAddress = new InetSocketAddress(mPeerPort);
            mServerSocketChannel.bind(inetSocketAddress);

            // Set non-blocking mode for server channel.
            mServerSocketChannel.configureBlocking(false);
        }
        catch(final IOException exception) {
            mServerSocketChannel.close();
        }

        // Allocate transmittable buffer.
        mNioByteBuffer = ByteBuffer.allocate(BUFFER_SIZE);

        // Create new ThreadPoolExecutor.
        mThreadExecutor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());

        mConnectionNeeded = true;

        // Start new ReconnectTask with one second interval.
        mReconnectTimer = new Timer();
        mReconnectTimer.scheduleAtFixedRate(new ReconnectTask(), PEER_RECONNECT_INTERVAL * 1000, 1000);

        // Start new SpeedMeasureTask with SPEED_MEASURE_INTERVAL interval.
        if(mSpeedChangeable != null) {
            mSpeedMeasureTimer = new Timer();
            mSpeedMeasureTimer.scheduleAtFixedRate(new SpeedMeasureTask(), 1000, SPEED_MEASURE_INTERVAL * 1000);
        }
        else
            mSpeedMeasureTimer = null;

        start();
    }

    /**
     * Starts the connections processing.
     * @throws InterruptedException If thread is interrupted.
     */
    private void start() throws InterruptedException {
        try {
            // Open new selector and register server channel on it.
            Selector selector = Selector.open();
            mServerSocketChannel.register(selector, mServerSocketChannel.validOps(), null);

            while(true) {
                Util.checkInterrupted(null);
                selector.select(SELECTION_DELAY);

                // Try to open new connections, if it's needed.
                registerConnections(selector);

                final Iterator iterator = selector.selectedKeys().iterator();
                while(iterator.hasNext()) {
                    SelectionKey selectionKey = (SelectionKey) iterator.next();
                    iterator.remove();

                    if(!selectionKey.isValid())
                        continue;

                    // Connect new clients.
                    if(selectionKey.isConnectable())
                        connectHandler(selectionKey);
                    // Accept new clients.
                    else if(selectionKey.isAcceptable())
                        acceptHandler(selectionKey);
                        // Read messages from client.
                    else if(selectionKey.isReadable())
                        readHandler(selectionKey);
                }
            }
        }
        catch(final IOException exception) {
            Log.e("Impossible to create selector.");
        }
        finally {
            finishAllPeers();
        }
    }

    /**
     * Tries to open new connections, if it's needed.
     * @param selector - Java nio selector.
     * @throws InterruptedException If thread is interrupted.
     */
    private void registerConnections(@NotNull final Selector selector) throws InterruptedException {
        if(!mConnectionNeeded)
            return;

        mConnectionNeeded = false;

        for(final Map.Entry<Peer, PeerToPeerInformation> currentPeer: mConnections.entrySet()) {
            final Peer peer = currentPeer.getKey();
            final PeerToPeerInformation information = currentPeer.getValue();

            // Connect new peer or reconnect existing peer.
            if(information.state == ConnectionState.INITIAL || (information.state == ConnectionState.DISCONNECTED && information.reconnect != null && information.reconnect == 0)) {
                information.connection.refreshParameters();

                SocketChannel clientSocketChannel;
                try {
                    clientSocketChannel = SocketChannel.open();
                } catch(final IOException exception) {
                    information.state = ConnectionState.DISCONNECTED;
                    information.peerTableData.setState("Disconnected");
                    information.peerTableData.setUploadingSpeed("");
                    information.peerTableData.setSharingSpeed("");
                    information.reconnect = PEER_RECONNECT_INTERVAL;
                    information.channel = null;
                    continue;
                }

                Util.checkInterrupted(null);

                try {
                    // Configure socket channel.
                    clientSocketChannel.configureBlocking(false);
                    clientSocketChannel.connect(new InetSocketAddress(peer.ip, peer.port));
                    clientSocketChannel.register(selector, SelectionKey.OP_CONNECT, information);
                }
                catch(final IOException exception) {
                    finishPeer(clientSocketChannel, information);
                }
            }
        }
    }

    /**
     * Tries to accept new clients.
     * @param selectionKey - Java nio selection key.
     */
    private void acceptHandler(@NotNull final SelectionKey selectionKey) {
        final SocketChannel clientSocketChannel;
        try {
            clientSocketChannel = mServerSocketChannel.accept();
        }
        catch(final IOException exception) {
            return;
        }

        // Try to find peer into connections map.
        final Peer peer = Peer.getPeerFromSocket(clientSocketChannel.socket());
        final PeerToPeerInformation information = mConnections.get(peer);
        if(information == null) {
            finishChannel(clientSocketChannel);
            return;
        }

        setPeerInformation(clientSocketChannel, information, selectionKey);
    }

    /**
     * Tries to connect new clients.
     * @param selectionKey - Java nio selection key.
     */
    private void connectHandler(@NotNull final SelectionKey selectionKey) {
        final SocketChannel clientSocketChannel = (SocketChannel) selectionKey.channel();
        final PeerToPeerInformation information = (PeerToPeerInformation) selectionKey.attachment();

        try {
            // If connection is finished, set peer state to ConnectionState.CONNECTED.
            if (clientSocketChannel.finishConnect())
                setPeerInformation(clientSocketChannel, information, selectionKey);
            else
                finishPeer(clientSocketChannel, information);
        }
        catch(final IOException exception) {
            finishPeer(clientSocketChannel, information);
        }
    }

    /**
     * Sends handshake notify. Sets peer state to ConnectionState.CONNECTED.
     * @param clientSocketChannel - Peer socket channel.
     * @param information - PeerToPeerInformation object.
     * @param selectionKey - Java nio selection key.
     */
    private void setPeerInformation(@NotNull final SocketChannel clientSocketChannel, @NotNull final PeerToPeerInformation information,
                                    @NotNull final SelectionKey selectionKey) {
        try {
            clientSocketChannel.configureBlocking(false);

            try {
                // Send handshake notify.
                information.connection.handshakeNotify(clientSocketChannel);
            }
            catch(final PeerToPeerConnection.BitTorrentException exception) {
                Log.e(information.connection.getPeerInformation().toString() + " Impossible to set handshake state: " + exception.getMessage());
            }

            Log.i(information.connection.getPeerInformation().toString() + " Client " + ((information.state == ConnectionState.INITIAL) ? "connected.": "reconnected."));

            information.channel = clientSocketChannel;
            information.reconnect = null;
            information.state = ConnectionState.CONNECTED;
            information.peerTableData.setState("Connected");

            selectionKey.interestOps(SelectionKey.OP_READ);
        }
        catch(final IOException exception) {
            finishPeer(clientSocketChannel, information);
        }
    }

    /**
     * Tries to read messages from clients.
     * @param selectionKey - Java nio selection key.
     */
    private void readHandler(@NotNull final SelectionKey selectionKey) {
        final SocketChannel clientSocketChannel = (SocketChannel) selectionKey.channel();

        // Try to find peer into connections map.
        final Peer peer = Peer.getPeerFromSocket(clientSocketChannel.socket());
        final PeerToPeerInformation information = mConnections.get(peer);
        if(information == null || information.state != ConnectionState.CONNECTED) {
            selectionKey.interestOps(SelectionKey.OP_READ);
            return;
        }

        int countOfBytes;
        try {
            // Try to read into transmittable buffer.
            mNioByteBuffer.clear();
            countOfBytes = clientSocketChannel.read(mNioByteBuffer);

            if(countOfBytes == 0)
                return;
            else if(countOfBytes < 0)
                throw new IOException("The connection is broken by peer.");
        }
        catch(final IOException exception) {
            Log.e(information.connection.getPeerInformation().toString() + " Impossible to read. Connection refused with peer with message: " + exception.getMessage());
            finishPeer(information);
            return;
        }

        // Create new PeerToPeerRunnable for buffer.
        mThreadExecutor.submit(new PeerToPeerRunnable(information, selectionKey, Arrays.copyOf(mNioByteBuffer.array(), countOfBytes)));
        selectionKey.interestOps(SelectionKey.OP_READ);
    }

    /**
     * Finishes work with peer.
     * @param information - PeerToPeerInformation object.
     */
    private void finishPeer(@NotNull final PeerToPeerInformation information) {
        Log.i(information.connection.getPeerInformation().toString() + " Peer disconnected.");

        finishPeer(information.channel, information);
    }

    /**
     * Finishes work with peer (but other socket channel).
     * @param channel - Socket channel for closing.
     * @param information - PeerToPeerInformation object.
     */
    private void finishPeer(@NotNull final SocketChannel channel, @NotNull final PeerToPeerInformation information) {
        if(information.state == ConnectionState.CONNECTED) {
            try {
                information.connection.finish(channel);
            }
            catch(final IOException exception) { }
        }

        finishChannel(channel);

        information.state = ConnectionState.DISCONNECTED;
        information.peerTableData.setState("Disconnected");
        information.peerTableData.setUploadingSpeed("");
        information.peerTableData.setSharingSpeed("");
        information.reconnect = PEER_RECONNECT_INTERVAL;
        information.channel = null;
    }

    /**
     * Closes socket channel.
     * @param channel - Socket channel for closing.
     */
    private void finishChannel(@NotNull final SocketChannel channel) {
        if(channel == null)
            return;

        try { channel.close(); }
        catch(final IOException closeException) {
            Log.e("Impossible to close client socket channel.");
        }
    }

    /**
     * Finishes work with all peers, stops all timer tasks and threads.
     */
    private void finishAllPeers() {
        // Stop reconnect task.
        if(mReconnectTimer != null) {
            mReconnectTimer.cancel();
            mReconnectTimer.purge();
        }

        // Stop speed measure task.
        if(mSpeedMeasureTimer != null) {
            mSpeedMeasureTimer.cancel();
            mSpeedMeasureTimer.purge();
        }

        // Interrupt all PeerToPeerRunnable threads.
        mThreadExecutor.shutdownNow();

        // Finish work with all peers.
        for(final Map.Entry<Peer, PeerToPeerInformation> currentPeer: mConnections.entrySet())
            finishPeer(currentPeer.getValue());

        // Close server socket channel.
        try { mServerSocketChannel.close(); }
        catch(final IOException exception) {
            Log.e("Impossible to close server socket channel.");
        }
    }

    /**
     * Callback function. Sends 'interested' notifications, if needed.
     * @param pieceId - New operational piece identifier.
     */
    @Override
    public void onOperationPieceAllocated(@NotNull final int pieceId) {
        Log.i("New operational piece #" + pieceId + " is allocated.");

        if(mConnections == null)
            return;

        for(final Map.Entry<Peer, PeerToPeerInformation> currentInformation: mConnections.entrySet()) {
            final PeerToPeerInformation information = currentInformation.getValue();

            if(information.state == ConnectionState.CONNECTED) {
                try {
                    // Send 'interested' notifications, if needed.
                    information.connection.interestedNotify(information.channel);
                }
                catch(final IOException exception) {
                    Log.e(information.connection.getPeerInformation().toString() + " Impossible to send piece notification. Connection refused with peer with message: " + exception.getMessage());
                    finishPeer(information);
                }
            }
        }
    }


    /**
     * Callback function. Sends 'have' notifications, if needed.
     * @param filePartition - File partition of piece.
     * @param pieceId - Piece identifier.
     */
    @Override
    public void onOnePieceChanged(@NotNull final FilePartition filePartition, @NotNull final int pieceId) {
        Log.i("Piece #" + pieceId + " is completed.");

        for(final Map.Entry<Peer, PeerToPeerInformation> currentInformation: mConnections.entrySet()) {
            final PeerToPeerInformation information = currentInformation.getValue();

            if(information.state == ConnectionState.CONNECTED) {
                try {
                    // Send 'have' notifications, if needed.
                    information.connection.haveNotify(pieceId, information.channel);
                }
                catch(final IOException exception) {
                    Log.e(information.connection.getPeerInformation().toString() + " Impossible to send have notification. Connection refused with peer with message: " + exception.getMessage());
                    finishPeer(information);
                }
            }
        }
    }

    /**
     * Callback function. Creates new peers, if needed.
     * @param trackerInfo - TrackerInfo object, that called function.
     * @param peers - Unique set of peers, that changed.
     */
    @Override
    public void onOnePeerChanged(@NotNull final TrackerInfo trackerInfo, @NotNull final Set<Peer> peers) {
        Log.i("New peers added.");

        boolean needed = false;

        // Create PeerToPeerInformation objects for every peer, and put it into map.
        for(final Peer currentPeer: peers)
            if(!mConnections.containsKey(currentPeer)) {
                // Add peer table data into list.
                final PeerTableData peerTableData = new PeerTableData(-1, currentPeer.ip, currentPeer.port, "", "", "");
                Platform.runLater(() -> mPeersList.add(peerTableData));

                // Create PeerToPeerInformation object.
                final PeerToPeerConnection connection = new PeerToPeerConnection(mTorrentParser, mFileAnalyzer, mTrackerService, currentPeer, mOperationalPieces);
                final PeerToPeerInformation information = new PeerToPeerInformation(connection, peerTableData);

                mConnections.put(currentPeer, information);
                needed = true;
            }

        mConnectionNeeded = needed;
    }

    /**
     * Thread, that analyzes read bytes.
     * @author Boyarkin Nikita.
     */
    class PeerToPeerRunnable extends Thread {
        /** PeerToPeer information object. */
        private @NotNull final PeerToPeerInformation mInformation;

        /** Read bytes. */
        private @NotNull final byte[] mBytes;

        /** Java nio selection key. */
        private @NotNull final SelectionKey mKey;

        /**
         * Creates PeerToPeerRunnable object.
         * @param information - PeerToPeer information object.
         * @param key - Java nio selection key.
         * @param bytes - Read bytes.
         */
        public PeerToPeerRunnable(@NotNull final PeerToPeerInformation information,
                                  @NotNull final SelectionKey key, @NotNull final byte[] bytes) {
            mInformation = information;
            mKey = key;
            mBytes = bytes;

            this.setDaemon(false);
        }

        @Override
        public void run() {
            if(mInformation.channel != null) {
                try {
                    // Try to analyze read bytes.
                    mInformation.connection.analyzeMessage(mBytes, mInformation.channel);
                }
                catch(final PeerToPeerConnection.BitTorrentException | IOException | IllegalArgumentException exception) {
                    Log.e(mInformation.connection.getPeerInformation().toString() + " Impossible to analyze messages. Connection refused with peer with message: " + exception.getMessage());
                    finishPeer(mInformation);
                }
                catch(final InterruptedException exception) {
                    return;
                }
            }

            mKey.interestOps(SelectionKey.OP_READ);
        }
    }

    /**
     * Timer task, that decrement reconnect counter every second, if needed.
     * @author Boyarkin Nikita.
     */
    private class ReconnectTask extends TimerTask {
        @Override
        public void run() {
            boolean needed = false;

            for(final Map.Entry<Peer, PeerToPeerInformation> currentInformation: mConnections.entrySet()) {
                final PeerToPeerInformation information = currentInformation.getValue();

                // Decrement reconnect counter every second,, if needed.
                if(information.state == ConnectionState.DISCONNECTED && information.reconnect != null) {
                    if(information.reconnect != 0)
                        --information.reconnect;

                    if(information.reconnect == 0)
                        needed = true;
                }
            }

            mConnectionNeeded = needed;
        }
    }

    /**
     * Timer task, that measures speed for all peers.
     * @author Boyarkin Nikita.
     */
    private class SpeedMeasureTask extends TimerTask {
        @Override
        public void run() {
            if(mSpeedChangeable == null)
                return;

            long uploadingSpeed = 0, sharingSpeed = 0;

            for(final Map.Entry<Peer, PeerToPeerInformation> currentInformation: mConnections.entrySet()) {
                final PeerToPeerInformation information = currentInformation.getValue();

                if(information.state == ConnectionState.CONNECTED) {
                    // Pop count of bytes uploading and sharing at this period.
                    final long uploadingBytes = information.connection.popUploadingBytes();
                    final long sharingBytes = information.connection.popSharingBytes();

                    // Refresh table data.
                    information.peerTableData.setUploadingSpeed(Util.lengthToHumanReadableFormat(uploadingBytes * Byte.SIZE, true) + "ps");
                    information.peerTableData.setSharingSpeed(Util.lengthToHumanReadableFormat(sharingBytes * Byte.SIZE, true) + "ps");

                    uploadingSpeed += uploadingBytes;
                    sharingSpeed += sharingBytes;
                }
            }

            mSpeedChangeable.onSpeedChanged(uploadingSpeed, sharingSpeed);
        }
    }
}
