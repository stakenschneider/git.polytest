package interaction.fileAnalyser;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import gui.FileTableData;
import interaction.torrentParser.TorrentParser;
import javafx.application.Platform;
import javafx.collections.ObservableList;
import javafx.util.Pair;
import util.Log;
import util.SHA1;
import util.Util;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Pieces partition of file.
 * @author Boyarkin Nikita.
 */
public class FilePartition implements OnePieceChangeable {
    /** Count of pieces in file. */
    private @NotNull final int mCountOfPieces;

    /** Count of existing pieces in file. */
    private @NotNull int mCountOfExistingPieces;

    /** Minimal and maximal piece index in partition. */
    private @NotNull int mMinIndex, mMaxIndex;

    /** Count of completed bytes. */
    private @NotNull long mCountOfCompletedBytes;

    /** File descriptor. */
    private @NotNull final File mFile;

    /**
     * Pieces map, that contains piece interval and completed flag.
     * @see Interval
     */
    private @NotNull final Map<Integer, Pair<Interval, Boolean>> mPieces;

    /** Callback, that calls when pieces changes. */
    private @NotNull OnePieceChangeable mOnePieceChangeable;

    /**
     * Data model for current partition.
     * @see FileTableData
     */
    private @NotNull FileTableData mFileTableData;

    /**
     * Interval of piece (part of piece, that contains into file).
     * @author Boyarkin Nikita.
     */
    public static class Interval {
        /** Interval limits.  */
        public @NotNull final long min, max;

        /**
         * Creates Interval object.
         * @param min - Least limit.
         * @param max - Greatest limit.
         * @throws IllegalArgumentException Throws, when min < 0 or min > max.
         */
        Interval(@NotNull final long min, @NotNull final long max) throws IllegalArgumentException {
            if(min < 0 || max < min)
                throw new IllegalArgumentException();

            this.min = min;
            this.max = max;
        }

        /**
         * @return Size of the interval.
         */
        public @NotNull long getIntervalLength() {
            return max - min;
        }
    }

    /**
     * Creates FilePartition object.
     * @param filesList - FilesTableView list.
     * @see gui.Controller
     * @param file - File descriptor.
     * @param pieces - Map of the pieces with intervals.
     * @throws IOException If impossible to get path to file.
     */
    public FilePartition(@NotNull final ObservableList<FileTableData> filesList, @NotNull final File file, @NotNull final Map<Integer, Pair<Interval, Boolean>> pieces) throws IOException {
        mFile = file;
        mPieces = (pieces instanceof ConcurrentHashMap) ? pieces : new ConcurrentHashMap<>(pieces);

        mCountOfPieces = mPieces.entrySet().size();

        mMinIndex = Integer.MAX_VALUE;
        mMaxIndex = Integer.MIN_VALUE;

        mCountOfExistingPieces = 0;
        mCountOfCompletedBytes = 0;
        for(final Map.Entry<Integer, Pair<Interval, Boolean>> currentPiece: pieces.entrySet()) {
            // Find minimum value.
            if(currentPiece.getKey() < mMinIndex)
                mMinIndex = currentPiece.getKey();

            // Find maximum value.
            if(currentPiece.getKey() > mMaxIndex)
                mMaxIndex = currentPiece.getKey();

            // Find count of existing pieces.
            if(currentPiece.getValue().getValue()) {
                ++mCountOfExistingPieces;
                mCountOfCompletedBytes += currentPiece.getValue().getKey().getIntervalLength();
            }
        }

        mFileTableData = new FileTableData(-1, file.getCanonicalPath(), Util.lengthToHumanReadableFormat(file.length(), false), getBytesPercentCompleted());
        Platform.runLater(() -> filesList.add(mFileTableData));
    }

    /**
     * @param pieceId - Piece id.
     * @return Returns true, if piece contains into partition.
     */
    public synchronized @NotNull boolean contains(@NotNull final int pieceId) {
        return mPieces.get(pieceId) != null;
    }

    /**
     * @return First non existing piece of file (returns null if all pieces are exist).
     */
    public synchronized @Nullable Integer getNextNonexistingPieceId() {
        for(final Map.Entry<Integer, Pair<Interval, Boolean>> currentPiece: mPieces.entrySet())
            if(!currentPiece.getValue().getValue())
                return currentPiece.getKey();

        return null;
    }

    /**
     * @param count - Count of desired pieces identifiers.
     * @return Returns list of non existing pieces identifiers.
     * @throws IllegalArgumentException If parameter count <= 0.
     */
    public synchronized @Nullable List<Integer> getNextNonexistingPieceIdList(@NotNull int count) throws IllegalArgumentException {
        if(count <= 0)
            throw new IllegalArgumentException();

        final List<Integer> result = new ArrayList<>();

        for(final Map.Entry<Integer, Pair<Interval, Boolean>> currentPiece: mPieces.entrySet())
            if(!currentPiece.getValue().getValue()) {
                result.add(currentPiece.getKey());

                if(--count == 0)
                    break;
            }

        return result;
    }

    /**
     * @param pieceId - Piece id.
     * @param pieceMaxLength - Max length of the piece.
     * @return Returns offset from begin of the file for specific piece.
     * @throws IndexOutOfBoundsException If mMinIndex isn't contains into partition.
     */
    public synchronized long getOffsetForPiece(@NotNull final int pieceId, @NotNull final long pieceMaxLength) throws IndexOutOfBoundsException {
        if(pieceId == mMinIndex)
            return 0;

        return getLengthOfPiece(mMinIndex) + (pieceId - mMinIndex - 1) * pieceMaxLength;
    }

    /**
     * @param pieceId - Piece id.
     * @return Returns length of specific piece (size of piece interval).
     * @throws IndexOutOfBoundsException If piece isn't contains into partition.
     */
    public synchronized long getLengthOfPiece(@NotNull final int pieceId) throws IndexOutOfBoundsException {
        final Pair<Interval, Boolean> minPiece = mPieces.get(pieceId);
        if(minPiece == null)
            throw new IndexOutOfBoundsException();

        return minPiece.getKey().getIntervalLength();
    }

    /**
     * @return Returns true if file upload is finished (all pieces completed).
     */
    public synchronized boolean isFinished() {
        return mCountOfExistingPieces == mCountOfPieces;
    }

    /**
     * Saves piece into file, increments mCountOfExistingPieces and call callback function.
     * @param pieceId - Piece id.
     * @param bytes - Piece bytes.
     * @param torrentParser - TorrentParser object.
     * @throws NoSuchAlgorithmException If SHA-1 algorithm isn't exist.
     * @throws IndexOutOfBoundsException If piece isn't contains into partition.
     * @throws IllegalArgumentException If size of bytes array is wrong.
     * @throws IOException If impossible to write into file.
     */
    public synchronized void setPiece(@NotNull final int pieceId, @NotNull final byte[] bytes, @NotNull final TorrentParser torrentParser) throws NoSuchAlgorithmException, IndexOutOfBoundsException, IllegalArgumentException, IOException {
        final SHA1 reference = torrentParser.getPiecesMap().get(pieceId);
        final Pair<Interval, Boolean> piece = mPieces.get(pieceId);

        if(reference == null || piece == null)
            throw new IndexOutOfBoundsException();

        if(piece.getValue())
            return;

        if(bytes.length > torrentParser.getPieceLength() || piece.getKey().max > bytes.length)
            throw new IllegalArgumentException();

        if(SHA1.generateSHA1(bytes).equals(reference)) {
            // Get offset from begin of the file for piece.
            final long pieceOffset = getOffsetForPiece(pieceId, torrentParser.getPieceLength());

            // Save bytes into file.
            final RandomAccessFile inputStream = new RandomAccessFile(mFile, "rw");
            inputStream.seek(pieceOffset);
            inputStream.write(Arrays.copyOfRange(bytes, (int) piece.getKey().min, (int) piece.getKey().max));
            inputStream.close();

            // Mark piece as completed.
            mPieces.put(pieceId, new Pair<>(piece.getKey(), true));
            ++mCountOfExistingPieces;
            mCountOfCompletedBytes += piece.getKey().getIntervalLength();

            // Call callback function.
            onOnePieceChanged(this, pieceId);
        }
        else
            throw new IllegalArgumentException();
    }

    /**
     * @return Returns file descriptor.
     */
    public synchronized @NotNull File getFile() {
        return mFile;
    }

    /**
     * @return Returns percent of uploaded pieces.
     */
    public synchronized float getPiecesPercentCompleted() {
        return ((float) mCountOfExistingPieces) / mCountOfPieces * 100;
    }

    /**
     * @return Returns percent of uploaded bytes.
     */
    public synchronized float getBytesPercentCompleted() {
        return ((float) mCountOfCompletedBytes) / mFile.length() * 100;
    }

    /**
     * @return Returns count of completed bytes (slow function).
     */
    public synchronized long getBytesCompleted() {
        return mCountOfCompletedBytes;
    }

    /**
     * @return Returns count of left bytes (slow function).
     */
    public synchronized long getBytesLeft() {
        return mFile.length() - mCountOfCompletedBytes;
    }

    /**
     * @param pieceId - Piece id.
     * @param offset - Offset from start of the piece.
     * @param size - Size of the interval.
     * @param torrentParser - TorrentParser object.
     * @return Returns block of bytes into partition, or null if it's impossible.
     */
    public synchronized @Nullable byte[] getPieceBlock(@NotNull final int pieceId, @NotNull final int offset, @NotNull final int size, @NotNull final TorrentParser torrentParser) {
        if(!contains(pieceId))
            return null;

        final Pair<Interval, Boolean> piece = mPieces.get(pieceId);
        final Interval interval = piece.getKey();

        if(interval.max <= offset || interval.min >= offset + size)
            return null;

        // Get offset from begin of the file for piece.
        final long pieceOffset = getOffsetForPiece(pieceId, torrentParser.getPieceLength());

        // Common parts of intervals.
        long min = Math.max(interval.min, offset);
        long max = Math.min(interval.max, offset + size);

        final byte[] result = new byte[(int) (max - min)];

        RandomAccessFile outputStream = null;

        int countOfBytesRead;
        try {
            // Read bytes from file.
            outputStream = new RandomAccessFile(mFile, "r");
            outputStream.seek(pieceOffset + min);
            countOfBytesRead = outputStream.read(result);
        }
        catch(final IOException exception) {
            return null;
        }
        finally {
            // Close stream if needed.
            if(outputStream != null) {
                try { outputStream.close(); }
                catch(final IOException exception) {}
            }
        }

        if(countOfBytesRead != max - min)
            return null;

        return result;
    }

    /**
     * Sets callback on change of piece.
     * @param onePieceChangeable - OnePieceChangeable callback.
     */
    public void setOnPieceChanged(@NotNull final OnePieceChangeable onePieceChangeable) {
        mOnePieceChangeable = onePieceChangeable;
    }

    /**
     * Callback function.
     * @param filePartition - File partition of piece.
     * @param pieceId - Piece id.
     */
    @Override
    public synchronized void onOnePieceChanged(@NotNull final FilePartition filePartition, @NotNull int pieceId) {
        Log.d("Piece #" + pieceId + " has been changed.");

        // Refresh percent value.
        filePartition.mFileTableData.setPercent(Math.round(filePartition.getBytesPercentCompleted() * 100.f) / 100.f);

        if(mOnePieceChangeable != null)
            mOnePieceChangeable.onOnePieceChanged(filePartition, pieceId);
    }
}