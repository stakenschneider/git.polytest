package interaction.peerToPeer.message;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import javafx.util.Pair;
import org.apache.commons.lang.ArrayUtils;

import javax.xml.bind.DatatypeConverter;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Abstract BitTorrent message.
 * @author Boyarkin Nikita.
 */
public abstract class BitTorrentMessage {
    /**
     * All bytes of the message.
     */
    protected @NotNull final byte[] mBytes;

    /**
     * Creates new BitTorrentMessage object.
     * @param bytes - New byte array.
     */
    protected BitTorrentMessage(@NotNull final byte[] bytes) {
        mBytes = bytes;
    }

    /**
     * Special function for initialize mBytes array.
     */
    protected abstract void initializeBytes();

    /**
     * @return Returns length of message.
     */
    public @NotNull int getLength() {
        return mBytes.length;
    }

    /**
     * @return Returns bytes of message.
     */
    public @NotNull byte[] getBytes() {
        return mBytes;
    }

    /**
     * @return Returns string representation of message with default charset (ISO-8859-1).
     */
    @Override
    public @NotNull String toString() {
        return new String(mBytes);
    }

    /**
     * @param charset - Charset.
     * @return Returns string representation of message with definite charset.
     */
    public @NotNull String toString(@NotNull final Charset charset) {
        return new String(mBytes, charset);
    }

    /**
     * @return String hexadecimal representation of message.
     */
    public @NotNull String toHexadecimalString() {
        return DatatypeConverter.printHexBinary(mBytes);
    }

    /**
     * @param value - Other message.
     * @return Returns true, if two messages are equals.
     */
    @Override
    public @NotNull boolean equals(@Nullable final Object value) {
        if(!(value instanceof BitTorrentMessage))
            return false;

        return value == this || Arrays.equals(mBytes, ((BitTorrentMessage) value).mBytes);
    }

    /**
     * @return Returns unique hash code for message.
     */
    @Override
    public @NotNull int hashCode() {
        return Arrays.hashCode(mBytes);
    }

    /**
     * Unites several messages into one.
     * @param messages - Messages list.
     * @return Byte array, that contains all messages.
     */
    public static @Nullable byte[] constructAll(@NotNull final List<BitTorrentMessage> messages) {
        byte[] result = null;

        for(final BitTorrentMessage currentMessage: messages)
            result = ArrayUtils.addAll(result, currentMessage.getBytes());

        return result;
    }

    /**
     * Parses byte array and creates recognised message list from it.
     * @param bytes - Byte array for parsing.
     * @param length - Count of bytes into array for parsing.
     * @return Returns list of recognized messages.
     */
    public static @NotNull Pair<List<BitTorrentMessage>, Integer> parseAll(@NotNull final byte[] bytes, @NotNull final int length) {
        final List<BitTorrentMessage> messages = new ArrayList<>();

        BitTorrentMessage message;

        int offset = 0;
        while(offset < length) {
            message = BitTorrentKeepAlive.parse(bytes, offset, length);

            if(message == null) message = BitTorrentHandshake.parse(bytes, offset, length);

            if(message == null) message = BitTorrentChoke.parse(bytes, offset, length);

            if(message == null) message = BitTorrentUnchoke.parse(bytes, offset, length);

            if(message == null) message = BitTorrentInterested.parse(bytes, offset, length);

            if(message == null) message = BitTorrentNotInterested.parse(bytes, offset, length);

            if(message == null) message = BitTorrentHave.parse(bytes, offset, length);

            if(message == null) message = BitTorrentBitfield.parse(bytes, offset, length);

            if(message == null) message = BitTorrentRequest.parse(bytes, offset, length);

            if(message == null) message = BitTorrentPiece.parse(bytes, offset, length);

            if(message == null) message = BitTorrentCancel.parse(bytes, offset, length);

            if(message == null) message = BitTorrentPort.parse(bytes, offset, length);

            if(message == null) message = BitTorrentExtended.parse(bytes, offset, length);

            if(message != null) {
                messages.add(message);
                offset += message.getLength();
            }
            else
                return new Pair<>(messages, length - offset);
        }

        return new Pair<>(messages, 0);
    }
}
