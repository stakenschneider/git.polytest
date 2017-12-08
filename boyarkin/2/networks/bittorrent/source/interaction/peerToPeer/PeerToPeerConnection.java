package interaction.peerToPeer;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import interaction.fileAnalyser.Bitfield;
import interaction.fileAnalyser.FileAnalyzer;
import interaction.peerToPeer.message.*;
import interaction.torrentParser.TorrentParser;
import interaction.trackerService.Peer;
import interaction.trackerService.TrackerService;
import javafx.util.Pair;
import org.apache.commons.lang3.tuple.Triple;
import util.Log;
import util.SHA1;
import util.Util;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * Class, that analyses messages from current peer, and sets special server/client states.
 * @author Boyarkin Nikita.
 */
public class PeerToPeerConnection {
    /** Transmittable buffer size (in bytes). */
    public static int BUFFER_SIZE = 0x20000;

    /** Maximum count of blocks, that can be requested. */
    private static final int MAX_COUNT_OF_BLOCKS_REQUESTED = 0x4;

    /** Minimum count of blocks, that can be requested. */
    private static final int MIN_COUNT_OF_BLOCKS_REQUESTED = 0x2;

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
    private @NotNull OperationalPieces mOperationalPieces;

    /** Uploading state. */
    private @NotNull PeerState mCurrentClientState;

    /** Sharing state. */
    private @NotNull PeerState mCurrentServerState;

    /** Transmittable buffer with BUFFER_SIZE length. */
    private @Nullable byte[] mContinuationData;

    /** Current buffer data length. */
    private @Nullable int mContinuationLength;

    /** Primary peer bitfield. */
    private @NotNull Bitfield mPrimaryBitfield;

    /** Foreign peer bitfield. */
    private @NotNull Bitfield mForeignBitfield;

    /**
     * Information about peer remote address.
     * @see Peer
     */
    private @NotNull Peer mPeer;

    /** Count of uploading bytes. */
    private @NotNull long mUploadingBytes;

    /** Count of sharing bytes. */
    private @NotNull long mSharingBytes;

    /**
     * Count of blocks currently requested.
     * MIN_COUNT_OF_BLOCKS_REQUESTED <= mCountOfBlocksRequested <= MAX_COUNT_OF_BLOCKS_REQUESTED
     */
    private @NotNull int mCountOfBlocksRequested;

    /**
     * Peer identifier.
     * @see interaction.trackerService.TrackerService
     */
    private @NotNull String mPeerId;

    /**
     * Sha-1 hash of 'info' dictionary.
     * @see interaction.torrentParser.TorrentParser
     */
    private @NotNull SHA1 mInfoHash;

    /**
     * Peer states.
     */
    private enum PeerState {
        INITIAL, HANDSHAKE, BITFIELD, NOT_INTERESTED, INTERESTED, UNCHOKE
    }

    /**
     * Creates PeerToPeerConnection options.
     * @param torrentParser - TorrentParser object.
     * @see TorrentParser
     * @param fileAnalyzer - FileAnalyzer object.
     * @see FileAnalyzer
     * @param trackerService - TrackerService object.
     * @see TrackerService
     * @param peer - Information about peer remote address.
     * @param operationalPieces - OperationalPieces object.
     */
    public PeerToPeerConnection(@NotNull final TorrentParser torrentParser, @NotNull final FileAnalyzer fileAnalyzer, @NotNull final TrackerService trackerService,
                                @NotNull final Peer peer, @NotNull final OperationalPieces operationalPieces) {
        mTorrentParser = torrentParser;
        mFileAnalyzer = fileAnalyzer;
        mTrackerService = trackerService;

        mPeer = peer;
        mOperationalPieces = operationalPieces;

        mPrimaryBitfield = mFileAnalyzer.getBitfield();
        mForeignBitfield = null;

        mUploadingBytes = 0;
        mSharingBytes = 0;

        mContinuationData = new byte[BUFFER_SIZE];
        mContinuationLength = 0;

        mCountOfBlocksRequested = 0;

        mPeerId = null;
        mInfoHash = null;

        mCurrentServerState = mCurrentClientState = PeerState.INITIAL;
    }

    /**
     * Parses messages from byte array, analyzes them and changes state, if needed.
     * @param message - Byte array for parsing.
     * @param channel - Peer socket channel.
     * @throws BitTorrentException BitTorrent logical exception.
     * @throws IOException If impossible to write message.
     * @throws InterruptedException If thread is interrupted.
     */
    public synchronized void analyzeMessage(@NotNull final byte[] message, @Nullable final SocketChannel channel) throws BitTorrentException, IOException, InterruptedException {
        // Log.d(channel.getRemoteAddress().toString() + " Try to analyze " + message.length + " bytes.");

        System.arraycopy(message, 0, mContinuationData, mContinuationLength, message.length);
        mContinuationLength += message.length;

        final Pair<List<BitTorrentMessage>, Integer> result = BitTorrentMessage.parseAll(mContinuationData, mContinuationLength);

        final List<BitTorrentMessage> messages = result.getKey();
        final int countOfContinuationBytes = result.getValue();

        mUploadingBytes += mContinuationLength - countOfContinuationBytes;

        // Log.d(channel.getRemoteAddress().toString() + " Resolved " + messages.size() + " messages from buffer with length " + mContinuationLength + ".");

        if(countOfContinuationBytes > 0) {
            // Shift buffer to the beginning.
            System.arraycopy(mContinuationData, mContinuationLength - countOfContinuationBytes, mContinuationData, 0, countOfContinuationBytes);
            mContinuationLength = countOfContinuationBytes;

            // Log.d(channel.getRemoteAddress().toString() + " Continuation data length " + mContinuationLength + " bytes.");
        }
        else
            mContinuationLength = 0;

        boolean changed = true;
        while(!Thread.currentThread().isInterrupted() && !messages.isEmpty() && changed)
            changed = analyzeMessage(messages, channel);
    }

    /**
     * Analyzes messages and changes state, if needed.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @throws BitTorrentException BitTorrent logical exception.
     * @throws IOException If impossible to write message.
     * @throws InterruptedException If thread is interrupted.
     */
    private boolean analyzeMessage(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) throws BitTorrentException, IOException, InterruptedException {
        if(mCurrentClientState == PeerState.INITIAL || mCurrentServerState == PeerState.INITIAL)
            throw new BitTorrentException(BitTorrentException.Code.INITIAL_STATE_NOT_EXPECTED);

        Util.checkInterrupted(null);
        boolean changed = analyzeHandshakeState(messages, channel);

        Util.checkInterrupted(null);
        changed = analyzeClientKeepAliveMessages(messages, channel) || changed;

        Util.checkInterrupted(null);
        changed = analyzeClientBitfieldState(messages, channel) || changed;

        Util.checkInterrupted(null);
        changed = analyzeClientInterestedState(messages, channel) || changed;

        Util.checkInterrupted(null);
        changed = analyzeClientUnchokeState(messages, channel) || changed;

        Util.checkInterrupted(null);
        changed = analyzeClientExtendedMessages(messages, channel) || changed;

        Util.checkInterrupted(null);
        changed = analyzeClientPortMessages(messages, channel) || changed;

        Util.checkInterrupted(null);
        changed = analyzeClientHaveMessages(messages, channel) || changed;

        Util.checkInterrupted(null);
        changed = analyzeServerBitfieldState(messages, channel) || changed;

        Util.checkInterrupted(null);
        changed = analyzeServerUnchokeState(messages, channel) || changed;

        return changed;
    }

    /**
     * Analyzes 'keep alive' messages and sends responses back to peer.
     * Removes all 'keep alive' messages from messages list.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @return Returns true if at least one 'keep alive' message is recognized.
     * @throws IOException If impossible to write message.
     */
    private boolean analyzeClientKeepAliveMessages(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) throws IOException {
        if(mCurrentClientState == PeerState.HANDSHAKE && mCurrentServerState == PeerState.HANDSHAKE)
            return false;

        if(messages.isEmpty())
            return false;

        boolean containsPort = false;

        Iterator<BitTorrentMessage> iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();
            if(currentMessage instanceof BitTorrentKeepAlive) {
                // Send 'keep alive' messages back to client.
                final BitTorrentKeepAlive keepAlive = new BitTorrentKeepAlive();
                channel.write(ByteBuffer.wrap(keepAlive.getBytes()));
                mSharingBytes += keepAlive.getLength();

                Log.d(channel.getRemoteAddress().toString() + " Keep alive sent.");
                iterator.remove();

                containsPort = true;
            }
        }

        return containsPort;
    }

    /**
     * Analyzes 'handshake' message and sends 'bitfield' back to peer.
     * Removes 'handshake' message from messages list.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @return Returns true if 'handshake' message is recognized.
     * @throws BitTorrentException BitTorrent logical exception.
     * @throws IOException If impossible to write message.
     */
    private boolean analyzeHandshakeState(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) throws BitTorrentException, IOException {
        if(mCurrentClientState != PeerState.HANDSHAKE || mCurrentServerState != PeerState.HANDSHAKE)
            return false;

        if(messages.isEmpty() || !(messages.get(0) instanceof BitTorrentHandshake))
            throw new BitTorrentException(BitTorrentException.Code.HANDSHAKE_MESSAGE_EXPECTED);

        final BitTorrentHandshake handshake = (BitTorrentHandshake) messages.remove(0);
        if(!handshake.getInfoHash().equals(mTorrentParser.getInfoHash()))
            throw new BitTorrentException(BitTorrentException.Code.HANDSHAKE_WRONG_HASH);

        mPeerId = handshake.getStringPeerId();
        mInfoHash = handshake.getInfoHash();

        // TODO
        /*// Send 'bitfield' message back to client.
        final BitTorrentExtended extended = new BitTorrentExtended();
        channel.write(ByteBuffer.wrap(extended.getBytes()));
        mSharingBytes += extended.getLength();*/

        // Send 'bitfield' message back to client.
        final BitTorrentBitfield bitfield = new BitTorrentBitfield(mPrimaryBitfield.getBytes());
        channel.write(ByteBuffer.wrap(bitfield.getBytes()));
        mSharingBytes += bitfield.getLength();

        mCurrentServerState = mCurrentClientState = PeerState.BITFIELD;
        Log.d(channel.getRemoteAddress().toString() + " Client and server states sets into BITFIELD.");
        return true;
    }

    /**
     * Analyzes 'interested' and 'not interested' messages and sends 'unchoke' back to peer, if needed.
     * Removes 'interested' and 'not interested' messages from messages list.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @return Returns true if 'interested' or 'not interested' messages are recognized.
     * @throws BitTorrentException BitTorrent logical exception.
     * @throws IOException If impossible to write message.
     */
    private boolean analyzeServerBitfieldState(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) throws BitTorrentException, IOException {
        if(mCurrentServerState != PeerState.BITFIELD && mCurrentServerState != PeerState.NOT_INTERESTED)
            return false;

        if(messages.isEmpty())
            return false;

        Iterator<BitTorrentMessage> iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();
            if(currentMessage instanceof BitTorrentInterested) {
                // Send 'port', 'extended' and 'unchoke' messages back to client.
                final List<BitTorrentMessage> transfer = new ArrayList<>();
                transfer.add(new BitTorrentPort(channel.socket().getLocalPort()));
                transfer.add(new BitTorrentExtended((byte) 1));
                transfer.add(new BitTorrentUnchoke());
                final byte[] bytes = BitTorrentMessage.constructAll(transfer);
                channel.write(ByteBuffer.wrap(bytes));
                mSharingBytes += bytes.length;

                mCurrentServerState = PeerState.UNCHOKE;
                Log.d(channel.getRemoteAddress().toString() + " Server state sets into UNCHOKE.");
                iterator.remove();
                return true;
            }
            else if(currentMessage instanceof BitTorrentNotInterested) {
                mCurrentServerState = PeerState.NOT_INTERESTED;
                Log.d(channel.getRemoteAddress().toString() + " Server state sets into NOT_INTERESTED.");
                iterator.remove();
                return true;
            }
        }

        return false;
    }

    /**
     * Analyzes 'bitfield' and 'have' messages and sends 'interested' or 'not interested' back to peer, if needed.
     * Removes 'bitfield' and 'have' messages from messages list.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @return Returns true if 'bitfield' message is recognized.
     * @throws BitTorrentException BitTorrent logical exception.
     * @throws IOException If impossible to write message.
     */
    private boolean analyzeClientBitfieldState(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) throws BitTorrentException, IOException {
        if(mCurrentClientState != PeerState.BITFIELD)
            return false;

        if(messages.isEmpty())
            return false;

        boolean containsBitfield = false;

        Iterator<BitTorrentMessage> iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();
            if(currentMessage instanceof BitTorrentBitfield) {
                mForeignBitfield = new Bitfield(((BitTorrentBitfield) currentMessage).getBitfieldBytes());

                // Length of primary and foreign bitfield must be the same.
                if(mForeignBitfield.getLength() != mPrimaryBitfield.getLength())
                    throw new BitTorrentException(BitTorrentException.Code.BITFIELD_WRONG_LENGTH);

                containsBitfield = true;
                iterator.remove();
                break;
            }
        }

        if(!containsBitfield)
            return false;

        iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();
            if(currentMessage instanceof BitTorrentHave) {
                final int pieceId = ((BitTorrentHave) currentMessage).getPieceId();

                // Piece identifier must be lesser, than pieces count.
                if(pieceId >= mTorrentParser.getPiecesCount())
                    throw new BitTorrentException(BitTorrentException.Code.HAVE_MESSAGE_WRONG_PIECE_ID);

                mForeignBitfield.setPiece(pieceId);
                iterator.remove();
            }
        }

        final List<Integer> pieces = mOperationalPieces.getAllPieces(mForeignBitfield);
        if(pieces.isEmpty()) {
            // Send 'not interested' message back to client.
            final BitTorrentNotInterested notInterested = new BitTorrentNotInterested();
            channel.write(ByteBuffer.wrap(notInterested.getBytes()));
            mSharingBytes += notInterested.getLength();

            mCurrentClientState = PeerState.NOT_INTERESTED;
            Log.d(channel.getRemoteAddress().toString() + " Client state sets into NOT_INTERESTED.");
        }
        else
            setInterestedState(channel);

        return true;
    }

    /**
     * Analyzes 'unchoke' message and sends 'request' messages back to peer, if needed.
     * Removes 'unchoke' message from messages list.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @return Returns true if 'unchoke' message is recognized.
     * @throws BitTorrentException BitTorrent logical exception.
     * @throws IOException If impossible to write message.
     */
    private boolean analyzeClientInterestedState(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) throws BitTorrentException, IOException {
        if(mCurrentClientState != PeerState.INTERESTED)
            return false;

        if(messages.isEmpty())
            return false;

        final Iterator<BitTorrentMessage> iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();
            if(currentMessage instanceof BitTorrentUnchoke) {
                // Make requests to peer, if needed.
                makeClientRequestsIfNeeded(channel);
                iterator.remove();
                return true;
            }
        }

        return false;
    }

    /**
     * Analyzes 'choke' or 'request' messages and sends 'piece' or 'cancel' messages back to peer, if needed.
     * Removes 'choke' or 'request' messages from messages list.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @return Returns true if 'choke' or 'request' messages are recognized.
     * @throws BitTorrentException BitTorrent logical exception.
     * @throws IOException If impossible to write message.
     */
    private boolean analyzeServerUnchokeState(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) throws BitTorrentException, IOException {
        if(mCurrentServerState != PeerState.UNCHOKE)
            return false;

        if(messages.isEmpty())
            return false;

        boolean containsRequest = false;

        Iterator<BitTorrentMessage> iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();
            if(currentMessage instanceof BitTorrentChoke) {
                mCurrentServerState = PeerState.NOT_INTERESTED;
                Log.d(channel.getRemoteAddress().toString() + " Server state sets into NOT_INTERESTED.");

                iterator.remove();
                return true;
            }
            else if(currentMessage instanceof BitTorrentRequest) {
                final BitTorrentRequest request = (BitTorrentRequest) currentMessage;

                try {
                    final byte[] bytes = mFileAnalyzer.getPieceBlock(request.getPieceId(), request.getBeginOffset(), request.getLength());

                    // Send 'piece' message back to peer.
                    final BitTorrentPiece piece = new BitTorrentPiece(request.getPieceId(), request.getBeginOffset(), bytes);
                    channel.write(ByteBuffer.wrap(piece.getBytes()));
                    mSharingBytes += piece.getLength();
                }
                catch(final IllegalArgumentException exception) {
                    // Send 'cancel' message back to peer.
                    final BitTorrentCancel cancel = new BitTorrentCancel(request.getPieceId(), request.getBeginOffset(), request.getLength());
                    channel.write(ByteBuffer.wrap(cancel.getBytes()));
                    mSharingBytes += cancel.getLength();
                }

                containsRequest = true;
                iterator.remove();
            }
        }

        return containsRequest;
    }

    /**
     * Analyzes 'piece' or 'cancel' messages and sends 'request' messages back to peer, if needed.
     * Removes 'piece' or 'cancel' messages from messages list.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @return Returns true if 'piece' or 'cancel' messages are recognized.
     * @throws BitTorrentException BitTorrent logical exception.
     * @throws IOException If impossible to write message.
     */
    private boolean analyzeClientUnchokeState(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) throws BitTorrentException, IOException {
        if(mCurrentClientState != PeerState.UNCHOKE)
            return false;

        if(messages.isEmpty())
            return false;

        boolean containsRequestOrCancel = false;

        final Iterator<BitTorrentMessage> iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();

            if(currentMessage instanceof BitTorrentPiece) {
                final BitTorrentPiece piece = (BitTorrentPiece) currentMessage;
                Log.d(channel.getRemoteAddress().toString() + " Accepted part #" + piece.getPieceId() + ":0x" + Integer.toString(piece.getBeginOffset(), 16));

                try {
                    // Try to set block for piece.
                    mOperationalPieces.setBlockForPiece(piece.getPieceId(), piece.getBeginOffset(), piece.getPieceBlock());
                }
                catch(final IndexOutOfBoundsException | IllegalArgumentException exception) {
                    Log.e(channel.getRemoteAddress().toString() + " Impossible to set piece #" + piece.getPieceId() + ":0x" + Integer.toString(piece.getBeginOffset(), 16));
                }

                if(mCountOfBlocksRequested != 0)
                    --mCountOfBlocksRequested;

                containsRequestOrCancel = true;
                iterator.remove();
            }
            else if(currentMessage instanceof BitTorrentCancel) {
                if(mCountOfBlocksRequested != 0)
                    --mCountOfBlocksRequested;

                containsRequestOrCancel = true;
                iterator.remove();
            }
        }

        if(!containsRequestOrCancel)
            return false;

        if(mCountOfBlocksRequested <= MIN_COUNT_OF_BLOCKS_REQUESTED)
            makeClientRequestsIfNeeded(channel);

        return true;
    }

    /**
     * Analyzes 'extended' messages.
     * Removes 'extended' messages from messages list.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @return Returns true if at least one 'extended' message is recognized.
     */
    private boolean analyzeClientExtendedMessages(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) {
        if(mCurrentClientState == PeerState.HANDSHAKE)
            return false;

        if(messages.isEmpty())
            return false;

        // TODO

        boolean containsExtended = false;

        final Iterator<BitTorrentMessage> iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();
            if(currentMessage instanceof BitTorrentExtended) {
                containsExtended = true;
                iterator.remove();
            }
        }

        return containsExtended;
    }

    /**
     * Analyzes 'port' messages.
     * Removes 'port' messages from messages list.
     * @param messages - Messages list to analyze.
     * @return Returns true if at least one 'port' message is recognized.
     */
    private boolean analyzeClientPortMessages(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) {
        if(mCurrentClientState == PeerState.HANDSHAKE)
            return false;

        if(messages.isEmpty())
            return false;

        // TODO

        boolean containsPort = false;

        final Iterator<BitTorrentMessage> iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();
            if(currentMessage instanceof BitTorrentPort) {
                containsPort = true;
                iterator.remove();
            }
        }

        return containsPort;
    }

    /**
     * Analyzes 'have' messages, sets blocks for that pieces. Sends 'interested' message back to peer, if needed.
     * Removes 'have' messages from messages list.
     * @param messages - Messages list to analyze.
     * @param channel - Peer socket channel.
     * @return Returns true if at least one 'have' message is recognized.
     * @throws BitTorrentException BitTorrent logical exception.
     * @throws IOException If impossible to write message.
     */
    private boolean analyzeClientHaveMessages(@NotNull final List<BitTorrentMessage> messages, @Nullable final SocketChannel channel) throws BitTorrentException, IOException {
        if(mCurrentClientState == PeerState.HANDSHAKE || mCurrentClientState == PeerState.BITFIELD)
            return false;

        if(messages.isEmpty())
            return false;

        boolean containsHave = false;

        final Iterator<BitTorrentMessage> iterator = messages.iterator();
        while(iterator.hasNext()) {
            final BitTorrentMessage currentMessage = iterator.next();
            if(currentMessage instanceof BitTorrentHave) {
                final int pieceId = ((BitTorrentHave) currentMessage).getPieceId();
                if(pieceId >= mTorrentParser.getPiecesCount())
                    throw new BitTorrentException(BitTorrentException.Code.HAVE_MESSAGE_WRONG_PIECE_ID);

                // Set block for current piece.
                mForeignBitfield.setPiece(pieceId);
                iterator.remove();

                containsHave = true;
            }
        }

        if(!containsHave)
            return false;

        // Send 'interested' message back to peer, if needed.
        if(mCurrentClientState == PeerState.NOT_INTERESTED) {
            final List<Integer> pieces = mOperationalPieces.getAllPieces(mForeignBitfield);

            if(!pieces.isEmpty())
                setInterestedState(channel);
        }

        return true;
    }

    /**
     * Sends 'interested', 'choke', 'not interested' or 'request' messages back to peer depending on current state.
     * @param channel - Peer socket channel.
     * @throws IOException If impossible to write message.
     */
    private void makeClientRequestsIfNeeded(@Nullable final SocketChannel channel) throws IOException {
        if(mCurrentClientState != PeerState.NOT_INTERESTED && mCurrentClientState != PeerState.INTERESTED && mCurrentClientState != PeerState.UNCHOKE)
            return;

        final List<Triple<Integer, Long, Long>> offsets = mOperationalPieces.getAllAvailableOffsets(mForeignBitfield, MAX_COUNT_OF_BLOCKS_REQUESTED - mCountOfBlocksRequested);
        if(offsets.isEmpty()) {
            if(mCurrentClientState == PeerState.NOT_INTERESTED)
                return;

            // Send 'choke' message back to peer.
            final BitTorrentChoke choke = new BitTorrentChoke();
            channel.write(ByteBuffer.wrap(choke.getBytes()));
            mSharingBytes += choke.getLength();

            // Send 'not interested' message back to peer.
            final BitTorrentNotInterested notInterested = new BitTorrentNotInterested();
            channel.write(ByteBuffer.wrap(notInterested.getBytes()));
            mSharingBytes += notInterested.getLength();

            mCountOfBlocksRequested = 0;
            mCurrentClientState = PeerState.NOT_INTERESTED;
            Log.d(channel.getRemoteAddress().toString() + " Client state sets into NOT_INTERESTED.");
        }
        else {
            if(mCurrentClientState == PeerState.NOT_INTERESTED) {
                setInterestedState(channel);
                return;
            }

            final List<BitTorrentMessage> transfer = new ArrayList<>();
            final int length = offsets.size();

            //StringBuilder log = new StringBuilder();
            //log.append(" ").append(length).append(" requests sent for pieces: ");

            // Construct 'request' messages.
            for(final Triple<Integer, Long, Long> currentOffset: offsets) {
                final int pieceId = currentOffset.getLeft();
                final long pieceOffset = currentOffset.getMiddle();
                final long pieceLength = currentOffset.getRight();

                transfer.add(new BitTorrentRequest(pieceId, (int) pieceOffset, (int) pieceLength));
                //log.append(", #").append(pieceId).append(":0x").append(Integer.toString((int) pieceOffset, 16));
            }

            //Log.d(channel.getRemoteAddress().toString() + log.append(".").toString());

            mCountOfBlocksRequested += length;

            // Send all 'request' messages back to peer.
            final byte[] bytes = BitTorrentMessage.constructAll(transfer);
            channel.write(ByteBuffer.wrap(bytes));
            mSharingBytes += bytes.length;

            if(mCurrentClientState != PeerState.UNCHOKE) {
                mCurrentClientState = PeerState.UNCHOKE;
                Log.d(channel.getRemoteAddress().toString() + " Client state sets into UNCHOKE.");
            }
        }
    }

    /**
     * Sends 'port', 'extended' and 'interested' messages to peer.
     * @param channel - Peer socket channel.
     * @throws IOException If impossible to write message.
     */
    private void setInterestedState(@Nullable final SocketChannel channel) throws IOException {
        // Send 'port', 'extended' and 'interested' messages to peer.
        final List<BitTorrentMessage> transfer = new ArrayList<>();
        transfer.add(new BitTorrentPort(channel.socket().getLocalPort()));
        transfer.add(new BitTorrentExtended((byte) 0));
        transfer.add(new BitTorrentInterested());

        final byte[] bytes = BitTorrentMessage.constructAll(transfer);
        channel.write(ByteBuffer.wrap(bytes));
        mSharingBytes += bytes.length;

        mCurrentClientState = PeerState.INTERESTED;
        Log.d(channel.getRemoteAddress().toString() + " Client state sets into INTERESTED.");
    }

    /**
     * Sends 'handshake' message to peer.
     * @param channel - Peer socket channel.
     * @throws IOException If impossible to write message.
     */
    public void handshakeNotify(@Nullable final SocketChannel channel) throws BitTorrentException, IOException {
        if(mCurrentClientState != PeerState.INITIAL || mCurrentServerState != PeerState.INITIAL)
            throw new BitTorrentException(BitTorrentException.Code.INITIAL_STATE_EXPECTED);

        // Send 'handshake' message to peer.
        final BitTorrentHandshake handshake = new BitTorrentHandshake(mTorrentParser.getInfoHash(), mTrackerService.getStringPeerId());
        channel.write(ByteBuffer.wrap(handshake.getBytes()));
        mSharingBytes += handshake.getLength();

        mCurrentServerState = mCurrentClientState = PeerState.HANDSHAKE;
        Log.d(channel.getRemoteAddress().toString() + " Client and server states sets into HANDSHAKE.");
    }

    /**
     * Sends 'interested', 'choke', 'not interested' or 'request' messages back to peer depending on current state.
     * @param channel - Peer socket channel.
     * @throws IOException If impossible to write message.
     */
    public void interestedNotify(@Nullable final SocketChannel channel) throws IOException {
        if(mCurrentClientState != PeerState.NOT_INTERESTED && mCurrentClientState != PeerState.UNCHOKE)
            return;

        makeClientRequestsIfNeeded(channel);
    }

    /**
     * Sends 'have' message to peer.
     * @param pieceId - Piece identifier.
     * @param channel - Peer socket channel.
     * @throws IOException If impossible to write message.
     */
    public void haveNotify(@NotNull final int pieceId, @Nullable final SocketChannel channel) throws IOException {
        if(mCurrentServerState == PeerState.INITIAL || mCurrentServerState == PeerState.HANDSHAKE)
            return;

        // Send 'have' message to peer.
        final BitTorrentHave have = new BitTorrentHave(pieceId);
        channel.write(ByteBuffer.wrap(have.getBytes()));
        mSharingBytes += have.getLength();
    }

    /**
     * Reset all parameters for current peer.
     */
    public synchronized void refreshParameters() {
        mForeignBitfield = null;
        mContinuationLength = 0;
        mCountOfBlocksRequested = 0;
        mUploadingBytes = 0;
        mSharingBytes = 0;
        mPeerId = null;
        mInfoHash = null;
        mCurrentServerState = mCurrentClientState = PeerState.INITIAL;
    }

    /**
     * Sends 'choke' and 'not interested' messages to peer depending on current state.
     * @param channel - Peer socket channel.
     * @throws IOException If impossible to write message.
     */
    public void finish(@Nullable final SocketChannel channel) throws IOException {
        if(mCurrentClientState == PeerState.UNCHOKE) {

            // Send 'choke' message to peer.
            final BitTorrentChoke choke = new BitTorrentChoke();
            channel.write(ByteBuffer.wrap(choke.getBytes()));

            // Send 'not interested' message to peer.
            final BitTorrentNotInterested notInterested = new BitTorrentNotInterested();
            channel.write(ByteBuffer.wrap(notInterested.getBytes()));
        }
    }

    /**
     * @return Returns information about peer remote address.
     */
    public @NotNull Peer getPeerInformation() {
        return mPeer;
    }

    /**
     * Returns count of uploading bytes, and reset it to zero.
     * @return Returns count of uploading bytes.
     */
    public @NotNull long popUploadingBytes() {
        long temp = mUploadingBytes;
        mUploadingBytes = 0;
        return temp;
    }

    /**
     * Returns count of sharing bytes, and reset it to zero.
     * @return Returns count of sharing bytes.
     */
    public @NotNull long popSharingBytes() {
        long temp = mSharingBytes;
        mSharingBytes = 0;
        return temp;
    }

    /**
     * BitTorrent exception class.
     * @author Boyarkin Nikita.
     */
    public static class BitTorrentException extends Exception {
        /**
         * All types of exception codes.
         */
        enum Code {
            INITIAL_STATE_NOT_EXPECTED,
            INITIAL_STATE_EXPECTED,
            HANDSHAKE_MESSAGE_EXPECTED,
            HANDSHAKE_WRONG_HASH,
            BITFIELD_WRONG_LENGTH,
            HAVE_MESSAGE_WRONG_PIECE_ID
        }

        BitTorrentException(@NotNull final PeerToPeerConnection.BitTorrentException.Code code) {
            super(getMessageByCode(code));
        }

        BitTorrentException(@NotNull final String message) {
            super(message);
        }

        BitTorrentException(@NotNull final Throwable cause) {
            super(cause);
        }

        BitTorrentException(@NotNull final String message, @NotNull final Throwable cause) {
            super(message, cause);
        }

        /**
         * @param code - Code of exception
         * @return Returns string description of exception by code.
         */
        private static @NotNull String getMessageByCode(@NotNull final PeerToPeerConnection.BitTorrentException.Code code) {
            switch(code) {
                case INITIAL_STATE_NOT_EXPECTED: return "INITIAL state not expected.";
                case INITIAL_STATE_EXPECTED: return "INITIAL state expected.";
                case HANDSHAKE_MESSAGE_EXPECTED: return "Handshake message expected.";
                case HANDSHAKE_WRONG_HASH: return "Wrong sha-1 hash of 'info' dictionary.";
                case BITFIELD_WRONG_LENGTH: return "Bitfield message has wrong length.";
                case HAVE_MESSAGE_WRONG_PIECE_ID: return "Have message has wrong piece identifier.";
                default: return "Unknown exception.";
            }
        }
    }
}
