package interaction.peerToPeer.message;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.util.Arrays;

/**
 * BitTorrent 'port' message (message type 0x9).
 *
 * [length = 3 [4 bytes]]
 * [message type = 9 [1 byte]]
 * [port number [2 bytes]]
 *
 * @see BitTorrentMessage
 * @author Boyarkin Nikita.
 */
public class BitTorrentPort extends BitTorrentMessage {
    /** Static message size of the 'port' message. */
    public static @NotNull final int DEFAULT_MESSAGE_LENGTH = 0x7;

    /** Length of message type field (in bytes). */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE_LENGTH = 0x1;

    /** Message type value. */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE = 0x9;

    /** Length of port field (in bytes). */
    public static @NotNull final int DEFAULT_PORT_LENGTH = 0x2;

    /** Offset from the beginning of port field (in bytes). */
    public static @NotNull final int DEFAULT_PORT_OFFSET = DEFAULT_MESSAGE_LENGTH - DEFAULT_PORT_LENGTH;

    /**
     * Port number (0 - 65535).
     */
    private @NotNull final int mPort;

    /**
     * Creates new BitTorrentPort object.
     * @param port - Port number (0 - 65535).
     */
    public BitTorrentPort(@NotNull final int port) {
        super(new byte[DEFAULT_MESSAGE_LENGTH]);

        mPort = port;

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
        mBytes[3] = DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_PORT_LENGTH;

        // Set message type field.
        mBytes[4] = DEFAULT_MESSAGE_TYPE;

        // Set port number (0 - 65535).
        for(int index = DEFAULT_PORT_OFFSET; index < DEFAULT_MESSAGE_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_MESSAGE_LENGTH - index - 1);
            mBytes[index] = (byte) ((mPort & (0xFF << count)) >> count);
        }
    }

    /**
     * Parses 'port' message from byte array (with zero offset from the array beginning).
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentPort object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If length <= 0.
     */
    public static @Nullable BitTorrentPort parse(@NotNull final byte[] bytes, @NotNull final int length) throws IllegalArgumentException {
        return parse(bytes, 0, length);
    }

    /**
     * Parses 'port' message from byte array.
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentPort object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If offset >= length.
     */
    public static @Nullable BitTorrentPort parse(@NotNull final byte[] bytes, @NotNull int offset, @NotNull final int length) throws IllegalArgumentException {
        if(offset >= length)
            throw new IllegalArgumentException();

        if(length - offset < DEFAULT_MESSAGE_LENGTH)
            return null;

        // Try to parse message type.
        final int lengthByteOffset = offset + 3;
        final int typeByteOffset = offset + 4;
        for(final int limit = offset + DEFAULT_PORT_OFFSET; offset < limit; ++offset)
            if(offset != lengthByteOffset && offset != typeByteOffset && bytes[offset] != 0x0 ||
               offset == lengthByteOffset && bytes[offset] != DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_PORT_LENGTH ||
               offset == typeByteOffset && bytes[offset] != DEFAULT_MESSAGE_TYPE)
                return null;

        // Try to parse port number.
        int port = 0;
        for(int index = DEFAULT_PORT_LENGTH - 1; index >= 0; --index)
            port += ((int) bytes[offset + (DEFAULT_PORT_LENGTH - index - 1)] & 0xFF) << (index * Byte.SIZE);

        return new BitTorrentPort(port);
    }
}
