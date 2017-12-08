package interaction.torrentParser;

import interaction.torrentParser.bencode.*;
import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import interaction.fileAnalyser.DistributedFile;
import util.Log;
import util.MD5;
import util.SHA1;
import util.Util;

import java.io.*;
import java.net.MalformedURLException;
import java.net.URL;
import java.security.NoSuchAlgorithmException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 *  Parser of the torrent file.
 *  @author Boyarkin Nikita.
 */
public class TorrentParser {
    /**
     * Minimal length of one piece in bytes.
     */
    public static int PIECE_MINIMUM_LENGTH = 0x100;

    /**
     * Minimal length of tho one file.
     */
    public static int FILE_MINIMUM_LENGTH = 0x1;

    /** The announce URL of the tracker. */
    private @NotNull URL mAnnounce;

    /** This is an extention to the official specification, offering backwards-compatibility. */
    private @Nullable Set<URL> mAnnounceSet;

    /** The creation time of the torrent, in standard UNIX epoch format. */
    private @Nullable Long mCreationDate;

    /** Free-form textual comments of the author. */
    private @Nullable String mComment;

    /** Name and version of the program used to create the .torrent. */
    private @Nullable String mCreatedBy;

    /** The string encoding format used to generate the pieces part of the info dictionary in the .torrent metafile. */
    private @Nullable String mEncoding;

    /** Sha-1 hash of info dictionary. */
    private @NotNull SHA1 mInfoHash;

    /**
     * If it is set to "1", the client MUST publish its presence to get other peers ONLY via the trackers explicitly
     * described in the metainfo file. If this field is set to "0" or is not present, the client may obtain peer
     * from other means, e.g. PEX peer exchange, dht.
     */
    private @Nullable Boolean mPrivate;

    /** Number of bytes in each piece. */
    private @NotNull Long mPieceLength;

    /** Count of pieces. */
    private @NotNull Integer mPiecesCount;

    /** String consisting of the concatenation of all 20-byte SHA1 hash values, one per piece. */
    private @NotNull byte[] mPieces;

    /** Map representation of mPieces array. */
    private @NotNull Map<Integer, SHA1> mPiecesMap;

    /** True if only one file in torrent. */
    private @NotNull Boolean mSingleMode;

    /** Header string (if only one file - it's a filename, if many files - it's a directory).*/
    private @NotNull String mHeader;

    /** Information about single file. If (!mSingleMode) -> mSingleFile = null. */
    private @Nullable
    DistributedFile mSingleFile;

    /** Information about group of files. If (mSingleMode) -> mFiles = null. */
    private @Nullable List<DistributedFile> mFiles;

    /**
     * Creates TorrentParser object.
     * @param torrentFile - Path to the torrent file.
     * @throws ParseException Parse exception.
     * @throws InterruptedException If thread is interrupted.
     */
    public TorrentParser(@NotNull final File torrentFile) throws ParseException, InterruptedException {
        BencodeMap rootMap;

        InputStream torrentStream = null;
        try {
            // Try to parse *.torrent file.
            torrentStream = new FileInputStream(torrentFile);
            final BencodeObject bencodeObject = Bencode.parseBencode(torrentStream);

            // Checks, that root element is map.
            if(!(bencodeObject instanceof BencodeMap))
                throw new ParseException(ParseException.Code.ROOT_DICTIONARY_EXPECTED);

            rootMap = (BencodeMap) bencodeObject;
        }
        catch(final Exception exception) {
            if(exception instanceof ParseException)
                throw ((ParseException) exception);

            throw new ParseException(ParseException.Code.IMPOSSIBLE_TO_PARSE);
        }
        finally {
            try {
                if(torrentStream != null)
                    torrentStream.close();
            }
            catch(final IOException exception) { }
        }

        Util.checkInterrupted(null);
        setAnnounce(rootMap);

        Log.d("Announce and announce list have been successfully recognized: announce url - '" + mAnnounce.toString() +
                "'; announce backups count - " + ((mAnnounceSet != null) ? mAnnounceSet.size() : 0) + ".");

        Util.checkInterrupted(null);
        setMetadata(rootMap);

        final DateFormat dateFormat = new SimpleDateFormat("dd.MM.YYYY HH:mm:ss");
        Log.d("Metadata has been successfully recognized: comment - '" + mComment + "'; created by - '" + mCreatedBy +
                "'; creation date - '" + ((mCreationDate != null) ? dateFormat.format(new Date(mCreationDate * 1000)) : "null") +
                "'; encoding - '" + mEncoding + "'.");

        Util.checkInterrupted(null);
        setInfo(rootMap);

        Log.d("Info dictionary and files have been successfully recognized: piece length - " + mPieceLength + "; pieces count - " + mPiecesCount +
                "; info hash - '" + mInfoHash.toString() + "'.");
    }

    /**
     * Sets mAnnounce and mAnnounceSet values.
     * @param rootMap - Root dictionary of the torrent file.
     * @throws ParseException Parse exception.
     */
    private void setAnnounce(@NotNull final BencodeMap rootMap) throws ParseException {
        // Checks, that 'announce' element is string.
        BencodeObject bencodeObject = rootMap.get(new BencodeString("announce"));
        if(!(bencodeObject instanceof BencodeString))
            throw new ParseException(ParseException.Code.ANNOUNCE_STRING_EXPECTED);

        String announceUrl = bencodeObject.toString();
        try {
            // TODO
            if(announceUrl.startsWith("udp://"))
                announceUrl = announceUrl.replace("udp://", "http://");

            mAnnounce = new URL(announceUrl);

            if(mAnnounce.getQuery() != null)
                throw new ParseException(ParseException.Code.ANNOUNCE_URL_IS_NOT_VALID);
        }
        catch(final MalformedURLException exception) {
            throw new ParseException(ParseException.Code.ANNOUNCE_URL_IS_NOT_VALID);
        }

        bencodeObject = rootMap.get(new BencodeString("announce-list"));
        if(bencodeObject == null)
            return;

        // Checks, that 'announce-list' element is list.
        if(!(bencodeObject instanceof BencodeList))
            throw new ParseException(ParseException.Code.ANNOUNCE_LIST_EXPECTED);

        final BencodeList announceList = (BencodeList) bencodeObject;
        mAnnounceSet = new HashSet<>();

        for(final BencodeObject currentAnnounce: announceList) {
            // Checks, that current element is list.
            if(!(currentAnnounce instanceof BencodeList))
                throw new ParseException(ParseException.Code.ANNOUNCE_LIST_EXPECTED);

            final BencodeList tempList = (BencodeList) currentAnnounce;
            for(final BencodeObject currentBackup: tempList) {
                // Checks, that current element is string.
                if(!(currentBackup instanceof BencodeString))
                    throw new ParseException(ParseException.Code.BACKUP_STRING_EXPECTED);

                // Checks, that backup url is valid.
                String backupUrl = currentBackup.toString();
                try {
                    // TODO
                    if(backupUrl.startsWith("udp://"))
                        backupUrl = backupUrl.replace("udp://", "http://");

                    final URL backup = new URL(backupUrl);
                    if(backup.getQuery() != null)
                        throw new ParseException(ParseException.Code.ANNOUNCE_URL_IS_NOT_VALID);

                    mAnnounceSet.add(backup);
                }
                catch(final MalformedURLException exception) {
                    throw new ParseException(ParseException.Code.BACKUP_URL_IS_NOT_VALID);
                }
            }
        }

        // Make announce list unmodifiable.
        mAnnounceSet = Collections.unmodifiableSet(mAnnounceSet);
    }


    /**
     * Sets metadata fields (mComment, mCreatedBy, mCreationDate, mEncoding).
     * @param rootMap - Root dictionary of the torrent file.
     * @throws ParseException Parse exception.
     */
    private void setMetadata(@NotNull final BencodeMap rootMap) throws ParseException {
        // Optional param.
        BencodeObject bencodeObject = rootMap.get(new BencodeString("comment"));
        if(bencodeObject != null) {
            // Checks, that 'comment' element is string.
            if(!(bencodeObject instanceof BencodeString))
                throw new ParseException(ParseException.Code.COMMENT_STRING_EXPECTED);

            mComment = new String(((BencodeString) bencodeObject).getBytes());
        }

        // Optional param.
        bencodeObject = rootMap.get(new BencodeString("created by"));
        if(bencodeObject != null) {
            // Checks, that 'created by' element is string.
            if(!(bencodeObject instanceof BencodeString))
                throw new ParseException(ParseException.Code.CREATED_BY_STRING_EXPECTED);

            mCreatedBy = new String(((BencodeString) bencodeObject).getBytes());
        }

        // Optional param.
        bencodeObject = rootMap.get(new BencodeString("creation date"));
        if(bencodeObject != null) {
            // Checks, that 'creation date' element is integer.
            if(!(bencodeObject instanceof BencodeLong))
                throw new ParseException(ParseException.Code.CREATION_DATE_INTEGER_EXPECTED);

            mCreationDate = ((BencodeLong) bencodeObject).getLong();
        }

        // Optional param.
        bencodeObject = rootMap.get(new BencodeString("encoding"));
        if(bencodeObject != null) {
            // Checks, that 'encoding' element is string.
            if(!(bencodeObject instanceof BencodeString))
                throw new ParseException(ParseException.Code.ENCODING_STRING_EXPECTED);

            mEncoding = new String(((BencodeString) bencodeObject).getBytes());
        }
    }

    /**
     * Sets 'info' dictionary fields (mInfoHash, mPieceLength, mPieces, mPiecesMap, mSingleMode, mHeader, mPrivate, mSingleFile).
     * @param rootMap - Root dictionary of the torrent file.
     * @throws ParseException Parse exception.
     */
    private void setInfo(@NotNull final BencodeMap rootMap) throws ParseException {
        // Checks, that 'info' element is string.
        BencodeObject bencodeObject = rootMap.get(new BencodeString("info"));
        if(!(bencodeObject instanceof BencodeMap))
            throw new ParseException(ParseException.Code.INFO_DICTIONARY_EXPECTED);

        BencodeMap infoMap = (BencodeMap) bencodeObject;

        final ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();

        try {
            infoMap.write(byteArrayOutputStream);
            mInfoHash = SHA1.generateSHA1(byteArrayOutputStream.toByteArray());
        }
        catch(final IOException | NoSuchAlgorithmException exception) {
            throw new ParseException(ParseException.Code.IMPOSSIBLE_TO_GENERATE_INFO_HASH);
        }

        // Checks, that 'piece length' element is integer.
        bencodeObject = infoMap.get(new BencodeString("piece length"));
        if(!(bencodeObject instanceof BencodeLong))
            throw new ParseException(ParseException.Code.PIECE_LENGTH_INTEGER_EXPECTED);

        // Checks, that 'piece length' is power of two.
        final long pieceLength = ((BencodeLong) bencodeObject).getLong();
        if(pieceLength <= PIECE_MINIMUM_LENGTH || (pieceLength & (pieceLength - 1)) != 0)
            throw new ParseException(ParseException.Code.PIECE_LENGTH_WRONG_NUMBER);

        mPieceLength = pieceLength;

        // Checks, that 'pieces' element is string.
        bencodeObject = infoMap.get(new BencodeString("pieces"));
        if(!(bencodeObject instanceof BencodeString))
            throw new ParseException(ParseException.Code.PIECES_STRING_EXPECTED);

        // Checks, that pieces count are divided by 0x14 (SHA-1 length).
        int piecesCount = ((BencodeString) bencodeObject).getBytes().length;
        if(piecesCount == 0 || piecesCount % SHA1.LENGTH != 0)
            throw new ParseException(ParseException.Code.PIECES_WRONG_COUNT);

        mPieces = ((BencodeString) bencodeObject).getBytes();

        mPiecesMap = new TreeMap<>();
        mPiecesCount = piecesCount / SHA1.LENGTH;
        for(int pieceIndex = 0; pieceIndex < mPiecesCount; ++pieceIndex) {
            final byte[] value = new byte[SHA1.LENGTH];
            System.arraycopy(mPieces, pieceIndex * SHA1.LENGTH, value, 0, SHA1.LENGTH);
            mPiecesMap.put(pieceIndex, new SHA1(value));
        }

        // Make pieces map unmodifiable.
        mPiecesMap = Collections.unmodifiableMap(mPiecesMap);

        bencodeObject = infoMap.get(new BencodeString("length"));

        // In single mode, info dictionary must contains 'length' integer.
        mSingleMode = (bencodeObject != null);

        if(mSingleMode && !(bencodeObject instanceof BencodeLong))
            throw new ParseException(ParseException.Code.LENGTH_INTEGER_EXPECTED);

        Long fileLength = null;
        if(mSingleMode) {
            fileLength = ((BencodeLong) bencodeObject).getLong();
            if(fileLength <= FILE_MINIMUM_LENGTH)
                throw new ParseException(ParseException.Code.LENGTH_IS_TOO_SMALL);
        }

        // Checks, that 'name' element is string.
        bencodeObject = infoMap.get(new BencodeString("name"));
        if(!(bencodeObject instanceof BencodeString))
            throw new ParseException(ParseException.Code.NAME_STRING_EXPECTED);

        // Checks, that name is valid.
        final String name = bencodeObject.toString();
        if(mSingleMode && !Util.fileNameIsValid(name) || !mSingleMode && !Util.directoryNameIsValid(name))
            throw new ParseException(ParseException.Code.NAME_IS_NOT_VALID);

        mHeader = name;

        // Optional param.
        bencodeObject = rootMap.get(new BencodeString("private"));
        if(bencodeObject != null) {
            // Checks, that 'private' element is integer.
            if(!(bencodeObject instanceof BencodeLong))
                throw new ParseException(ParseException.Code.PRIVATE_BOOLEAN_EXPECTED);

            final long temp = ((BencodeLong) bencodeObject).getLong();
            if(temp != 0 && temp != 1)
                throw new ParseException(ParseException.Code.PRIVATE_BOOLEAN_EXPECTED);

            mPrivate = (temp != 0);
        }

        MD5 hash = null;

        // Optional param.
        bencodeObject = rootMap.get(new BencodeString("md5sum"));
        if(bencodeObject != null) {
            // Checks, that 'md5sum' element is string.
            if(!mSingleMode || !(bencodeObject instanceof BencodeString))
                throw new ParseException(ParseException.Code.MD5_HASH_STRING_EXPECTED);

            try {
                hash = new MD5(bencodeObject.toString());
            }
            catch(final IllegalArgumentException exception) {
                throw new ParseException(ParseException.Code.MD5_HASH_BROKEN);
            }
        }

        if(mSingleMode)
            mSingleFile = new DistributedFile(name, fileLength, hash, null);
        else
            setFiles(infoMap);
    }

    /**
     * Sets 'files' list fields (mFiles).
     * @param infoMap - Info dictionary of the torrent file.
     * @throws ParseException Parse exception.
     */
    private void setFiles(@NotNull final BencodeMap infoMap) throws ParseException {
        // Checks, that 'files' element is list.
        BencodeObject bencodeObject = infoMap.get(new BencodeString("files"));
        if(!(bencodeObject instanceof BencodeList))
            throw new ParseException(ParseException.Code.FILES_LIST_EXPECTED);

        // Checks, that files list isn't empty.
        final BencodeList filesList = (BencodeList) bencodeObject;
        if(filesList.isEmpty())
            throw new ParseException(ParseException.Code.FILES_LIST_IS_EMPTY);

        mFiles = new ArrayList<>();
        for(final BencodeObject currentFile: filesList) {
            // Checks, that current file is map.
            if(!(currentFile instanceof BencodeMap))
                throw new ParseException(ParseException.Code.FILE_DICTIONARY_EXPECTED);

            final BencodeMap tempMap = (BencodeMap) currentFile;

            // Checks, that 'length' element is integer.
            bencodeObject = tempMap.get(new BencodeString("length"));
            if(!(bencodeObject instanceof BencodeLong))
                throw new ParseException(ParseException.Code.LENGTH_INTEGER_EXPECTED);

            // Checks, that file length greater than minimum value.
            final long fileLength = ((BencodeLong) bencodeObject).getLong();
            if(fileLength <= FILE_MINIMUM_LENGTH)
                throw new ParseException(ParseException.Code.LENGTH_IS_TOO_SMALL);

            MD5 hash = null;

            // Optional param.
            bencodeObject = tempMap.get(new BencodeString("md5sum"));
            if(bencodeObject != null) {
                // Checks, that 'md5sum' element is string.
                if(!(bencodeObject instanceof BencodeString))
                    throw new ParseException(ParseException.Code.MD5_HASH_STRING_EXPECTED);

                try {
                    hash = new MD5(bencodeObject.toString());
                }
                catch(final IllegalArgumentException exception) {
                    throw new ParseException(ParseException.Code.MD5_HASH_BROKEN);
                }
            }

            // Checks, that 'path' element is list.
            bencodeObject = tempMap.get(new BencodeString("path"));
            if(!(bencodeObject instanceof BencodeList))
                throw new ParseException(ParseException.Code.PATHS_LIST_EXPECTED);

            // Checks, that paths list isn't empty.
            final BencodeList tempList = (BencodeList) bencodeObject;
            if(tempList.isEmpty())
                throw new ParseException(ParseException.Code.PATHS_LIST_IS_EMPTY);

            List<String> directories = null;
            String name = null;

            for(final BencodeObject currentPath: tempList) {
                // Checks, that current path is string.
                if(!(currentPath instanceof BencodeString))
                    throw new ParseException(ParseException.Code.PATH_STRING_EXPECTED);

                // Checks, that file names and directory names are valid.
                final String tempPath = currentPath.toString();

                if(currentPath != tempList.lastElement()) {
                    if(!Util.directoryNameIsValid(tempPath))
                        throw new ParseException(ParseException.Code.DIRECTORY_NAME_IS_NOT_VALID);

                    if(directories == null)
                        directories = new ArrayList<>();

                    directories.add(tempPath);
                }
                else {
                    // Last element is file name.
                    if(!Util.fileNameIsValid(tempPath))
                        throw new ParseException(ParseException.Code.FILE_NAME_IS_NOT_VALID);

                    name = tempPath;
                }
            }

            mFiles.add(new DistributedFile(name, fileLength, hash, directories));
        }

        mFiles = Collections.unmodifiableList(mFiles);
    }

    /**
     * Getters for all class fields.
     */

    public @NotNull URL getAnnounce() { return mAnnounce; }

    public @Nullable Long getCreationDate() { return mCreationDate; }

    public @Nullable String getComment() { return mComment; }

    public @Nullable String getCreatedBy() { return mCreatedBy; }

    public @Nullable String getEncoding() { return mEncoding; }

    public @NotNull SHA1 getInfoHash() { return mInfoHash; }

    public @Nullable Boolean getPrivate() { return mPrivate; }

    public @NotNull long getPieceLength() { return mPieceLength; }

    public @NotNull String getHeader() { return mHeader; }

    public @Nullable DistributedFile getSingleFile() { return mSingleFile; }

    public @Nullable List<DistributedFile> getFiles() { return mFiles; }

    public @NotNull boolean getSingleMode() { return mSingleMode; }

    public @Nullable Set<URL> getAnnounceSet() { return mAnnounceSet; }

    public @NotNull int getPiecesCount() { return mPiecesCount; }

    public @NotNull byte[] getPieces() { return mPieces; }

    public @NotNull Map<Integer, SHA1> getPiecesMap() { return mPiecesMap; }

    public @NotNull long getSummaryLength() {
        if(mSingleMode)
            return mSingleFile.length;

        long summary = 0;
        for(final DistributedFile currentFile: mFiles)
            summary += currentFile.length;

        return summary;
    }

    /**
     * Parsing exception class.
     * @author Boyarkin Nikita.
     */
    public static class ParseException extends Exception {
        /**
         * All types of exception codes.
         */
        enum Code {
            IMPOSSIBLE_TO_PARSE, ROOT_DICTIONARY_EXPECTED,
            ANNOUNCE_STRING_EXPECTED, ANNOUNCE_URL_IS_NOT_VALID,
            INFO_DICTIONARY_EXPECTED, IMPOSSIBLE_TO_GENERATE_INFO_HASH,
            PRIVATE_BOOLEAN_EXPECTED,
            MD5_HASH_STRING_EXPECTED, MD5_HASH_BROKEN,
            PIECE_LENGTH_INTEGER_EXPECTED, PIECE_LENGTH_WRONG_NUMBER,
            PIECES_STRING_EXPECTED, PIECES_WRONG_COUNT,
            LENGTH_INTEGER_EXPECTED, LENGTH_IS_TOO_SMALL,
            NAME_STRING_EXPECTED,  NAME_IS_NOT_VALID,
            FILES_LIST_EXPECTED, FILES_LIST_IS_EMPTY, FILE_DICTIONARY_EXPECTED,
            PATHS_LIST_EXPECTED, PATHS_LIST_IS_EMPTY, PATH_STRING_EXPECTED,
            DIRECTORY_NAME_IS_NOT_VALID, FILE_NAME_IS_NOT_VALID,
            ANNOUNCE_LIST_EXPECTED, BACKUP_STRING_EXPECTED, BACKUP_URL_IS_NOT_VALID,
            COMMENT_STRING_EXPECTED, CREATED_BY_STRING_EXPECTED,
            ENCODING_STRING_EXPECTED, CREATION_DATE_INTEGER_EXPECTED,
        }

        ParseException(@NotNull final Code code) {
            super(getMessageByCode(code));
        }

        ParseException(@NotNull final String message) {
            super(message);
        }

        ParseException(@NotNull final Throwable cause) {
            super(cause);
        }

        ParseException(@NotNull final String message, @NotNull final Throwable cause) {
            super(message, cause);
        }

        /**
         * @param code - Code of exception
         * @return Returns string description of exception by code.
         */
        private static @NotNull String getMessageByCode(@NotNull final Code code) {
            switch(code) {
                case IMPOSSIBLE_TO_PARSE: return "Impossible to parse interaction.torrentParser.bencode file.";
                case ROOT_DICTIONARY_EXPECTED: return "Root element isn't a dictionary.";
                case ANNOUNCE_STRING_EXPECTED: return "Field 'announce' missed or corrupted.";
                case ANNOUNCE_URL_IS_NOT_VALID: return "Announce url isn't valid.";
                case INFO_DICTIONARY_EXPECTED: return "Field 'info' missed or corrupted.";
                case IMPOSSIBLE_TO_GENERATE_INFO_HASH: return "Impossible to generate info hash.";
                case PRIVATE_BOOLEAN_EXPECTED: return "Field 'private' is corrupted.";
                case MD5_HASH_STRING_EXPECTED: return "Field 'MD5' is corrupted.";
                case MD5_HASH_BROKEN: return "MD5 hash has wrong format.";
                case PIECE_LENGTH_INTEGER_EXPECTED: return "Field 'piece length' missed or corrupted.";
                case PIECE_LENGTH_WRONG_NUMBER: return "Piece length isn't power of two.";
                case PIECES_STRING_EXPECTED: return "Field 'pieces' missed or corrupted.";
                case PIECES_WRONG_COUNT: return "Pieces count must be a multiple of 20.";
                case LENGTH_INTEGER_EXPECTED: return "Field 'length' missed or corrupted.";
                case LENGTH_IS_TOO_SMALL: return "File length is too small.";
                case NAME_STRING_EXPECTED: return "Field 'name' missed or corrupted.";
                case NAME_IS_NOT_VALID: return "Header is not valid.";
                case FILES_LIST_EXPECTED: return "Field 'files' missed or corrupted.";
                case FILES_LIST_IS_EMPTY: return "Field 'files' is empty.";
                case FILE_DICTIONARY_EXPECTED: return "Files list member must be a dictionary";
                case PATHS_LIST_EXPECTED: return "Field 'path' missed or corrupted.";
                case PATHS_LIST_IS_EMPTY: return "Field 'path' is empty.";
                case PATH_STRING_EXPECTED: return "Path list member must be a string.";
                case DIRECTORY_NAME_IS_NOT_VALID: return "Directory name isn't valid.";
                case FILE_NAME_IS_NOT_VALID: return "File name is not valid.";
                case ANNOUNCE_LIST_EXPECTED: return "Field 'announce-list' is corrupted.";
                case BACKUP_STRING_EXPECTED: return "Announce list member must be a string.";
                case BACKUP_URL_IS_NOT_VALID: return "Backup url into announce list isn't valid.";
                case COMMENT_STRING_EXPECTED: return "Field 'comment' is corrupted.";
                case CREATED_BY_STRING_EXPECTED: return "Field 'created by' is corrupted.";
                case ENCODING_STRING_EXPECTED: return "Field 'encoding' is corrupted.";
                case CREATION_DATE_INTEGER_EXPECTED: return "Field 'creation date' is corrupted.";
                default: return "Unknown exception.";
            }
        }
    }
}
