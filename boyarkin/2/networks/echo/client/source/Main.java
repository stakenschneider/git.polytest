import java.io.IOException;
import java.net.InetAddress;

class Main {
    public static void main(String[] args) {
        try {
            Client server = new Client(InetAddress.getByName(Constants.IP), Constants.PORT, null, null,null);
            server.start();
        }
        catch (final IOException exception) {
            System.err.print(exception.getMessage());
        }
    }
}