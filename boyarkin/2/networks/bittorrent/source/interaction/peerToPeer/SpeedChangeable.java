package interaction.peerToPeer;


import com.sun.istack.internal.NotNull;

/**
 * @author Boyarkin Nikita.
 */
public interface SpeedChangeable {
    /**
     * Calls, when uploading or sharing speed is changed.
     * @param uploadingBytes - Uploading speed.
     * @param sharingBytes - Sharing speed.
     */
    void onSpeedChanged(@NotNull final long uploadingBytes, @NotNull final long sharingBytes);
}
