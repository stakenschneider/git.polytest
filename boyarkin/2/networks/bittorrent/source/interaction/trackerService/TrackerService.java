package interaction.trackerService;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import gui.TrackerTableData;
import interaction.fileAnalyser.FileAnalyzer;
import interaction.torrentParser.TorrentParser;
import javafx.collections.ObservableList;
import util.Constants;
import util.Log;
import util.SHA1;
import util.Util;

import javax.xml.bind.DatatypeConverter;
import java.io.IOException;
import java.net.URL;
import java.security.NoSuchAlgorithmException;
import java.util.*;

/**
 * Contains information about all trackers.
 * @author Boyarkin Nikita.
 */
public class TrackerService implements OnePeerChangeable {
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

    /** Callback, that calls when one of all peers changed. */
    private @NotNull final PeersCountChangeable mPeersCountChangeable;

    /** Callback, that calls when set of peers changes. */
    private @Nullable OnePeerChangeable mOnePeerChangeable;

    /** Map, that contains information about all trackers (not updatable). */
    private @NotNull Map<Integer, TrackerInfo> mTrackers;

    /** Map, that contains information about all peers (updatable). */
    private @NotNull final Set<Peer> mPeers;

    /** Last 12 bytes of peer identifier. */
    private @NotNull byte[] mBytesPeerId;

    /** 20 bytes of peer identifier in hexadecimal string format. */
    private @NotNull String mStringPeerId;

    /** 4 bytes of key in hexadecimal string format. */
    private @NotNull String mKey;

    /**
     * Creates TrackerService object.
     * @param trackersList - TrackerTableView list.
     * @see gui.Controller
     * @param peersCountChangeable - PeersCountChangeable callback (can be null).
     * @param torrentParser - TorrentParser object.
     * @see TorrentParser
     * @param fileAnalyzer - FileAnalyzer object.
     * @see FileAnalyzer
     * @param peerPort - Reserved client port.
     * @throws InterruptedException If thread is interrupted.
     * @throws TrackerException Tracker exception (usually caused by IOException or SecurityException).
     */
    public TrackerService(@NotNull final ObservableList<TrackerTableData> trackersList, @Nullable final PeersCountChangeable peersCountChangeable,
                          @NotNull final TorrentParser torrentParser, @NotNull final FileAnalyzer fileAnalyzer, @NotNull final int peerPort) throws InterruptedException, TrackerException {
        mTorrentParser = torrentParser;
        mFileAnalyzer = fileAnalyzer;
        mPeersCountChangeable = peersCountChangeable;
        mPeers = new HashSet<>();

        // Create unique set of trackers.
        final Set<URL> trackers = new HashSet<>();
        trackers.add(torrentParser.getAnnounce());
        if(torrentParser.getAnnounceSet() != null)
            trackers.addAll(torrentParser.getAnnounceSet());

        // Generate authentication parameters.
        setPeerIdAndKey();

        // Try to create TrackerInfo object for every tracker into unique set.
        int trackerId = 0;
        mTrackers = new HashMap<>();
        for(final URL currentTracker: trackers) {
            final TrackerInfo trackerInfo = new TrackerInfo(currentTracker, mTorrentParser, mFileAnalyzer, mBytesPeerId, mKey, peerPort);
            mTrackers.put(trackerId++, trackerInfo);

            try {
                trackerInfo.startTracker(trackersList, this);
            }
            catch(final IOException exception) {
                Log.e("Impossible start TrackerInfo object for tracker '" + currentTracker.toString() + "', cuz tracker is unreachable.");
            }
        }

        mTrackers = Collections.unmodifiableMap(mTrackers);
        Util.checkInterrupted(this::stopTimers);
    }

    /**
     * Generates authentication parameters - peer identifier (20 bytes) and key (4 bytes).
     * Peer identifier contains 8 bytes of ASCII identifier and 12 bytes of bytes identifier.
     * Generation of this parameters based on Constants.TRACKER_PEER_ID_PREFIX constant.
     * @throws TrackerException Tracker exception (usually caused by IOException or SecurityException).
     */
    private void setPeerIdAndKey() throws TrackerException {
        try {
            // Peer identifier generation based on sha-1 hash of Constants.TRACKER_PEER_ID_PREFIX.
            final SHA1 hash = SHA1.generateSHA1(Constants.TRACKER_PEER_ID_PREFIX.getBytes());
            mBytesPeerId = Arrays.copyOfRange(hash.toBytes(), 0, 12);
            mStringPeerId = DatatypeConverter.printHexBinary(Constants.TRACKER_PEER_ID_PREFIX.getBytes()).toLowerCase() + hash.toString().substring(0, 24).toLowerCase();
        }
        catch(final NoSuchAlgorithmException exception) {
            throw new TrackerException(TrackerException.Code.IMPOSSIBLE_TO_GENERATE_PEER_ID);
        }

        try {
            // Key generation based on sha-1 hash of peer identifier last 12 bytes.
            final SHA1 hash = SHA1.generateSHA1(mBytesPeerId);
            mKey = hash.toString().substring(0, 8);
        }
        catch(final NoSuchAlgorithmException exception) {
            throw new TrackerException(TrackerException.Code.IMPOSSIBLE_TO_GENERATE_KEY);
        }

        Log.i("Peer identifier '" + mStringPeerId + "', key: '" + mKey + "'.");
    }

    /**
     * Stops all timer tasks for all trackers.
     */
    public synchronized void stopTimers() {
        for(final Map.Entry<Integer, TrackerInfo> currentTracker: mTrackers.entrySet())
            currentTracker.getValue().stopTimers();
    }

    /**
     * Sets callback on change of peer.
     * @param onePeerChangeable - OnePeerChangeable callback.
     */
    public void setOnOnePeerChanged(@NotNull final OnePeerChangeable onePeerChangeable) {
        mOnePeerChangeable = onePeerChangeable;
    }

    /**
     * Callback function.
     * @param trackerInfo - The TrackerInfo object on which this function was called.
     * @param peers - Unique set of peers into TrackerInfo object.
     */
    @Override
    public void onOnePeerChanged(@NotNull final TrackerInfo trackerInfo, @NotNull final Set<Peer> peers) {
        for(final Peer peer: peers)
            if(!mPeers.contains(peer))
                mPeers.add(peer);

        if(mPeersCountChangeable != null)
            mPeersCountChangeable.onPeersCountChanged(mPeers.size());

        if(mOnePeerChangeable != null)
            mOnePeerChangeable.onOnePeerChanged(trackerInfo, peers);
    }

    /**
     * Getters for some class fields.
     */

    public @NotNull String getStringPeerId() {
        return mStringPeerId;
    }

    public @NotNull Set<Peer> getPeerSet() { return mPeers; }

    /**
     * Tracker exception class.
     * @author Boyarkin Nikita.
     */
    public static class TrackerException extends Exception {
        /**
         * All types of exception codes.
         */
        enum Code {
            IMPOSSIBLE_TO_GENERATE_PEER_ID,
            IMPOSSIBLE_TO_GENERATE_KEY
        }

        TrackerException(@NotNull final TrackerService.TrackerException.Code code) {
            super(getMessageByCode(code));
        }

        TrackerException(@NotNull final String message) {
            super(message);
        }

        TrackerException(@NotNull final Throwable cause) {
            super(cause);
        }

        TrackerException(@NotNull final String message, @NotNull final Throwable cause) {
            super(message, cause);
        }

        /**
         * @param code - Code of exception
         * @return Returns string description of exception by code.
         */
        private static @NotNull String getMessageByCode(@NotNull final TrackerService.TrackerException.Code code) {
            switch(code) {
                case IMPOSSIBLE_TO_GENERATE_PEER_ID: return "Impossible to generate peer identifier.";
                case IMPOSSIBLE_TO_GENERATE_KEY: return "Impossible to generate key.";
                default: return "Unknown exception.";
            }
        }
    }
}
