package interaction.peerToPeer.message;

import interaction.torrentParser.bencode.Bencode;
import interaction.torrentParser.bencode.BencodeMap;
import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.io.*;
import java.util.Arrays;

/**
 * BitTorrent 'extended' message (message type 0x14).
 *
 * [length = 2 + N [4 bytes]]
 * [message type = 20 [1 byte]]
 * [extended type [1 byte]]
 * [extended content (bencode or single byte) [N bytes]]
 *
 * @see BitTorrentMessage
 * @author Boyarkin Nikita.
 */
public class BitTorrentExtended extends BitTorrentMessage {
    /** Static message size of the 'extended' message in empty mode. */
    public static @NotNull final int DEFAULT_MESSAGE_SIZE_LENGTH = 0x4;

    /** Length of message type field (in bytes). */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE_LENGTH = 0x1;

    /** Message type value. */
    public static @NotNull final int DEFAULT_MESSAGE_TYPE = 0x14;

    /** Extended type field length (in bytes). */
    public static @NotNull final int DEFAULT_EXTENDED_TYPE_LENGTH = 0x1;

    /** Extended type value in bencode mode. */
    public static @NotNull final int DEFAULT_EXTENDED_TYPE_BENCODE = 0x0;

    /** Extended default length in bencode mode. */
    public static @NotNull final int DEFAULT_EXTENDED_BENCODE_MAP_LENGTH = 0x2;

    /** Extended type value in empty mode. */
    public static @NotNull final int DEFAULT_EXTENDED_TYPE_EMPTY = 0x3;

    /** Extended default length in empty mode. */
    public static @NotNull final int DEFAULT_EXTENDED_EMPTY_LENGTH = 0x1;

    /** True, if in bencode mode, false if in empty mode. */
    private @NotNull final boolean mBencodeEnabled;

    /** Size of message in bencode mode. */
    private @Nullable final Integer mBencodeSize;

    /** Bencode map of extended message. */
    private @Nullable final BencodeMap mExtendedBencode;

    /** Single byte value in empty mode. */
    private @Nullable final Byte mExtendedByte;

    /**
     * Creates new BitTorrentExtended object in bencode mode, bencode map is empty (TODO).
     */
    public BitTorrentExtended() {
        super(new byte[DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_EXTENDED_TYPE_LENGTH + DEFAULT_EXTENDED_BENCODE_MAP_LENGTH]);

        mBencodeEnabled = true;
        mExtendedBencode = new BencodeMap();
        mBencodeSize = DEFAULT_EXTENDED_BENCODE_MAP_LENGTH;
        mExtendedByte = null;

        initializeBytes();
    }

    /**
     * Creates new BitTorrentExtended object in bencode mode.
     * @param map - Bencode map.
     * @param size - Size of bencode message.
     */
    public BitTorrentExtended(@NotNull final BencodeMap map, @NotNull final int size) {
        super(new byte[DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_EXTENDED_TYPE_LENGTH + size]);

        mBencodeEnabled = true;
        mExtendedBencode = map;
        mBencodeSize = size;
        mExtendedByte = null;

        initializeBytes();
    }

    /**
     * Creates new BitTorrentExtended object in empty mode.
     * @param extendedByte - Single byte value.
     */
    public BitTorrentExtended(@NotNull final byte extendedByte) {
        super(new byte[DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_EXTENDED_TYPE_LENGTH + DEFAULT_EXTENDED_EMPTY_LENGTH]);

        mBencodeEnabled = false;
        mExtendedBencode = null;
        mBencodeSize = null;
        mExtendedByte = extendedByte;

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
        final int size = mBytes.length - DEFAULT_MESSAGE_SIZE_LENGTH;
        for(int index = 0; index < DEFAULT_MESSAGE_SIZE_LENGTH; ++index) {
            final int count = Byte.SIZE * (DEFAULT_MESSAGE_SIZE_LENGTH - index - 1);
            mBytes[index] = (byte) ((size & (0xFF << count)) >> count);
        }

        // Set message type field.
        mBytes[DEFAULT_MESSAGE_SIZE_LENGTH] = DEFAULT_MESSAGE_TYPE;

        // Set extended type field.
        mBytes[DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH] = (byte) ((mBencodeEnabled) ? DEFAULT_EXTENDED_TYPE_BENCODE : DEFAULT_EXTENDED_TYPE_EMPTY);

        if(mBencodeEnabled) {
            // Convert bencode map into byte array.

            final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
            try {
                mExtendedBencode.write(outputStream);
            } catch(final IOException exception) {
                throw new UnknownError();
            }

            System.arraycopy(outputStream.toByteArray(), 0, mBytes, DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_EXTENDED_EMPTY_LENGTH, mBencodeSize);
        }
        else
            mBytes[DEFAULT_MESSAGE_SIZE_LENGTH + DEFAULT_MESSAGE_TYPE_LENGTH + DEFAULT_EXTENDED_EMPTY_LENGTH] = mExtendedByte;
    }

    /**
     * Parses 'extended' message from byte array (with zero offset from the array beginning).
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentExtended object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If length <= 0.
     */
    public static @Nullable BitTorrentExtended parse(@NotNull final byte[] bytes, @NotNull final int length) throws IllegalArgumentException {
        return parse(bytes, 0, length);
    }

    /**
     * Parses 'extended' message from byte array.
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentExtended object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If offset >= length.
     */
    public static @Nullable BitTorrentExtended parse(@NotNull final byte[] bytes, @NotNull int offset, @NotNull final int length) throws IllegalArgumentException {
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

        if(bytes[offset] == DEFAULT_EXTENDED_TYPE_BENCODE) {
            // Bencode mode.

            offset += DEFAULT_EXTENDED_TYPE_LENGTH;

            byte[] extendedBytes = Arrays.copyOfRange(bytes, offset, offset + size - DEFAULT_MESSAGE_TYPE_LENGTH - DEFAULT_EXTENDED_TYPE_LENGTH);

            // Try to parse bencode map.
            BencodeMap bencodeMap;
            final ByteArrayInputStream inputStream = new ByteArrayInputStream(extendedBytes);
            try {
                bencodeMap = (BencodeMap) Bencode.parseBencode(inputStream);
            }
            catch(final IOException | RuntimeException exception) {
                return null;
            }

            return new BitTorrentExtended(bencodeMap, extendedBytes.length);
        }
        else if(bytes[offset] == DEFAULT_EXTENDED_TYPE_EMPTY) {
            // Empty mode.

            offset += DEFAULT_EXTENDED_TYPE_LENGTH;

            return new BitTorrentExtended(bytes[offset]);
        }

        return null;
    }
}
