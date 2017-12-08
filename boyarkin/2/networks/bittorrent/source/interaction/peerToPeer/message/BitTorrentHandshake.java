package interaction.peerToPeer.message;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import util.SHA1;

import javax.xml.bind.DatatypeConverter;
import java.util.Arrays;

/**
 * BitTorrent 'handshake' message.
 *
 * [protocol name length = 19 [1 byte]]
 * [protocol name = 'BitTorrent protocol' [19 bytes]]
 * [extension bytes [8 bytes]]
 * [info hash [20 bytes]]
 * [peer identifier [20 bytes]]
 *
 * @see BitTorrentMessage
 * @author Boyarkin Nikita.
 */
public class BitTorrentHandshake extends BitTorrentMessage {
    /** Static message size of the 'handshake' message. */
    public static @NotNull final int DEFAULT_MESSAGE_LENGTH = 0x44;

    /** Default protocol name. */
    public static @NotNull final String DEFAULT_PROTOCOL_NAME = "BitTorrent protocol";

    /** Default extension bytes. */
    public static @NotNull final byte[] DEFAULT_EXTENSION_BYTES = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    // TODO public static @NotNull final byte[] DEFAULT_EXTENSION_BYTES = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x05 };

    /** Default peer identifier length. */
    public static @NotNull final int DEFAULT_PEER_ID_LENGTH = 0x14;

    /**
     * Sha-1 hash of 'info' dictionary.
     * @see interaction.torrentParser.TorrentParser
     */
    private @NotNull final SHA1 mInfoHash;

    /**
     * Extension bytes.
     * @see BitTorrentExtended
     */
    private @NotNull final byte[] mExtensionBytes;

    /**
     * Peer identifier.
     * @see interaction.trackerService.TrackerService
     */
    private @NotNull final byte[] mPeerId;

    /**
     * Creates new BitTorrentHandshake object.
     * @param infoHash - Sha-1 hash of 'info' dictionary.
     * @param peerId - String peer identifier.
     */
    public BitTorrentHandshake(@NotNull final SHA1 infoHash, @NotNull final String peerId) {
        super(new byte[DEFAULT_MESSAGE_LENGTH]);

        mInfoHash = infoHash;
        mExtensionBytes = DEFAULT_EXTENSION_BYTES.clone();
        mPeerId = DatatypeConverter.parseHexBinary(peerId);

        initializeBytes();
    }

    /**
     * Creates new BitTorrentHandshake object.
     * @param infoHash - Sha-1 hash of 'info' dictionary.
     * @param peerId - Byte array of peer identifier.
     */
    public BitTorrentHandshake(@NotNull final SHA1 infoHash, @NotNull final byte[] peerId) {
        super(new byte[DEFAULT_MESSAGE_LENGTH]);

        mInfoHash = infoHash;
        mExtensionBytes = DEFAULT_EXTENSION_BYTES.clone();
        mPeerId = peerId.clone();

        initializeBytes();
    }

    /**
     * Creates new BitTorrentHandshake object.
     * @param infoHash - Sha-1 hash of 'info' dictionary.
     * @param peerId - String peer identifier.
     * @param extensionBytes - Byte array of extension bytes.
     */
    public BitTorrentHandshake(@NotNull final SHA1 infoHash, @NotNull final String peerId, @NotNull final byte[] extensionBytes) {
        super(new byte[DEFAULT_MESSAGE_LENGTH]);

        mInfoHash = infoHash;
        mExtensionBytes = extensionBytes.clone();
        mPeerId = DatatypeConverter.parseHexBinary(peerId);

        initializeBytes();
    }

    /**
     * Creates new BitTorrentHandshake object.
     * @param infoHash - Sha-1 hash of 'info' dictionary.
     * @param peerId - Byte array of peer identifier.
     * @param extensionBytes - Byte array of extension bytes.
     */
    public BitTorrentHandshake(@NotNull final SHA1 infoHash, @NotNull final byte[] peerId, @NotNull final byte[] extensionBytes) {
        super(new byte[DEFAULT_MESSAGE_LENGTH]);

        mInfoHash = infoHash;
        mExtensionBytes = extensionBytes.clone();
        mPeerId = peerId.clone();

        initializeBytes();
    }

    /**
     * Initializes mBytes array.
     * @see BitTorrentMessage#initializeBytes()
     */
    @Override
    protected void initializeBytes() {
        if(mPeerId.length != DEFAULT_PEER_ID_LENGTH)
            throw new IllegalArgumentException();

        int offset = 0;

        // Set protocol name size.
        mBytes[offset] = (byte) DEFAULT_PROTOCOL_NAME.length();

        // Set protocol name.
        System.arraycopy(DEFAULT_PROTOCOL_NAME.getBytes(), 0, mBytes, ++offset, DEFAULT_PROTOCOL_NAME.length());

        // Set extension bytes.
        System.arraycopy(mExtensionBytes, 0, mBytes, offset += DEFAULT_PROTOCOL_NAME.length(), mExtensionBytes.length);

        // Set sha-1 hash of info dictionary,
        System.arraycopy(mInfoHash.toBytes(), 0, mBytes, offset +=  mExtensionBytes.length, SHA1.LENGTH);

        // Set peer identifier.
        System.arraycopy(mPeerId, 0, mBytes, offset + SHA1.LENGTH, mPeerId.length);
    }

    /**
     * @return Returns sha-1 hash of info dictionary.
     */
    public @NotNull SHA1 getInfoHash() {
        return mInfoHash;
    }

    /**
     * @return Returns peer identifier.
     */
    public @NotNull String getStringPeerId() {
        return DatatypeConverter.printHexBinary(mPeerId);
    }

    /**
     * Parses 'handshake' message from byte array (with zero offset from the array beginning).
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentHandshake object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If length <= 0.
     */
    public static @Nullable BitTorrentHandshake parse(@NotNull final byte[] bytes, @NotNull final int length) throws IllegalArgumentException {
        return parse(bytes, 0, length);
    }

    /**
     * Parses 'handshake' message from byte array.
     * @see BitTorrentMessage#parseAll(byte[], int)
     * @param bytes - Byte array for parsing.
     * @param length - Length of bytes into array to parse.
     * @return Returns new BitTorrentHandshake object or null, if impossible to parse this object.
     * @throws IllegalArgumentException If offset >= length.
     */
    public static @Nullable BitTorrentHandshake parse(@NotNull final byte[] bytes, @NotNull int offset, @NotNull final int length) throws IllegalArgumentException {
        if(offset >= length)
            throw new IllegalArgumentException();

        if(length - offset < DEFAULT_MESSAGE_LENGTH)
            return null;

        if(bytes[offset++] != DEFAULT_PROTOCOL_NAME.length())
            return null;

        // Try to parse protocol name.
        byte[] temp = Arrays.copyOfRange(bytes, offset, offset + DEFAULT_PROTOCOL_NAME.length());
        if(!Arrays.equals(temp, DEFAULT_PROTOCOL_NAME.getBytes()))
            return null;

        // Try to parse extension bytes.
        offset += DEFAULT_PROTOCOL_NAME.length();
        temp = Arrays.copyOfRange(bytes, offset, offset + DEFAULT_EXTENSION_BYTES.length);

        // Try to parse sha-1 hash of 'info' dictionary.
        offset += temp.length;
        final SHA1 infoHash = new SHA1(Arrays.copyOfRange(bytes, offset, offset + SHA1.LENGTH));

        // Try to parse peer identifier.
        offset += SHA1.LENGTH;
        final byte[] peerId = Arrays.copyOfRange(bytes, offset, offset + DEFAULT_PEER_ID_LENGTH);

        return new BitTorrentHandshake(infoHash, peerId, temp);
    }
}
