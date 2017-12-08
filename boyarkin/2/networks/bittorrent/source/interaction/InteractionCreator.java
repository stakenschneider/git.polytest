package interaction;

import com.sun.istack.internal.NotNull;
import gui.Controller;
import util.Log;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

/**
 * Singleton class, that creates new Interaction objects, which starts into new threads.
 * @see Interaction
 * @author Boyarkin Nikita.
 */
public class InteractionCreator {
    /** Singleton instance. */
    private static volatile @NotNull InteractionCreator sInstance;

    /** Returns InteractionCreator instance. */
    public static InteractionCreator getInstance() {
        InteractionCreator localInstance = sInstance;
        if (localInstance == null) {
            synchronized (InteractionCreator.class) {
                localInstance = sInstance;

                if (localInstance == null)
                    sInstance = localInstance = new InteractionCreator();
            }
        }

        return localInstance;
    }

    /** Map, that contains all threads and interactions. */
    private @NotNull final Map<Thread, Interaction> mInteractionThreads;

    /**
     * Creates InteractionCreator object. Calls only into getInstance() method.
     */
    private InteractionCreator() {
        mInteractionThreads = new HashMap<>();
    }

    /**
     * Creates new Interaction and starts it in new thread.
     * @see Interaction
     * @param context - Controller class (context).
     * @see Controller
     * @param torrentFile - File descriptor of .torrent file.
     */
    public synchronized void createNewInteraction(@NotNull final Controller context, @NotNull final File torrentFile) {
        final int peerPort = PortCreator.getInstance().reserveNewPort();

        final Interaction interaction = new Interaction(context, torrentFile, peerPort);
        final Thread thread = new Thread(() -> {
            try {
                interaction.startInteraction();
            }
            catch(final InterruptedException exception) {
                // If thread is interrupted, not need to remove it from hash map, cuz it already removed.
                interaction.clearResources();
                PortCreator.getInstance().removeExistingPort(peerPort);

                Log.e("Thread is interrupted.");
            }
        });

        Log.i("Torrent file is recognized, try to start interaction.");

        mInteractionThreads.put(thread, interaction);
        thread.setDaemon(false);
        thread.start();
    }

    /**
     * Stops all timer tasks and correctly finishes all interactions.
     */
    public synchronized void interruptAllThreads() {
        for(final HashMap.Entry<Thread, Interaction> currentPair: mInteractionThreads.entrySet()) {
            currentPair.getKey().interrupt();

            try {
                currentPair.getKey().join();
            }
            catch(final InterruptedException exception) { }
        }

        mInteractionThreads.clear();
    }
}
