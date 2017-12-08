package interaction.fileAnalyser;

import com.sun.istack.internal.NotNull;

/**
 * @author Boyarkin Nikita.
 */
public interface PiecesPercentChangeable {
    /**
     * Calls, when one of piece is changed (changed percent of all pieces).
     * @param percent - Percent of all pieces.
     */
    void onPiecesPercentChanged(@NotNull final float percent);
}