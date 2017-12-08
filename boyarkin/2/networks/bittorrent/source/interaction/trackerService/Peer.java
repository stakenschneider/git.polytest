package interaction.trackerService;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;

import java.net.Socket;

/**
 * Class, that contains information about the peer (IP + port).
 * @author Boyarkin Nikita.
 */
public class Peer {
    /** IP address of the peer in format 'x.x.x.x', where x - integer number from 0 to 255. */
    public @NotNull final String ip;

    /** Port number of the peer with limits from 0 to 65536. */
    public @NotNull final Integer port;

    /**
     * Creates Peer object.
     * @param ip - IP address of the peer in format 'x.x.x.x', where x - integer number from 0 to 255.
     * @param port - Port number of the peer with limits from 0 to 65536.
     */
    public Peer(@NotNull final String ip, @NotNull final int port) {
        this.ip = ip;
        this.port = port;
    }

    /**
     * Creates Peer object based on socket.
     * @param socket - Peer socket.
     * @return Peer object.
     */
    public static @NotNull Peer getPeerFromSocket(@NotNull final Socket socket) {
        return new Peer(socket.getInetAddress().getHostAddress(), socket.getPort());
    }

    /**
     * @return String concatenation of ip address and port.
     */
    @Override
    public @NotNull String toString() {
        return "/" + ip + ":" + port;
    }

    /**
     * @return Returns hash code of the object.
     */
    @Override
    public int hashCode() {
        return ip.hashCode() + port.hashCode();
    }

    /**
     * @param value - Other object.
     * @return True if objects are equal.
     */
    @Override
    public boolean equals(@Nullable final Object value) {
        if(!(value instanceof Peer))
            return false;

        return value == this || ((Peer) value).ip.equals(ip) && ((Peer) value).port.equals(port);
    }
}
