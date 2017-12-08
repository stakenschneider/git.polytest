package interaction.trackerService;

import com.sun.istack.internal.NotNull;

import java.util.Set;

/**
 * @author Boyarkin Nikita.
 */
public interface OnePeerChangeable {
    /**
     * Calls, when one of peer unique sets is changed.
     * @see TrackerInfo
     * @param trackerInfo - TrackerInfo object, that called function.
     * @param peers - Unique set of peers, that changed.
     */
    void onOnePeerChanged(@NotNull final TrackerInfo trackerInfo, @NotNull final Set<Peer> peers);
}


