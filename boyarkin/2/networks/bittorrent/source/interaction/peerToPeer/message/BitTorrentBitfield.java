package interaction.peerToPeer.message;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.util.Arrays;

/**
 * BitTorrent 'bitfield' message (message type 0x5).
 *
 * [length = 1 + N [4 bytes]]
 * [message type = 5 [1 byte]]
 * [bitfield [N bytes]]
 *
 * @see BitTorrentMessage
 * @author Boyarkin Nikita.
 */
public class BitTorrentBitfield extends BitTorrentMessage {
    /** Static message size of the 'bitfield' message. */
    public static @NotNull final int DEFAULT_MESSAGE_SIZE_LENGTH = 0x4;

    /** Length of message type field (in bytes). */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE_LENGTH = 0x1;

    /** Message type value. */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE = 0x5;

    /**
     * Bit diagram of completed parts.
     * @see interaction.fileAnalyser.Bitfield
     */
    private @NotNull final byte[] mBitfield;

    /**
     * Creates new BitTorrentBitfield object.
     * @param bitfield - Bit diagram of completed parts.
     */
    public BitTorrentBitfield(@NotNull final byte[] bitfield) {
        super(new byte[DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH + bitfield.length]);

        mBitfield = bitfield;

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
        final int size = DEFAULT_MESSAGE_TYPE_LENGTH + mBitfield.length;
        for(int index = 0; index < DEFAULT_MESSAGE_SIZE_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_MESSAGE_SIZE_LENGTH - index - 1);
            mBytes[index] = (byte) ((size & (0xFF << count)) >> count);
        }

        // Set message type field.
        mBytes[DEFAULT_MESSAGE_SIZE_LENGTH] = DEFAULT_MESSAGE_TYPE;

        // Set bit diagram of completed parts.
        System.arraycopy(mBitfield, 0, mBytes, DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH, mBitfield.length);
    }

    /**
     * @return Returns bit diagram of completed parts.
     */
    public @NotNull byte[] getBitfieldBytes() {
        return mBitfield;
    }

    /**
     * Parses 'bitfield' message from byte array (with zero offset from the array beginning).
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentBitfield object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If length <= 0.
     */
    public static @Nullable BitTorrentBitfield parse(@NotNull final byte[] bytes, @NotNull final int length) throws IllegalArgumentException {
        return parse(bytes, 0, length);
    }

    /**
     * Parses 'bitfield' message from byte array.
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentBitfield object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If offset >= length.
     */
    public static @Nullable BitTorrentBitfield parse(@NotNull final byte[] bytes, @NotNull int offset, @NotNull final int length) throws IllegalArgumentException {
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

        // Try to parse message type.
        if(bytes[offset] != DEFAULT_MESSAGE_TYPE)
            return null;

        offset += DEFAULT_MESSAGE_TYPE_LENGTH;

        // Try to parse bit diagram of completed parts.
        final byte[] bitfield = new byte[size - DEFAULT_MESSAGE_TYPE_LENGTH];
        System.arraycopy(bytes, offset, bitfield, 0, bitfield.length);

        return new BitTorrentBitfield(bitfield);
    }
}
