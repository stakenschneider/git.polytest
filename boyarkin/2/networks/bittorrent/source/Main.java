import gui.Controller;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import util.Log;

public class Main extends Application {

    @Override
    public void start(Stage stage) throws Exception{
        final FXMLLoader loader = new FXMLLoader(getClass().getResource("gui/window.fxml"));
        final Parent root = loader.load();

        final Controller controller = loader.getController();
        controller.setStage(stage);

        stage.setTitle("Torrent");
        stage.setScene(new Scene(root));
        stage.show();
    }


    public static void main(String[] args) {
        Log.setLogStreams(System.out, System.err);
        launch(args);
    }
}
