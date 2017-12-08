import java.io.IOException;

class Main {
    public static void main(String[] args) {
        try {
            Server server = new Server(Constants.PORT, null, null,null);
            server.start();
        }
        catch (final IOException exception) {
            System.err.print(exception.getMessage());
        }
    }
}