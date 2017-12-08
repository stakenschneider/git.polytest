package interaction.trackerService;

import com.sun.istack.internal.NotNull;

/**
 * @author Boyarkin Nikita.
 */
public interface PeersCountChangeable {
    /**
     * Calls, when one of peer unique sets is changed (changed peers count of all peers).
     * @param peersCount - Count of all peers now.
     */
    void onPeersCountChanged(@NotNull final int peersCount);
}
