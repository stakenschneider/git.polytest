package interaction;


import com.sun.istack.internal.NotNull;
import util.Log;
import util.Util;

import java.util.HashSet;
import java.util.Set;

/**
 * Singleton class, that reserves port numbers for interactions.
 * @author Boyarkin Nikita.
 */
public class PortCreator {
    /** Start value for port allocating. */
    public static int PEER_START_PORT = 6881;

    /** Singleton instance. */
    private static @NotNull volatile PortCreator sInstance;

    /** Returns PortCreator instance. */
    public static PortCreator getInstance() {
        PortCreator localInstance = sInstance;
        if (localInstance == null) {
            synchronized (PortCreator.class) {
                localInstance = sInstance;

                if (localInstance == null)
                    sInstance = localInstance = new PortCreator();
            }
        }

        return localInstance;
    }

    /** Unique set of busy ports. */
    private final Set<Integer> mPorts;

    /**
     * Creates PortCreator object. Calls only into getInstance() method.
     */
    private PortCreator() {
        mPorts = new HashSet<>();
    }

    /**
     * @return Reserved port number.
     * @throws UnknownError If impossible to reserve port (all ports are busy, very strange situation).
     */
    public synchronized int reserveNewPort() throws UnknownError {
        int resultPort = PEER_START_PORT - 1;

        do {
            ++resultPort;

            if(resultPort >= 0x10000)
                throw new UnknownError();
        }
        while(!Util.isPortAvailable(resultPort) || mPorts.contains(resultPort));

        mPorts.add(resultPort);
        Log.d("Reserved new port number '" + resultPort + "', count of reserved ports is '" + mPorts.size() + "' now.");

        return resultPort;
    }

    /**
     * Removes reserved port from unique list.
     * @param port - Port for removing.
     */
    public synchronized void removeExistingPort(@NotNull final int port) {
        if(!mPorts.contains(port))
            return;

        mPorts.remove(port);
    }
}
