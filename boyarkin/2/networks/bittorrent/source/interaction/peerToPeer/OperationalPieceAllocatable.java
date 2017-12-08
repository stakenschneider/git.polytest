package interaction.peerToPeer;

import com.sun.istack.internal.NotNull;

/**
 * @author Boyarkin Nikita.
 */
public interface OperationalPieceAllocatable {
    /**
     * Calls, when new operational piece is allocated.
     * @see OperationalPieces
     * @param pieceId - New operational piece identifier.
     */
    void onOperationPieceAllocated(@NotNull final int pieceId);
}
