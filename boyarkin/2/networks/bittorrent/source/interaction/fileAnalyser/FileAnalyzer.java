package interaction.fileAnalyser;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import gui.FileTableData;
import interaction.torrentParser.TorrentParser;
import javafx.collections.ObservableList;
import javafx.util.Pair;
import org.apache.commons.lang.ArrayUtils;
import util.Log;
import util.SHA1;
import util.Util;

import java.awt.geom.IllegalPathStateException;
import java.io.*;
import java.security.NoSuchAlgorithmException;
import java.util.*;

/**
 *  Contains information about all files and pieces.
 *  @author Boyarkin Nikita.
 */
public class FileAnalyzer implements OnePieceChangeable {
    /**
     * TorrentParser object.
     * @see TorrentParser
     */
    private @NotNull TorrentParser mTorrentParser;

    /** Path to the torrent file (and folder with files). */
    private @NotNull String mPath;

    /**
     * FilesTableView list.
     * @see gui.Controller
     */
    private @NotNull final ObservableList<FileTableData> mFilesList;

    /**
     * List with information about all files and pieces.
     * @see FilePartition
     */
    private @NotNull List<FilePartition> mFilePartition;

    /** Bitfield representation of the completed pieces (updatable). */
    private @NotNull final Bitfield mBitfield;

    /** Callback, that calls when pieces changed. */
    private @NotNull
    PiecesPercentChangeable mPiecesPercentChangeable;

    /** Callback, that calls when pieces changes. */
    private @NotNull OnePieceChangeable mOnePieceChangeable;

    /**
     * Creates FileAnalyzer object.
     * @param filesList - FilesTableView list.
     * @param torrentFile - Torrent file descriptor.
     * @param torrentParser - TorrentParser object.
     * @throws AnalyzeException Analyze exception (usually caused by IOException or SecurityException).
     */
    public FileAnalyzer(@NotNull final ObservableList<FileTableData> filesList, @NotNull final File torrentFile, @NotNull final TorrentParser torrentParser) throws AnalyzeException, InterruptedException {
        mTorrentParser = torrentParser;
        mFilesList = filesList;
        mBitfield = new Bitfield(torrentParser.getPiecesCount());

        try {
            mPath = torrentFile.getParentFile().getCanonicalPath();
        }
        catch(final IOException exception) {
            throw new AnalyzeException(AnalyzeException.Code.IMPOSSIBLE_TO_GET_PATH);
        }

        Util.checkInterrupted(null);

        try {
            mFilePartition = new ArrayList<>();

            // Initialize mFilePartition object.
            setFilePartition();
        }
        catch(final SecurityException exception) {
            throw new AnalyzeException(AnalyzeException.Code.SECURITY_EXCEPTION);
        }

        Util.checkInterrupted(null);

        /* Handles all callbacks from all files using one function. */
        for(final FilePartition currentPartition: mFilePartition)
            currentPartition.setOnPieceChanged(this);
    }

    /**
     * Initializes mFilePartition list.
     * @throws SecurityException Security exception.
     * @throws AnalyzeException Analyze exception (usually caused by IOException).
     * @throws InterruptedException If thread is interrupted.
     */
    private void setFilePartition() throws SecurityException, AnalyzeException, InterruptedException {
        if(mTorrentParser.getSingleMode()) {
            // Single file, that contains in the same directory with .torrent file.
            final String path = mPath + File.separator + mTorrentParser.getHeader();
            File file = new File(path);

            if(file.exists() && !file.isDirectory()) {
                if(file.length() != mTorrentParser.getSingleFile().length)
                    throw new AnalyzeException(AnalyzeException.Code.WRONG_SIZE_OF_FILE);

                if(!file.canWrite() || !file.canRead())
                    throw new AnalyzeException(AnalyzeException.Code.SECURITY_EXCEPTION);

                Util.checkInterrupted(null);

                // Analyzes existing file and initializes mFilePartition.
                setPartitionFromSingleFile(file);
            }
            else {
                Util.checkInterrupted(null);

                // Creates single file and initializes mFilePartition.
                setPartitionFromSingleFile();
            }
        }
        else {
            // Several files, which root directory contains in the same directory with .torrent file.

            // Pairs of values: File (file descriptors) + Boolean (true if need to analyze).
            final List<Pair<File, Boolean>> files = new ArrayList<>();
            for(final DistributedFile currentFile: mTorrentParser.getFiles()) {
                File file;
                try {
                    file = createSingleFile(currentFile, mTorrentParser.getHeader());

                    // If file not contains, not need to analyze.
                    files.add(new Pair<>(file, false));
                }
                catch(final IllegalPathStateException exception) {
                    file = new File(exception.getMessage());
                    if(file.length() != currentFile.length)
                        throw new AnalyzeException(AnalyzeException.Code.WRONG_SIZE_OF_FILE);

                    if(!file.canWrite() || !file.canRead())
                        throw new AnalyzeException(AnalyzeException.Code.SECURITY_EXCEPTION);

                    Util.checkInterrupted(null);

                    // If file already contains, need to analyze.
                    files.add(new Pair<>(file, true));
                }
            }

            Util.checkInterrupted(null);

            // Initializes mFilePartition.
            setPartitionFromMultipleFiles(files);
        }

        mFilePartition = Collections.unmodifiableList(mFilePartition);
    }

    /**
     * Analyzes existing file and initializes mFilePartition.
     * @param file - File to analyze.
     * @throws SecurityException Security exception.
     * @throws AnalyzeException Analyze exception (usually caused by IOException).
     * @throws InterruptedException If thread is interrupted.
     */
    private void setPartitionFromSingleFile(@NotNull final File file) throws SecurityException, AnalyzeException, InterruptedException {
        final Map<Integer, Pair<FilePartition.Interval, Boolean>> piecesMap = new HashMap<>();

        BufferedInputStream inputStream = null;
        try {
            inputStream = new BufferedInputStream(new FileInputStream(file));
            final byte[] buffer = new byte[(int) mTorrentParser.getPieceLength()];

            long summary = 0;
            for(int pieceIndex = 0; pieceIndex < mTorrentParser.getPiecesCount(); ++pieceIndex) {
                Util.checkInterrupted(null);

                summary += mTorrentParser.getPieceLength();

                long min = 0, max = mTorrentParser.getPieceLength();

                // At the end of the file need to cut the bytes array.
                if(summary > mTorrentParser.getSingleFile().length)
                    max = mTorrentParser.getPieceLength() - (summary - mTorrentParser.getSingleFile().length);

                if(max <= 0)
                    break;

                final int bytesRead = inputStream.read(buffer);
                if(bytesRead <= 0)
                    break;

                byte[] temp = buffer;

                // At the end of the file need to cut the bytes array.
                if(bytesRead < mTorrentParser.getPieceLength())
                    temp = Arrays.copyOfRange(buffer, 0, bytesRead);

                // Check SHA-1 hash of bytes array.
                final boolean valid = SHA1.generateSHA1(temp).equals(mTorrentParser.getPiecesMap().get(pieceIndex));
                piecesMap.put(pieceIndex, new Pair<>(new FilePartition.Interval(min, max), valid));
            }

            Log.d("Partition for file '" + file.getCanonicalPath() + "' has been successfully created.");

            // Set bitfield pieces.
            for(final Map.Entry<Integer, Pair<FilePartition.Interval, Boolean>> bitfieldPiece: piecesMap.entrySet())
                if(bitfieldPiece.getValue().getValue())
                    mBitfield.setPiece(bitfieldPiece.getKey());

            mFilePartition.add(new FilePartition(mFilesList, file, piecesMap));
        }
        catch(final IOException | NoSuchAlgorithmException exception) {
            throw new AnalyzeException(AnalyzeException.Code.IMPOSSIBLE_TO_READ_FILE);
        }
        finally {
            try {
                if(inputStream != null)
                    inputStream.close();
            }
            catch(final IOException exception) { }
        }
    }

    /**
     * Initializes mFilePartition for new empty file.
     * @throws SecurityException Security exception.
     * @throws AnalyzeException Analyze exception (usually caused by IOException).
     * @throws InterruptedException If thread is interrupted.
     */
    private void setPartitionFromSingleFile() throws SecurityException, AnalyzeException, InterruptedException {
        // Create new file at the mPath directory.
        final File file = createSingleFile(mTorrentParser.getSingleFile(), null);

        final Map<Integer, Pair<FilePartition.Interval, Boolean>> piecesMap = new HashMap<>();

        long summary = 0;
        for(int pieceIndex = 0; pieceIndex < mTorrentParser.getPiecesCount(); ++pieceIndex) {
            Util.checkInterrupted(null);

            summary += mTorrentParser.getPieceLength();

            long min = 0, max = mTorrentParser.getPieceLength();

            // At the end of the file need to cut the piece interval.
            if(summary > mTorrentParser.getSingleFile().length)
                max = mTorrentParser.getPieceLength() - (summary - mTorrentParser.getSingleFile().length);

            if(max <= 0)
                break;

            // Set false, cuz no need to analyze empty file.
            piecesMap.put(pieceIndex, new Pair<>(new FilePartition.Interval(min, max), false));
        }

        try {
            Log.d("Partition for file '" + file.getCanonicalPath() + "' has been successfully created.");

            // Set bitfield pieces.
            for(final Map.Entry<Integer, Pair<FilePartition.Interval, Boolean>> bitfieldPiece: piecesMap.entrySet())
                if(bitfieldPiece.getValue().getValue())
                    mBitfield.setPiece(bitfieldPiece.getKey());

            mFilePartition.add(new FilePartition(mFilesList, file, piecesMap));
        }
        catch(final IOException exception) {
            throw new AnalyzeException(AnalyzeException.Code.IMPOSSIBLE_TO_READ_FILE);
        }
    }

    /**
     * Initializes mFilePartition for several files.
     * @param files - Pairs of values: File (file descriptors) + Boolean (true if need to analyze).
     * @throws SecurityException Security exception.
     * @throws AnalyzeException Analyze exception (usually caused by IOException).
     * @throws InterruptedException If thread is interrupted.
     */
    private void setPartitionFromMultipleFiles(@NotNull final List<Pair<File, Boolean>> files) throws SecurityException, AnalyzeException, InterruptedException {
        long summary = 0;

        // Offset from start of the current file.
        long offset = 0;

        // Current pair of values: File (file descriptor) + Boolean (true if need to analyze).
        Pair<File, Boolean> currentPair = files.remove(0);
        File currentFile = currentPair.getKey();
        boolean currentCheckNeeded = currentPair.getValue();

        /*
            If bytesRead == 0, need to analyze new buffer.
            If bytesRead != 0, need to add bytes to current buffer.
         */
        int bytesRead = 0;

        /*
            Information for mFilePartition, which postponed until bytesRead == 0.
            Actual between files.
         */
        final List<Pair<File, Map<Integer, Pair<FilePartition.Interval, Boolean>>>> snoozed = new ArrayList<>();

        /*
            For whole pieces: packetCheckNeeded = currentFile.currentCheckNeeded
            For shattered pieces: packetCheckNeeded = nextFile.currentCheckNeeded && packetCheckNeeded
        */
        boolean packetCheckNeeded;


        RandomAccessFile inputStream = null;
        try {
            // Read from the first file.
            inputStream = new RandomAccessFile(currentFile, "r");
            final byte[] buffer = new byte[(int) mTorrentParser.getPieceLength()];
            Map<Integer, Pair<FilePartition.Interval, Boolean>> piecesMap = new HashMap<>();

            for(final Map.Entry<Integer, SHA1> currentPiece : mTorrentParser.getPiecesMap().entrySet()) {
                Util.checkInterrupted(null);

                summary += mTorrentParser.getPieceLength();
                packetCheckNeeded = currentCheckNeeded;

                int pieceIndex = currentPiece.getKey();
                long min = 0, max = mTorrentParser.getPieceLength();

                if(summary <= currentFile.length()) {
                    // Piece is whole.

                    boolean valid = false;
                    if(currentCheckNeeded) {
                        // Set offset from start of the current file.
                        inputStream.seek(offset);

                        // Read all buffer, cuz we can!
                        final int countOfBytesRead = inputStream.read(buffer);
                        if(countOfBytesRead != mTorrentParser.getPieceLength())
                            throw new IOException();

                        // Check SHA-1 hash of bytes array.
                        valid = SHA1.generateSHA1(buffer).equals(currentPiece.getValue());
                    }

                    bytesRead = 0;
                    piecesMap.put(pieceIndex, new Pair<>(new FilePartition.Interval(min, max), valid));

                    offset += mTorrentParser.getPieceLength();

                    if(summary == currentFile.length()) {
                        Log.d("Partition for file '" + currentFile.getCanonicalPath() + "' has been successfully created.");

                        // If end of file, going to the new file.
                        if(files.isEmpty())
                            break;

                        // Set bitfield pieces.
                        for(final Map.Entry<Integer, Pair<FilePartition.Interval, Boolean>> bitfieldPiece: piecesMap.entrySet())
                            if(bitfieldPiece.getValue().getValue())
                                mBitfield.setPiece(bitfieldPiece.getKey());

                        mFilePartition.add(new FilePartition(mFilesList, currentFile, piecesMap));
                        piecesMap = new HashMap<>();

                        summary = 0;
                        offset = 0;

                        // Going to the next file.
                        currentPair = files.remove(0);
                        currentFile = currentPair.getKey();
                        currentCheckNeeded = currentPair.getValue();

                        // Read from the next file.
                        inputStream.close();
                        inputStream = new RandomAccessFile(currentFile, "r");
                    }
                }
                else {
                    // Piece is shattered.

                    while(true) {
                        Util.checkInterrupted(null);

                        if(packetCheckNeeded) {
                            // If piece need to bee checked.

                            // Set offset from start of the current file.
                            inputStream.seek(offset);

                            // Try to read all remaining bytes.
                            int lengthToRead = (int) (mTorrentParser.getPieceLength() - bytesRead);

                            min = bytesRead;

                            final int countOfBytesRead = inputStream.read(buffer, bytesRead, lengthToRead);
                            if(countOfBytesRead < 0)
                                throw new IOException();

                            bytesRead += countOfBytesRead;

                            if(bytesRead == mTorrentParser.getPieceLength()) {
                                // All bytes of buffer are read.

                                // Check SHA-1 hash of bytes array.
                                boolean valid = SHA1.generateSHA1(buffer).equals(currentPiece.getValue());

                                max = bytesRead;
                                piecesMap.put(pieceIndex, new Pair<>(new FilePartition.Interval(min, max), valid));

                                // Restore all saved partitions.
                                for(final Pair<File, Map<Integer, Pair<FilePartition.Interval, Boolean>>> currentSnooze : snoozed) {
                                    final FilePartition.Interval interval = currentSnooze.getValue().get(pieceIndex).getKey();
                                    currentSnooze.getValue().put(pieceIndex, new Pair<>(interval, valid));

                                    // Set bitfield pieces.
                                    for(final Map.Entry<Integer, Pair<FilePartition.Interval, Boolean>> bitfieldPiece: currentSnooze.getValue().entrySet())
                                        if(bitfieldPiece.getValue().getValue())
                                            mBitfield.setPiece(bitfieldPiece.getKey());

                                    mFilePartition.add(new FilePartition(mFilesList, currentSnooze.getKey(), currentSnooze.getValue()));
                                }

                                snoozed.clear();
                                bytesRead = 0;
                            } else {
                                // Not all bytes of buffer are read.

                                max = bytesRead;
                                piecesMap.put(pieceIndex, new Pair<>(new FilePartition.Interval(min, max), null));

                                // Save partition, while all bytes isn't read.
                                snoozed.add(new Pair<>(currentFile, piecesMap));

                                piecesMap = new HashMap<>();
                            }
                        } else {
                            // If piece not need to bee checked.

                            min = bytesRead;
                            bytesRead += (summary > currentFile.length()) ? currentFile.length() - offset : summary - offset;

                            if(bytesRead == mTorrentParser.getPieceLength()) {
                                // If piece is finished.

                                max = bytesRead;
                                piecesMap.put(pieceIndex, new Pair<>(new FilePartition.Interval(min, max), false));

                                // Restore all saved partitions.
                                for(final Pair<File, Map<Integer, Pair<FilePartition.Interval, Boolean>>> currentSnooze : snoozed) {
                                    final FilePartition.Interval interval = currentSnooze.getValue().get(pieceIndex).getKey();
                                    currentSnooze.getValue().put(pieceIndex, new Pair<>(interval, false));

                                    // Set bitfield pieces.
                                    for(final Map.Entry<Integer, Pair<FilePartition.Interval, Boolean>> bitfieldPiece: currentSnooze.getValue().entrySet())
                                        if(bitfieldPiece.getValue().getValue())
                                            mBitfield.setPiece(bitfieldPiece.getKey());

                                    mFilePartition.add(new FilePartition(mFilesList, currentSnooze.getKey(), currentSnooze.getValue()));
                                }

                                snoozed.clear();
                                bytesRead = 0;
                            } else {
                                // If piece isn't finished.

                                max = bytesRead;
                                piecesMap.put(pieceIndex, new Pair<>(new FilePartition.Interval(min, max), false));

                                // Save partition, while all bytes isn't read.
                                snoozed.add(new Pair<>(currentFile, piecesMap));

                                piecesMap = new HashMap<>();
                            }
                        }

                        // If piece is finished.
                        if(bytesRead == 0) {
                            offset = summary;
                            break;
                        }

                        Log.d("Partition for file '" + currentFile.getCanonicalPath() + "' has been successfully created.");

                        if(files.isEmpty()) {
                            // If next file isn't exists.

                            boolean valid = false;
                            if(packetCheckNeeded) {
                                // At the end of the file need to cut the bytes array.
                                final byte[] temp = Arrays.copyOfRange(buffer, 0, bytesRead);

                                // Check SHA-1 hash of bytes array.
                                valid = SHA1.generateSHA1(temp).equals(currentPiece.getValue());
                            }

                            // Restore all saved partitions.
                            for(final Pair<File, Map<Integer, Pair<FilePartition.Interval, Boolean>>> currentSnooze: snoozed) {
                                final FilePartition.Interval interval = currentSnooze.getValue().get(pieceIndex).getKey();
                                currentSnooze.getValue().put(pieceIndex, new Pair<>(interval, valid));

                                // Set bitfield pieces.
                                for(final Map.Entry<Integer, Pair<FilePartition.Interval, Boolean>> bitfieldPiece: currentSnooze.getValue().entrySet())
                                    if(bitfieldPiece.getValue().getValue())
                                        mBitfield.setPiece(bitfieldPiece.getKey());

                                mFilePartition.add(new FilePartition(mFilesList, currentSnooze.getKey(), currentSnooze.getValue()));
                            }

                            inputStream.close();
                            return;
                        }

                        summary -= currentFile.length();
                        offset = 0;

                        // Going to the next file.
                        currentPair = files.remove(0);
                        currentFile = currentPair.getKey();
                        currentCheckNeeded = currentPair.getValue();
                        packetCheckNeeded = currentCheckNeeded && packetCheckNeeded;

                        // Read from the next file.
                        inputStream.close();
                        inputStream = new RandomAccessFile(currentFile, "r");
                    }
                }
            }
        }
        catch(final IOException | NoSuchAlgorithmException exception) {
            throw new AnalyzeException(AnalyzeException.Code.IMPOSSIBLE_TO_READ_FILE);
        }
        finally {
            try {
                if(inputStream != null)
                    inputStream.close();
            }
            catch(final IOException exception) { }
        }
    }

    /**
     * Creates new file at the mPath + root directory.
     * @param distributedFile - Information about creating file.
     * @param root - Root directory (can be null).
     * @return Created file.
     * @throws SecurityException Security exception.
     * @throws AnalyzeException Analyze exception (usually caused by IOException).
     * @throws IllegalPathStateException If file is already exists.
     * @throws InterruptedException If thread is interrupted.
     */
    private @NotNull File createSingleFile(@NotNull final DistributedFile distributedFile, @Nullable final String root) throws SecurityException, AnalyzeException, IllegalPathStateException, InterruptedException {
        Util.checkInterrupted(null);

        final StringBuilder path = new StringBuilder(mPath);

        // Append root directory, if contains.
        if(root != null)
            path.append(File.separator).append(root);

        // Append special file path, if contains.
        if(distributedFile.directories != null)
            for(final String currentDirectory: distributedFile.directories)
                path.append(File.separator).append(currentDirectory);

        path.append(File.separator).append(distributedFile.name);

        final File file = new File(path.toString());
        file.getParentFile().mkdirs();

        try{
            // If file is already exists, throw exception.
            if(!file.createNewFile()) {
                Log.d("File '" + path + "' is already exists.");
                throw new IllegalPathStateException(path.toString());
            }
        }
        catch(final IOException exception) {
            throw new AnalyzeException(AnalyzeException.Code.IMPOSSIBLE_TO_CREATE_FILE);
        }

        Util.checkInterrupted(null);

        RandomAccessFile randomAccessFile = null;
        try {
            // Set file length (initialize file with 0x0 bytes).
            randomAccessFile = new RandomAccessFile(file, "rw");
            randomAccessFile.setLength(distributedFile.length);
            randomAccessFile.close();
        }
        catch(final IOException exception) {
            throw new AnalyzeException(AnalyzeException.Code.IMPOSSIBLE_TO_FILL_FILE);
        }
        finally {
            try {
                if(randomAccessFile != null)
                    randomAccessFile.close();
            }
            catch(final IOException exception) { }
        }

        Log.d("File '" + path + "' has been successfully created.");
        return file;
    }


    /**
     * @return Completed value in percent (from all pieces).
     */
    public float getAllPiecesPercent() {
        long percent = 0;
        long summary = 0;
        for(final FilePartition currentPartition: mFilePartition) {
            percent += currentPartition.getBytesCompleted();
            summary += currentPartition.getFile().length();
        }


        float result = 0;
        if(summary != 0)
            result = ((float) percent) / summary * 100.f;

        return result;
    }

    /**
     * @return Returns total count of completed bytes.
     */
    public long getAllBytesCompleted() {
        long completed = 0;

        for(final FilePartition currentPartition: mFilePartition)
            completed += currentPartition.getBytesCompleted();

        return completed;
    }

    /**
     * @return Returns total count of left bytes.
     */
    public long getAllBytesLeft() {
        long left = 0;

        for(final FilePartition currentPartition: mFilePartition)
            left += currentPartition.getBytesLeft();

        return left;
    }

    /**
     * @return Returns bitfield object.
     */
    public @NotNull Bitfield getBitfield() {
        return mBitfield;
    }

    /**
     * Sets callback on change all pieces.
     * @param piecesPercentChangeable - PiecesPercentChangeable callback.
     */
    public void setOnAllPiecesChanged(@NotNull final PiecesPercentChangeable piecesPercentChangeable) {
        mPiecesPercentChangeable = piecesPercentChangeable;
    }

    /**
     * Sets callback on change of piece.
     * @param onePieceChangeable - OnePieceChangeable callback.
     */
    public void setOnOnePieceChanged(@NotNull final OnePieceChangeable onePieceChangeable) {
        mOnePieceChangeable = onePieceChangeable;
    }

    /**
     * Calls, when one of piece is changed.
     * @param filePartition - File partition of piece.
     * @param pieceId - Piece id.
     */
    @Override
    public void onOnePieceChanged(@NotNull final FilePartition filePartition, @NotNull final int pieceId) {
        mBitfield.setPiece(pieceId);

        if(mPiecesPercentChangeable != null)
            mPiecesPercentChangeable.onPiecesPercentChanged(getAllPiecesPercent());

        if(mOnePieceChangeable != null)
            mOnePieceChangeable.onOnePieceChanged(filePartition, pieceId);
    }

    /**
     * @param pieceId - Piece id.
     * @return Returns length of specific piece (size of piece interval).
     */
    public synchronized long getLengthOfPiece(@NotNull final int pieceId) throws IndexOutOfBoundsException {
        long result = 0;

        for(final FilePartition currentPartition: mFilePartition)
            if(currentPartition.contains(pieceId))
                result += currentPartition.getLengthOfPiece(pieceId);

        return result;
    }

    /**
     * @return Returns true if all files successfully uploaded.
     */
    public synchronized boolean isFinished() {
        boolean result = true;

        for(final FilePartition currentPartition: mFilePartition)
            result = result && currentPartition.isFinished();

        return result;
    }

    /**
     * @return First non existing piece of files (returns null if all pieces are exist).
     */
    public synchronized @Nullable Integer getNextNonexistingPieceId() {
        for(final FilePartition currentPartition: mFilePartition)
            if(!currentPartition.isFinished())
                return currentPartition.getNextNonexistingPieceId();

        return null;
    }

    /**
     * @param count - Count of desired pieces identifiers.
     * @return Returns list of non existing pieces identifiers.
     * @throws IllegalArgumentException If parameter count <= 0.
     */
    public synchronized @NotNull List<Integer> getNextNonexistingPieceIdList(@NotNull int count) throws IllegalArgumentException {
        if(count <= 0)
            throw new IllegalArgumentException();

        final List<Integer> result = new ArrayList<>();

        for(final FilePartition currentPartition: mFilePartition)
            if(!currentPartition.isFinished()) {
                final List<Integer> temp = currentPartition.getNextNonexistingPieceIdList(count);
                if(temp.isEmpty())
                    continue;

                result.addAll(temp);

                count -= temp.size();
                if(count == 0)
                    break;
            }

        return result;
    }

    /**
     * @param pieceId - Piece id.
     * @param offset - Offset from start of the piece.
     * @param size - Size of the interval.
     * @return Returns block of bytes into all partitions.
     * @throws IllegalArgumentException If impossible to get block.
     */
    public synchronized @NotNull byte[] getPieceBlock(@NotNull final int pieceId, @NotNull final int offset, @NotNull final int size) {
        if(pieceId < 0 || pieceId >= mTorrentParser.getPiecesCount() ||
                offset < 0 || offset >= mTorrentParser.getPieceLength() ||
                size <= 0 || size >= mTorrentParser.getPieceLength())
            throw new IllegalArgumentException();

        byte[] result = null;
        for(final FilePartition currentPartition: mFilePartition) {
            final byte[] bytes = currentPartition.getPieceBlock(pieceId, offset, size, mTorrentParser);
            result = ArrayUtils.addAll(result, bytes);
        }

        if(result == null || result.length != size)
            throw new IllegalArgumentException();

        return result;
    }

    /**
     * Saves piece into file (or files).
     * @param pieceId - Piece id.
     * @param bytes - Piece bytes.
     * @throws NoSuchAlgorithmException If SHA-1 algorithm isn't exist.
     * @throws IllegalArgumentException If size of bytes array is wrong.
     * @throws IOException If impossible to write into file.
     */
    public synchronized void setPiece(@NotNull final int pieceId, @NotNull final byte[] bytes) throws NoSuchAlgorithmException, IllegalArgumentException, IOException {
        for(final FilePartition currentPartition: mFilePartition)
            if(currentPartition.contains(pieceId))
                currentPartition.setPiece(pieceId, bytes, mTorrentParser);
    }

    /**
     * Analyzing exception class.
     * @author Boyarkin Nikita.
     */
    public static class AnalyzeException extends Exception {
        /**
         * All types of exception codes.
         */
        enum Code {
            IMPOSSIBLE_TO_GET_PATH,
            SECURITY_EXCEPTION,
            IMPOSSIBLE_TO_CREATE_FILE,
            IMPOSSIBLE_TO_FILL_FILE,
            WRONG_SIZE_OF_FILE,
            IMPOSSIBLE_TO_READ_FILE
        }

        AnalyzeException(@NotNull final Code code) {
            super(getMessageByCode(code));
        }

        AnalyzeException(@NotNull final String message) {
            super(message);
        }

        AnalyzeException(@NotNull final Throwable cause) {
            super(cause);
        }

        AnalyzeException(@NotNull final String message, @NotNull final Throwable cause) {
            super(message, cause);
        }

        /**
         * @param code - Code of exception
         * @return Returns string description of exception by code.
         */
        private static @NotNull String getMessageByCode(@NotNull final Code code) {
            switch(code) {
                case IMPOSSIBLE_TO_GET_PATH: return "Impossible to get .torrent path.";
                case SECURITY_EXCEPTION: return "Security exception.";
                case IMPOSSIBLE_TO_CREATE_FILE: return "Impossible to create file.";
                case IMPOSSIBLE_TO_FILL_FILE: return "Impossible to initialize file.";
                case WRONG_SIZE_OF_FILE: return "Existing file has wrong size.";
                case IMPOSSIBLE_TO_READ_FILE: return "Impossible to read file.";
                default: return "Unknown exception.";
            }
        }
    }
}
