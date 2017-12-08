package interaction.peerToPeer.message;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.util.Arrays;

/**
 * BitTorrent 'keep alive' message.
 *
 * [length = 0 [4 bytes]]
 *
 * @see BitTorrentMessage
 * @author Boyarkin Nikita.
 */
public class BitTorrentKeepAlive extends BitTorrentMessage {
    /** Static message size of the 'keep alive' message. */
    public static @NotNull final int DEFAULT_MESSAGE_LENGTH = 0x4;

    /**
     * Creates new BitTorrentKeepAlive object.
     */
    public BitTorrentKeepAlive() {
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
    }

    /**
     * Parses 'keep alive' message from byte array (with zero offset from the array beginning).
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentKeepAlive object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If length <= 0.
     */
    public static @Nullable BitTorrentKeepAlive parse(@NotNull final byte[] bytes, @NotNull final int length) throws IllegalArgumentException {
        return parse(bytes, 0, length);
    }

    /**
     * Parses 'keep alive' message from byte array.
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentKeepAlive object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If offset >= length.
     */
    public static @Nullable BitTorrentKeepAlive parse(@NotNull final byte[] bytes, @NotNull int offset, @NotNull final int length) throws IllegalArgumentException {
        if(offset >= length)
            throw new IllegalArgumentException();

        if(length - offset < DEFAULT_MESSAGE_LENGTH)
            return null;

        for(final int limit = offset + DEFAULT_MESSAGE_LENGTH; offset < limit; ++offset)
            if(bytes[offset] != 0x0)
                return null;

        return new BitTorrentKeepAlive();
    }
}
