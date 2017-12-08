package interaction.peerToPeer.message;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.util.Arrays;

/**
 * BitTorrent 'piece' message (message type 0x7).
 *
 * [length = 9 + N [4 bytes]]
 * [message type = 7 [1 byte]]
 * [piece identifier [4 bytes]]
 * [offset [4 bytes]]
 * [data [N bytes]]
 *
 * @see BitTorrentMessage
 * @author Boyarkin Nikita.
 */
public class BitTorrentPiece extends BitTorrentMessage {
    /** Length of size field. */
    public static @NotNull final int DEFAULT_MESSAGE_SIZE_LENGTH = 0x4;

    /** Length of message type field (in bytes). */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE_LENGTH = 0x1;

    /** Message type value. */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE = 0x7;

    /** Length of piece identifier field (in bytes). */
    public static @NotNull final int DEFAULT_PIECE_ID_LENGTH = 0x4;

    /** Offset from the beginning of piece identifier field (in bytes). */
    public static @NotNull final int DEFAULT_PIECE_ID_OFFSET = DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH;

    /** Length of piece offset field (in bytes). */
    public static @NotNull final int DEFAULT_BEGIN_LENGTH = 0x4;

    /** Offset from the beginning of piece offset field (in bytes). */
    public static @NotNull final int DEFAULT_BEGIN_OFFSET = DEFAULT_PIECE_ID_OFFSET + DEFAULT_PIECE_ID_LENGTH;

    /**
     * Identifier of the piece.
     */
    private @NotNull final int mPieceId;

    /**
     * Offset from beginning of the piece.
     */
    private @NotNull final int mBegin;

    /**
     * Data of the piece.
     */
    private @NotNull final byte[] mPieceBytes;

    /**
     * Creates new BitTorrentPiece object.
     * @param pieceId - Identifier of the piece.
     * @param begin - Offset from beginning of the piece.
     * @param pieceBytes - Data of the piece.
     */
    public BitTorrentPiece(@NotNull final int pieceId, @NotNull final int begin, @NotNull final byte[] pieceBytes) {
        super(new byte[DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_PIECE_ID_LENGTH + DEFAULT_BEGIN_LENGTH + pieceBytes.length]);

        mPieceId = pieceId;
        mBegin = begin;
        mPieceBytes = pieceBytes;

        initializeBytes();
    }

    /**
     * Initializes mBytes array.
     * @see BitTorrentMessage#initializeBytes()
     */
    @Override
    protected void initializeBytes() {
        Arrays.fill(mBytes, 0, mBytes.length, (byte) 0x0);

        // Set message size field.
        final int size = DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_PIECE_ID_LENGTH + DEFAULT_BEGIN_LENGTH + mPieceBytes.length;
        for(int index = 0; index < DEFAULT_MESSAGE_SIZE_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_MESSAGE_SIZE_LENGTH - index - 1);
            mBytes[index] = (byte) ((size & (0xFF << count)) >> count);
        }

        // Set message type field.
        mBytes[DEFAULT_MESSAGE_SIZE_LENGTH] = DEFAULT_MESSAGE_TYPE;

        // Set identifier of the piece.
        for(int index = DEFAULT_PIECE_ID_OFFSET; index < DEFAULT_PIECE_ID_OFFSET + DEFAULT_PIECE_ID_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_PIECE_ID_OFFSET + DEFAULT_PIECE_ID_LENGTH - index - 1);
            mBytes[index] = (byte) ((mPieceId & (0xFF << count)) >> count);
        }

        // Set offset from beginning of the piece.
        for(int index = DEFAULT_BEGIN_OFFSET; index < DEFAULT_BEGIN_OFFSET + DEFAULT_BEGIN_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_BEGIN_OFFSET + DEFAULT_BEGIN_LENGTH - index - 1);
            mBytes[index] = (byte) ((mBegin & (0xFF << count)) >> count);
        }

        System.arraycopy(mPieceBytes, 0, mBytes, DEFAULT_BEGIN_OFFSET + DEFAULT_BEGIN_LENGTH, mPieceBytes.length);
    }

    /**
     * @return Returns piece identifier.
     */
    public @NotNull int getPieceId() {
        return mPieceId;
    }

    /**
     * @return Returns offset from beginning of the piece.
     */
    public @NotNull int getBeginOffset() {
        return mBegin;
    }

    /**
     * @return Returns data of the piece.
     */
    public @NotNull byte[] getPieceBlock() {
        return mPieceBytes;
    }

    /**
     * Parses 'piece' message from byte array (with zero offset from the array beginning).
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentPiece object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If length <= 0.
     */
    public static @Nullable BitTorrentPiece parse(@NotNull final byte[] bytes, @NotNull final int length) throws IllegalArgumentException {
        return parse(bytes, 0, length);
    }

    /**
     * Parses 'piece' message from byte array.
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentPiece object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If offset >= length.
     */
    public static @Nullable BitTorrentPiece parse(@NotNull final byte[] bytes, @NotNull int offset, @NotNull final int length) throws IllegalArgumentException {
        if(offset >= length)
            throw new IllegalArgumentException();

        // Try to parse message size.
        int size = 0;
        for(int index = DEFAULT_MESSAGE_SIZE_LENGTH - 1; index >= 0; --index) {
            final int byteIndex = offset + (DEFAULT_MESSAGE_SIZE_LENGTH - index - 1);
            if(byteIndex >= length)
                return null;

            size += ((int) bytes[byteIndex] & 0xFF) << (index * Byte.SIZE);
        }

        offset += DEFAULT_MESSAGE_SIZE_LENGTH;

        if(length - offset < size)
            return null;

        if(bytes[offset] != DEFAULT_MESSAGE_TYPE)
            return null;

        offset += DEFAULT_MESSAGE_TYPE_LENGTH;

        // Try to parse piece identifier.
        int pieceId = 0;
        for(int index = DEFAULT_PIECE_ID_LENGTH - 1; index >= 0; --index)
            pieceId += ((int) bytes[offset + (DEFAULT_PIECE_ID_LENGTH - index - 1)] & 0xFF) << (index * Byte.SIZE);

        offset += DEFAULT_PIECE_ID_LENGTH;

        // Try to parse offset from beginning of the piece.
        int begin = 0;
        for(int index = DEFAULT_BEGIN_LENGTH - 1; index >= 0; --index)
            begin += ((int) bytes[offset + (DEFAULT_BEGIN_LENGTH - index - 1)] & 0xFF) << (index * Byte.SIZE);

        offset += DEFAULT_BEGIN_LENGTH;

        // Try to parse data of the piece.
        final byte[] bitfield = new byte[size - DEFAULT_MESSAGE_TYPE_LENGTH - DEFAULT_PIECE_ID_LENGTH - DEFAULT_BEGIN_LENGTH];
        System.arraycopy(bytes, offset, bitfield, 0, bitfield.length);

        return new BitTorrentPiece(pieceId, begin, bitfield);
    }
}
