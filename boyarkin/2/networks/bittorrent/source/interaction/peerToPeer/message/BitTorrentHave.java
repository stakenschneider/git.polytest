package interaction.peerToPeer.message;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.util.Arrays;

/**
 * BitTorrent 'have' message (message type 0x4).
 *
 * [length = 5 [4 bytes]]
 * [message type = 4 [1 byte]]
 * [piece identifier [4 bytes]]
 *
 * @see BitTorrentMessage
 * @author Boyarkin Nikita.
 */
public class BitTorrentHave extends BitTorrentMessage {
    /** Static message size of the 'have' message. */
    public static @NotNull final int DEFAULT_MESSAGE_LENGTH = 0x9;

    /** Length of message type field (in bytes). */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE_LENGTH = 0x1;

    /** Message type value. */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE = 0x4;

    /** Length of piece identifier field (in bytes). */
    public static @NotNull final int DEFAULT_PIECE_ID_LENGTH = 0x4;

    /** Offset from the beginning of piece identifier field (in bytes). */
    public static @NotNull final int DEFAULT_PIECE_ID_OFFSET = DEFAULT_MESSAGE_LENGTH - DEFAULT_PIECE_ID_LENGTH;

    /**
     * Identifier of the cancelled piece.
     */
    private @NotNull final int mPieceId;

    /**
     * Creates new BitTorrentHave object.
     * @param pieceId - Identifier of the piece.
     */
    public BitTorrentHave(@NotNull final int pieceId) {
        super(new byte[DEFAULT_MESSAGE_LENGTH]);

        mPieceId = pieceId;

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
        mBytes[3] = DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_PIECE_ID_LENGTH;

        // Set message type field.
        mBytes[4] = DEFAULT_MESSAGE_TYPE;

        // Set identifier of the piece.
        for(int index = DEFAULT_PIECE_ID_OFFSET; index < DEFAULT_MESSAGE_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_MESSAGE_LENGTH - index - 1);
            mBytes[index] = (byte) ((mPieceId & (0xFF << count)) >> count);
        }
    }

    /**
     * @return Returns piece identifier.
     */
    public @NotNull int getPieceId() {
        return mPieceId;
    }

    /**
     * Parses 'have' message from byte array (with zero offset from the array beginning).
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentHave object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If length <= 0.
     */
    public static @Nullable BitTorrentHave parse(@NotNull final byte[] bytes, @NotNull final int length) throws IllegalArgumentException {
        return parse(bytes, 0, length);
    }

    /**
     * Parses 'have' message from byte array.
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentHave object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If offset >= length.
     */
    public static @Nullable BitTorrentHave parse(@NotNull final byte[] bytes, @NotNull int offset, @NotNull final int length) throws IllegalArgumentException {
        if(offset >= length)
            throw new IllegalArgumentException();

        if(length - offset < DEFAULT_MESSAGE_LENGTH)
            return null;

        // Try to parse message type.
        final int lengthByteOffset = offset + 3;
        final int typeByteOffset = offset + 4;
        for(final int limit = offset + DEFAULT_PIECE_ID_OFFSET; offset < limit; ++offset)
            if(offset != lengthByteOffset && offset != typeByteOffset && bytes[offset] != 0x0 ||
               offset == lengthByteOffset && bytes[offset] != DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_PIECE_ID_LENGTH ||
               offset == typeByteOffset && bytes[offset] != DEFAULT_MESSAGE_TYPE)
                return null;

        // Try to parse identifier of the piece.
        int pieceId = 0;
        for(int index = DEFAULT_PIECE_ID_LENGTH - 1; index >= 0; --index)
            pieceId += ((int) bytes[offset + (DEFAULT_PIECE_ID_LENGTH - index - 1)] & 0xFF) << (index * Byte.SIZE);

        return new BitTorrentHave(pieceId);
    }
}
