package util;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import javax.xml.bind.DatatypeConverter;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Class for SHA-1 hash.
 * @author Boyarkin Nikita.
 */
public class SHA1 implements Comparable<SHA1> {
    /** SHA-1 length (in bytes). */
    public static final int LENGTH = 0x14;

    /** Byte representation of SHA-1 hash. Length of this array must be SHA1.LENGTH. */
    private @NotNull byte[] mHashBytes;

    /** Hexadecimal representation of SHA-1 hash. Length of this string must be 2 * SHA1.LENGTH. */
    private @NotNull String mHashString;

    /**
     * Creates SHA1 object.
     * @param hexadecimalString - Hexadecimal representation of SHA-1 hash. Length of this string must be 2 * SHA1.LENGTH.
     * @throws IllegalArgumentException If length of hexadecimal string not equals 2 * SHA1.LENGTH.
     */
    public SHA1(@NotNull final String hexadecimalString) {
        // Hexadecimal string contains twice as much characters, than bytes count.
        if(hexadecimalString.length() != LENGTH * 2)
            throw new IllegalArgumentException("SHA-1 length must be 20 bytes.");

        mHashBytes = DatatypeConverter.parseHexBinary(hexadecimalString);
        mHashString = hexadecimalString.toUpperCase();
    }

    /**
     * Creates SHA1 object.
     * @param hashBytes - Byte representation of SHA-1 hash. Length of this array must be SHA1.LENGTH.
     * @throws IllegalArgumentException If length of byte array not equals SHA1.LENGTH.
     */
    public SHA1(@NotNull final byte[] hashBytes) {
        if(hashBytes.length != LENGTH)
            throw new IllegalArgumentException("SHA-1 length must be 20 bytes.");

        mHashString = DatatypeConverter.printHexBinary(hashBytes).toUpperCase();
        mHashBytes = hashBytes.clone();
    }

    /**
     * @return Byte representation of SHA-1 hash.
     */
    public @NotNull byte[] toBytes() {
        return mHashBytes;
    }

    /**
     * @return Hexadecimal representation of SHA-1 hash.
     */
    @Override
    public @NotNull String toString() {
        return mHashString;
    }

    /**
     * @return Comparison of two SHA1 objects.
     * @throws NullPointerException If argument is null.
     */
    @Override
    public int compareTo(@Nullable final SHA1 value) {
        if(value == null)
            throw new NullPointerException();

        return mHashString.compareTo(value.mHashString);
    }

    /**
     * @return Returns true if objects are equals.
     */
    @Override
    public boolean equals(@Nullable final Object value) {
        if(!(value instanceof SHA1))
            return false;

        return value == this || compareTo((SHA1) value) == 0;
    }

    @Override
    public int hashCode() {
        return mHashString.hashCode();
    }

    /**
     * @param dataBytes - Byte array, from which SHA-1 is generated.
     * @return SHA1 object.
     * @throws NoSuchAlgorithmException If SHA-1 algorithm isn't contains.
     */
    public static @NotNull SHA1 generateSHA1(@NotNull final byte[] dataBytes) throws NoSuchAlgorithmException {
        final MessageDigest messageDigest = MessageDigest.getInstance("SHA1");
        final byte[] result = messageDigest.digest(dataBytes);
        return new SHA1(result);
    }
}
