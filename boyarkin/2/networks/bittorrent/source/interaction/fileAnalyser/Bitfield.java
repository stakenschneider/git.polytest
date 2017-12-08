package interaction.fileAnalyser;

import com.sun.istack.internal.NotNull;

import javax.xml.bind.DatatypeConverter;


/**
 * Class, that contains bit diagram of completed parts.
 * @author Boyarkin Nikita.
 */
public class Bitfield {

    /**
     * Bit diagram of completed parts.
     * If bit == 1 - > part is already completed.
     * Array length = ceil(piecesCount / 8.f)
     */
    private @NotNull final byte[] mPieces;

    /**
     * Creates empty Bitfield object, based on pieces count.
     * @param piecesCount - Count of pieces into file.
     */
    public Bitfield(@NotNull final int piecesCount) {
        mPieces = new byte[(int) Math.ceil(((float) piecesCount) / Byte.SIZE)];
    }

    /**
     * Creates filled Bitfield object, based on bit diagram.
     * @param bitfieldBytes - Bit diagram of completed parts.
     */
    public Bitfield(@NotNull final byte[] bitfieldBytes) {
        mPieces = bitfieldBytes;
    }

    /**
     * Sets piece into completed state.
     * @param pieceId - Piece identifier.
     */
    public synchronized void setPiece(@NotNull final int pieceId) {
        final int byteId = pieceId / Byte.SIZE;
        final int bitOffset = pieceId % Byte.SIZE;
        mPieces[byteId] = (byte) (mPieces[byteId] | (0x80 >> bitOffset));
    }

    /**
     * @param pieceId - Piece identifier.
     * @return Returns true if piece is completed.
     */
    public synchronized boolean havePiece(@NotNull final int pieceId) {
        final int byteId = pieceId / Byte.SIZE;
        final int bitOffset = pieceId % Byte.SIZE;
        return (mPieces[byteId] & (0x80 >> bitOffset)) != 0;
    }

    /**
     * Merges two bitfield objects (this = this | bitfield)
     * @param bitfield - Bitfield object to merge.
     * @throws IllegalArgumentException - If bitfield.getLength() != this.getLength().
     */
    public synchronized void merge(@NotNull final Bitfield bitfield) throws IllegalArgumentException {
        if(bitfield.getLength() != getLength())
            throw new IllegalArgumentException();

        for(int index = 0; index < mPieces.length; ++index)
            mPieces[index] = (byte) (mPieces[index] | bitfield.getBytes()[index]);
    }

    /**
     * Merges bitfield object with byte array (this = this | bytes)
     * @param bytes - Bytes to merge.
     * @throws IllegalArgumentException - If bytes.length != this.getLength().
     */
    public synchronized void merge(@NotNull final byte[] bytes) throws IllegalArgumentException {
        if(bytes.length != getLength())
            throw new IllegalArgumentException();

        for(int index = 0; index < mPieces.length; ++index)
            mPieces[index] = (byte) (mPieces[index] | bytes[index]);
    }

    /**
     * @return - Returns bit diagram of completed parts.
     */
    public synchronized @NotNull byte[] getBytes() {
        return mPieces;
    }

    /**
     * @return - Returns bit diagram length.
     */
    public synchronized @NotNull int getLength() {
        return mPieces.length;
    }

    /**
     * @return - Returns hexadecimal representation of bit diagram.
     */
    @Override
    public synchronized @NotNull String toString() {
        return DatatypeConverter.printHexBinary(mPieces);
    }
}
