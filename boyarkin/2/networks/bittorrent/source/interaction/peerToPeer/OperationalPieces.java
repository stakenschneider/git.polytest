package interaction.peerToPeer;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import interaction.fileAnalyser.Bitfield;
import interaction.fileAnalyser.FileAnalyzer;
import interaction.torrentParser.TorrentParser;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.lang3.tuple.ImmutableTriple;
import org.apache.commons.lang3.tuple.Triple;
import util.Log;

import java.io.IOException;
import java.security.NoSuchAlgorithmException;
import java.util.*;

/**
 * Class, that allocates COUNT_OF_OPERATIONAL_PIECES pieces for sharing or uploading.
 * @see OperationalPieces#COUNT_OF_OPERATIONAL_PIECES
 * Every piece divides into blocks with length BLOCK_LENGTH.
 * @see OperationalPieces#BLOCK_LENGTH
 * If all blocks of piece are completed, class tries to save that piece into file and allocates new piece for sharing or uploading.
 * @see FileAnalyzer#setPiece(int, byte[])
 * @author Boyarkin Nikita.
 */
public class OperationalPieces implements OperationalPieceAllocatable {
    /** Count of operational pieces (every piece require a lot of memory). */
    public static final int COUNT_OF_OPERATIONAL_PIECES = 5;

    /** Length of single block. */
    public static final int BLOCK_LENGTH = 0x4000;

    /** Timeout on request sending for every block. */
    public static final int EXTRADITION_TIME = 1 * 100;

    /** FileAnalyzer object. */
    private @NotNull final FileAnalyzer mFileAnalyzer;

    /** TorrentParser object. */
    private @NotNull final TorrentParser mTorrentParser;

    /** Callback, that calls when new piece is allocated. */
    private @Nullable final OperationalPieceAllocatable mCallback;

    /**
     * Default count of blocks (for all pieces, besides last).
     * mDefaultBlocksCount = (int) (pieceLength / BLOCK_LENGTH)
     */
    private @NotNull final int mDefaultBlocksCount;

    /**
     * Map, that contains all blocks for all operational pieces.
     * Size of map always <= COUNT_OF_OPERATIONAL_PIECES.
     */
    private @NotNull final Map<Integer, List<byte[]>> mPiecesBlocks;

    /**
     * Map, that contains all timeouts on request sending for all operational pieces.
     * Size of map always <= COUNT_OF_OPERATIONAL_PIECES.
     */
    private @NotNull final Map<Integer, List<Long>> mPiecesExtraditions;

    /**
     * Map, that contains counts of completed blocks for all operational pieces.
     * Size of map always <= COUNT_OF_OPERATIONAL_PIECES.
     */
    private @NotNull final Map<Integer, Integer> mCompletesBlocksCount;

    /**
     * Creates OperationalPieces object.
     * @param fileAnalyzer - FileAnalyzer object.
     * @see FileAnalyzer
     * @param torrentParser - TorrentParser object.
     * @see TorrentParser
     * @param callback - OperationalPieceAllocatable callback (can be null).
     */
    public OperationalPieces(@NotNull final FileAnalyzer fileAnalyzer, @NotNull final TorrentParser torrentParser, @Nullable final OperationalPieceAllocatable callback) {
        mFileAnalyzer = fileAnalyzer;
        mTorrentParser = torrentParser;

        mDefaultBlocksCount = (int) (mTorrentParser.getPieceLength() / BLOCK_LENGTH);

        mCallback = callback;

        mPiecesBlocks = new HashMap<>();
        mPiecesExtraditions = new HashMap<>();
        mCompletesBlocksCount = new HashMap<>();

        allocatePieces();
    }

    /**
     * Allocates new pieces, if needed, and calls callback function, on success.
     */
    private synchronized void allocatePieces() {
        if(mPiecesBlocks.size() >= COUNT_OF_OPERATIONAL_PIECES)
            return;

        // Try to get some non existing pieces.
        final List<Integer> pieceIdList = mFileAnalyzer.getNextNonexistingPieceIdList(COUNT_OF_OPERATIONAL_PIECES);
        if(pieceIdList.isEmpty())
            return;

        for(final Integer currentPieceId: pieceIdList) {
            if(mPiecesBlocks.containsKey(currentPieceId))
                continue;

            // Count of blocks for current piece.
            final int length = (int) Math.ceil(((float) mFileAnalyzer.getLengthOfPiece(currentPieceId)) / BLOCK_LENGTH);

            // Initialize all blocks with null.
            final List<byte[]> initialBlocks = new ArrayList<>(length);
            for(int index = 0; index < length; ++index)
                initialBlocks.add(null);

            mPiecesBlocks.put(currentPieceId, initialBlocks);

            // Initialize all timeouts on request sending with null.
            final List<Long> initialExtraditions = new ArrayList<>(length);
            for(int index = 0; index < length; ++index)
                initialExtraditions.add(null);

            mPiecesExtraditions.put(currentPieceId, initialExtraditions);

            // Count of completed blocks is zero.
            mCompletesBlocksCount.put(currentPieceId, 0);

            // Call callback function.
            onOperationPieceAllocated(currentPieceId);

            // Size of map always <= COUNT_OF_OPERATIONAL_PIECES.
            if(mPiecesBlocks.size() >= COUNT_OF_OPERATIONAL_PIECES)
                return;
        }
    }

    /**
     * @param pieceId - Piece identifier.
     * @return Returns true, if all blocks is completed.
     */
    private synchronized boolean isFinished(@NotNull final int pieceId) {
        final List<byte[]> blocks = mPiecesBlocks.get(pieceId);
        if(blocks == null)
            throw new IndexOutOfBoundsException();

        for(final byte[] currentBlock: blocks)
            if(currentBlock == null)
                return false;

        return true;
    }

    /**
     * Tries to save piece into file if all blocks are finished.
     * @param pieceId - Piece identifier.
     * @throws IndexOutOfBoundsException If piece identifier isn't contains into map.
     * @throws IllegalStateException If one of blocks is null.
     */
    private synchronized void setPiece(@NotNull final int pieceId) throws IndexOutOfBoundsException, IllegalStateException {
        byte[] result = null;

        final List<byte[]> blocks = mPiecesBlocks.get(pieceId);
        if(blocks == null)
            throw new IndexOutOfBoundsException();

        for(final byte[] currentBlock: blocks) {
            if(currentBlock == null)
                throw new IllegalStateException();

            // Unites all blocks into one piece.
            result = ArrayUtils.addAll(result, currentBlock);
        }

        try {
            // Try to save piece into file.
            mFileAnalyzer.setPiece(pieceId, result);
        }
        catch(final NoSuchAlgorithmException | IOException | RuntimeException exception) {
            Log.e("Impossible to save piece #" + pieceId + "into file.");
        }
        finally {
            mPiecesBlocks.remove(pieceId);
            mPiecesExtraditions.remove(pieceId);
            mCompletesBlocksCount.remove(pieceId);
            allocatePieces();
        }
    }

    /**
     * Tries to set block for piece.
     * @param pieceId - Piece identifier.
     * @param offset - Offset from beginning of the piece.
     * @param bytes - Block for piece.
     */
    public synchronized void setBlockForPiece(@NotNull final int pieceId, @NotNull final long offset, @NotNull final byte[] bytes) {
        final List<byte[]> blocks = mPiecesBlocks.get(pieceId);
        if(blocks == null)
            throw new IndexOutOfBoundsException();

        if(offset % BLOCK_LENGTH != 0 || offset >= blocks.size() * BLOCK_LENGTH || bytes == null)
            throw new IllegalArgumentException();

        // Exit, if block is already completed.
        if(blocks.get((int) (offset / BLOCK_LENGTH)) != null)
            return;

        blocks.set((int) (offset / BLOCK_LENGTH), bytes);

        // Increment completed blocks counter.
        final int newCompletedCount = mCompletesBlocksCount.get(pieceId) + 1;
        mCompletesBlocksCount.put(pieceId, newCompletedCount);

        final float percent = 100.f * newCompletedCount / blocks.size();
        Log.i("Block updated for piece #" + pieceId + ". Percent of piece completed is " + percent + "%.");

        // Try to save piece into file if all blocks are finished.
        if(isFinished(pieceId))
            setPiece(pieceId);
    }

    /**
     * @param pieceId - Piece identifier.
     * @param offset - Offset from beginning of the piece.
     * @return Returns byte array block for piece.
     */
    public synchronized @Nullable byte[] getBlockByPiece(@NotNull final int pieceId, @NotNull final long offset) {
        final List<byte[]> blocks = mPiecesBlocks.get(pieceId);
        if(blocks == null)
            throw new IndexOutOfBoundsException();

        if(offset % BLOCK_LENGTH != 0 || offset >= blocks.size() * BLOCK_LENGTH)
            throw new IllegalArgumentException();

        return blocks.get((int) (offset / BLOCK_LENGTH));
    }

    /**
     * @param bitfield - Bitfield value.
     * @see Bitfield
     * @return Returns all operational pieces, that contains into bitfield.
     */
    public synchronized @NotNull List<Integer> getAllPieces(@NotNull final Bitfield bitfield) {
        final List<Integer> result = new ArrayList<>();

        for(final Map.Entry<Integer, List<byte[]>> currentPieceBlocks: mPiecesBlocks.entrySet())
            if(bitfield.havePiece(currentPieceBlocks.getKey()))
                result.add(currentPieceBlocks.getKey());

        return result;
    }

    /**
     * @param bitfield - Bitfield value.
     * @see Bitfield
     * @param count - Max size of result list.
     * @return Returns list with non existing triples of values (piece identifier + offset + length), that contains into bitfield.
     */
    public synchronized @NotNull List<Triple<Integer, Long, Long>> getAllAvailableOffsets(@NotNull final Bitfield bitfield, @NotNull final int count) {
        final List<Triple<Integer, Long, Long>> result = new ArrayList<>();

        for(final Map.Entry<Integer, List<byte[]>> currentPieceBlocks: mPiecesBlocks.entrySet()) {
            if(bitfield.havePiece(currentPieceBlocks.getKey())) {
                final List<byte[]> blocks = currentPieceBlocks.getValue();
                final List<Long> extraditions = mPiecesExtraditions.get(currentPieceBlocks.getKey());

                for(int index = 0; index < blocks.size(); ++index) {
                    if(blocks.get(index) != null)
                        continue;

                    // Add triple into list, if piece isn't completer or timeout is passed.
                    final long now = Calendar.getInstance().getTimeInMillis();
                    final Long last = extraditions.get(index);
                    if(last == null || now - last > EXTRADITION_TIME) {
                        extraditions.set(index, now);

                        final int pieceId = currentPieceBlocks.getKey();
                        final long pieceOffset = index * BLOCK_LENGTH;
                        long pieceLength = BLOCK_LENGTH;

                        // For last piece length < BLOCK_LENGTH.
                        if(blocks.size() < mDefaultBlocksCount && index == blocks.size() - 1)
                            pieceLength = mTorrentParser.getSummaryLength() % BLOCK_LENGTH;

                        result.add(new ImmutableTriple<>(pieceId, pieceOffset, pieceLength));

                        // Result list size cant be greater than count parameter.
                        if(result.size() >= count)
                            return result;
                    }
                }
            }
        }

        return result;
    }

    /**
     * Callback function.
     * @param pieceId - New operational piece.
     */
    @Override
    public void onOperationPieceAllocated(@NotNull final int pieceId) {
        if(mCallback != null)
            mCallback.onOperationPieceAllocated(pieceId);
    }
}
