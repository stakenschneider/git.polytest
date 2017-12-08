package interaction.fileAnalyser;

import com.sun.istack.internal.NotNull;
import interaction.fileAnalyser.FilePartition;

/**
 * @author Boyarkin Nikita.
 */
public interface OnePieceChangeable {
    /**
     * Calls, when one of piece is changed.
     * @see FilePartition
     * @param filePartition - File partition of piece.
     * @param pieceId - Piece identifier.
     */
    void onOnePieceChanged(@NotNull final FilePartition filePartition, @NotNull final int pieceId);
}

