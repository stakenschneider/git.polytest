package interaction.peerToPeer.message;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.util.Arrays;

/**
 * BitTorrent 'request' message (message type 0x6).
 *
 * [length = 13 [4 bytes]]
 * [message type = 6 [1 byte]]
 * [piece identifier [4 bytes]]
 * [offset [4 bytes]]
 * [length [4 bytes]]
 *
 * @see BitTorrentMessage
 * @author Boyarkin Nikita.
 */
public class BitTorrentRequest extends BitTorrentMessage {
    /** Static message size of the 'request' message. */
    public static @NotNull final int DEFAULT_MESSAGE_LENGTH = 0x11;

    /** Length of message type field (in bytes). */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE_LENGTH = 0x1;

    /** Message type value. */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE = 0x6;

    /** Length of piece size field (in bytes). */
    public static @NotNull final int DEFAULT_SIZE_LENGTH = 0x4;

    /** Offset from the beginning of piece size field (in bytes). */
    public static @NotNull final int DEFAULT_SIZE_OFFSET = DEFAULT_MESSAGE_LENGTH - DEFAULT_SIZE_LENGTH;

    /** Length of piece offset field (in bytes). */
    public static @NotNull final int DEFAULT_BEGIN_LENGTH = 0x4;

    /** Offset from the beginning of piece offset field (in bytes). */
    public static @NotNull final int DEFAULT_BEGIN_OFFSET = DEFAULT_MESSAGE_LENGTH - DEFAULT_BEGIN_LENGTH - DEFAULT_SIZE_LENGTH;

    /** Length of piece identifier field (in bytes). */
    public static @NotNull final int DEFAULT_PIECE_ID_LENGTH = 0x4;

    /** Offset from the beginning of piece identifier field (in bytes). */
    public static @NotNull final int DEFAULT_PIECE_ID_OFFSET = DEFAULT_MESSAGE_LENGTH - DEFAULT_PIECE_ID_LENGTH - DEFAULT_BEGIN_LENGTH - DEFAULT_SIZE_LENGTH;

    /**
     * Identifier of the requested piece.
     */
    private @NotNull final int mPieceId;

    /**
     * Offset from beginning of the requested piece.
     */
    private @NotNull final int mBegin;

    /**
     * Length of the requested piece.
     */
    private @NotNull final int mSize;

    /**
     * Creates new BitTorrentRequest object.
     * @param pieceId - Identifier of the requested piece.
     * @param begin - Offset from beginning of the requested piece.
     * @param size - Length of the requested piece.
     */
    public BitTorrentRequest(@NotNull final int pieceId, @NotNull final int begin, @NotNull final int size) {
        super(new byte[DEFAULT_MESSAGE_LENGTH]);

        mPieceId = pieceId;
        mBegin = begin;
        mSize = size;

        initializeBytes();
    }

    /**
     * Initializes mBytes array.
     * @see BitTorrentMessage#initializeBytes()
     */
    @Override
    protected void initializeBytes() {
        Arrays.fill(mBytes, 0, DEFAULT_MESSAGE_LENGTH, (byte) 0x0);

        // Set message size field.
        mBytes[3] = DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_PIECE_ID_LENGTH + DEFAULT_BEGIN_LENGTH + DEFAULT_SIZE_LENGTH;

        // Set message type field.
        mBytes[4] = DEFAULT_MESSAGE_TYPE;

        // Set identifier of the requested piece.
        for(int index = DEFAULT_PIECE_ID_OFFSET; index < DEFAULT_PIECE_ID_OFFSET + DEFAULT_PIECE_ID_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_PIECE_ID_OFFSET + DEFAULT_PIECE_ID_LENGTH - index - 1);
            mBytes[index] = (byte) ((mPieceId & (0xFF << count)) >> count);
        }

        // Set offset from beginning of the requested piece.
        for(int index = DEFAULT_BEGIN_OFFSET; index < DEFAULT_BEGIN_OFFSET + DEFAULT_BEGIN_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_BEGIN_OFFSET + DEFAULT_BEGIN_LENGTH - index - 1);
            mBytes[index] = (byte) ((mBegin & (0xFF << count)) >> count);
        }

        // Set length of the requested piece.
        for(int index = DEFAULT_SIZE_OFFSET; index < DEFAULT_SIZE_OFFSET + DEFAULT_SIZE_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_SIZE_OFFSET + DEFAULT_SIZE_LENGTH - index - 1);
            mBytes[index] = (byte) ((mSize & (0xFF << count)) >> count);
        }
    }

    /**
     * @return Returns requested piece identifier.
     */
    public @NotNull int getPieceId() {
        return mPieceId;
    }

    /**
     * @return Returns offset from beginning of the requested piece.
     */
    public @NotNull int getBeginOffset() {
        return mBegin;
    }

    /**
     * @return Returns length of the requested piece.
     */
    public @NotNull int getSize() {
        return mSize;
    }

    /**
     * Parses 'request' message from byte array (with zero offset from the array beginning).
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentRequest object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If length <= 0.
     */
    public static @Nullable BitTorrentRequest parse(@NotNull final byte[] bytes, @NotNull final int length) throws IllegalArgumentException {
        return parse(bytes, 0, length);
    }

    /**
     * Parses 'request' message from byte array.
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentRequest object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If offset >= length.
     */
    public static @Nullable BitTorrentRequest parse(@NotNull final byte[] bytes, @NotNull int offset, @NotNull final int length) throws IllegalArgumentException {
        if(offset >= length)
            throw new IllegalArgumentException();

        if(length - offset < DEFAULT_MESSAGE_LENGTH)
            return null;

        // Try to parse message type.
        final int lengthByteOffset = offset + 3;
        final int typeByteOffset = offset + 4;
        for(final int limit = offset + DEFAULT_PIECE_ID_OFFSET; offset < limit; ++offset)
            if(offset != lengthByteOffset && offset != typeByteOffset && bytes[offset] != 0x0 ||
               offset == lengthByteOffset && bytes[offset] != DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_PIECE_ID_LENGTH + DEFAULT_BEGIN_LENGTH + DEFAULT_SIZE_LENGTH ||
               offset == typeByteOffset && bytes[offset] != DEFAULT_MESSAGE_TYPE)
                return null;

        // Try to parse identifier of the requested piece.
        int pieceId = 0;
        for(int index = DEFAULT_PIECE_ID_LENGTH - 1; index >= 0; --index)
            pieceId += ((int) bytes[offset + (DEFAULT_PIECE_ID_LENGTH - index - 1)] & 0xFF) << (index * Byte.SIZE);

        offset += DEFAULT_PIECE_ID_LENGTH;

        // Try to parse offset from beginning of the requested piece.
        int begin = 0;
        for(int index = DEFAULT_BEGIN_LENGTH - 1; index >= 0; --index)
            begin += ((int) bytes[offset + (DEFAULT_BEGIN_LENGTH - index - 1)] & 0xFF) << (index * Byte.SIZE);

        offset += DEFAULT_BEGIN_LENGTH;

        // Try to parse length of the requested piece.
        int size = 0;
        for(int index = DEFAULT_SIZE_LENGTH - 1; index >= 0; --index)
            size += ((int) bytes[offset + (DEFAULT_SIZE_LENGTH - index - 1)] & 0xFF) << (index * Byte.SIZE);

        return new BitTorrentRequest(pieceId, begin, size);
    }
}
