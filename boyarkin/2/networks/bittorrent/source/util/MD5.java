package util;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import javax.xml.bind.DatatypeConverter;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Class for MD5 hash.
 * @author Boyarkin Nikita.
 */
public class MD5 implements Comparable<MD5> {
    /** MD5 length (in bytes). */
    public static final int LENGTH = 0x10;

    /** Byte representation of MD5 hash. Length of this array must be MD5.LENGTH. */
    private @NotNull byte[] mHashBytes;

    /** Hexadecimal representation of MD5 hash. Length of this string must be 2 * MD5.LENGTH. */
    private @NotNull String mHashString;

    /**
     * Creates MD5 object.
     * @param hexadecimalString - Hexadecimal representation of MD5 hash. Length of this string must be 2 * MD5.LENGTH.
     * @throws IllegalArgumentException If length of hexadecimal string not equals 2 * MD5.LENGTH.
     */
    public MD5(@NotNull final String hexadecimalString) {
        // Hexadecimal string contains twice as much characters, than bytes count.
        if(hexadecimalString.length() != LENGTH * 2)
            throw new IllegalArgumentException("MD5 length must be 16 bytes.");

        mHashBytes = DatatypeConverter.parseHexBinary(hexadecimalString);
        mHashString = hexadecimalString.toUpperCase();
    }

    /**
     * Creates MD5 object.
     * @param hashBytes - Byte representation of MD5 hash. Length of this array must be MD5.LENGTH.
     * @throws IllegalArgumentException If length of byte array not equals MD5.LENGTH.
     */
    public MD5(@NotNull final byte[] hashBytes) {
        if(hashBytes.length != LENGTH)
            throw new IllegalArgumentException("MD5 length must be 16 bytes.");

        mHashString = DatatypeConverter.printHexBinary(hashBytes).toUpperCase();
        mHashBytes = hashBytes.clone();
    }

    /**
     * @return Byte representation of MD5 hash.
     */
    public @NotNull byte[] toBytes() {
        return mHashBytes;
    }

    /**
     * @return Hexadecimal representation of MD5 hash.
     */
    @Override
    public @NotNull String toString() {
        return mHashString;
    }

    /**
     * @return Comparison of two MD5 objects.
     * @throws NullPointerException If argument is null.
     */
    @Override
    public int compareTo(@Nullable final MD5 value) {
        if(value == null)
            throw new NullPointerException();

        return mHashString.compareTo(value.mHashString);
    }

    /**
     * @return Returns true if objects are equals.
     */
    @Override
    public boolean equals(@Nullable final Object value) {
        if(!(value instanceof MD5))
            return false;

        return value == this || compareTo((MD5) value) == 0;
    }

    @Override
    public int hashCode() {
        return mHashString.hashCode();
    }

    /**
     * @param dataBytes - Byte array, from which MD5 is generated.
     * @return MD5 object.
     * @throws NoSuchAlgorithmException If MD5 algorithm isn't contains.
     */
    public static @NotNull MD5 generateMD5(@NotNull final byte[] dataBytes) throws NoSuchAlgorithmException {
        final MessageDigest messageDigest = MessageDigest.getInstance("MD5");
        final byte[] result = messageDigest.digest(dataBytes);
        return new MD5(result);
    }
}
