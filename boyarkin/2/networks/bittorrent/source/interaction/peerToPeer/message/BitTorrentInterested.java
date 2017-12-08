package interaction.peerToPeer.message;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.util.Arrays;

/**
 * BitTorrent 'interested' message (message type 0x2).
 *
 * [length = 1 [4 bytes]]
 * [message type = 2 [1 byte]]
 *
 * @see BitTorrentMessage
 * @author Boyarkin Nikita.
 */
public class BitTorrentInterested extends BitTorrentMessage {
    /** Static message size of the 'interested' message. */
    public static @NotNull final int DEFAULT_MESSAGE_LENGTH = 0x5;

    /** Length of message type field (in bytes). */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE_LENGTH = 0x1;

    /** Message type value. */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE = 0x2;

    /**
     * Creates new BitTorrentInterested object.
     */
    public BitTorrentInterested() {
        super(new byte[DEFAULT_MESSAGE_LENGTH]);

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
        mBytes[3] = DEFAULT_MESSAGE_TYPE_LENGTH;

        // Set message type field.
        mBytes[4] = DEFAULT_MESSAGE_TYPE;
    }

    /**
     * Parses 'interested' message from byte array (with zero offset from the array beginning).
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentInterested object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If length <= 0.
     */
    public static @Nullable BitTorrentInterested parse(@NotNull final byte[] bytes, @NotNull final int length) throws IllegalArgumentException {
        return parse(bytes, 0, length);
    }

    /**
     * Parses 'interested' message from byte array.
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentInterested object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If offset >= length.
     */
    public static @Nullable BitTorrentInterested parse(@NotNull final byte[] bytes, @NotNull int offset, @NotNull final int length) throws IllegalArgumentException {
        if(offset >= length)
            throw new IllegalArgumentException();

        if(length - offset < DEFAULT_MESSAGE_LENGTH)
            return null;

        // Try to parse message type.
        final int lengthByteOffset = offset + 3;
        final int typeByteOffset = offset + 4;
        for(final int limit = offset + DEFAULT_MESSAGE_LENGTH; offset < limit; ++offset)
            if(offset != lengthByteOffset && offset != typeByteOffset && bytes[offset] != 0x0 ||
               offset == lengthByteOffset && bytes[offset] != DEFAULT_MESSAGE_TYPE_LENGTH ||
               offset == typeByteOffset && bytes[offset] != DEFAULT_MESSAGE_TYPE)
                return null;

        return new BitTorrentInterested();
    }
}
