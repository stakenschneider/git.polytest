package gui;

import com.sun.istack.internal.NotNull;
import interaction.InteractionCreator;
import javafx.beans.property.ReadOnlyObjectWrapper;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import util.Log;

import java.io.*;
import java.net.URL;
import java.util.ResourceBundle;

public class Controller implements Initializable {
    private @NotNull Stage mStage;

    private @NotNull InteractionCreator mInteractionCreator;

    @FXML
    public @NotNull MenuItem menuFileOpen, menuHelpAbout;

    @FXML
    public @NotNull TableView<InteractionTableData> interactionsList;
    public @NotNull ObservableList<InteractionTableData> interactionsData;

    @FXML
    public @NotNull TableView<FileTableData> filesList;
    public @NotNull ObservableList<FileTableData> filesData;

    @FXML
    public @NotNull TableView<TrackerTableData> trackersList;
    public @NotNull ObservableList<TrackerTableData> trackersData;

    @FXML
    public @NotNull TableView<PeerTableData> peersList;
    public @NotNull ObservableList<PeerTableData> peersData;

    @Override
    @SuppressWarnings("unchecked")
    public void initialize(URL location, ResourceBundle resources) {
        // Interactions TableView initialize.

        final ObservableList<TableColumn<InteractionTableData, ?>> interactionsColumns = interactionsList.getColumns();
        interactionsColumns.get(0).setCellValueFactory(value -> new ReadOnlyObjectWrapper(interactionsList.getItems().indexOf(value.getValue())));
        ((TableColumn<InteractionTableData, String>) interactionsColumns.get(1)).setCellValueFactory(value -> value.getValue().getNameProperty());
        ((TableColumn<InteractionTableData, String>) interactionsColumns.get(2)).setCellValueFactory(value -> value.getValue().getSizeProperty());
        ((TableColumn<InteractionTableData, Integer>) interactionsColumns.get(3)).setCellValueFactory(value -> value.getValue().getPeersProperty().asObject());
        ((TableColumn<InteractionTableData, Float>) interactionsColumns.get(4)).setCellValueFactory(value -> value.getValue().getPercentProperty().asObject());
        ((TableColumn<InteractionTableData, String>) interactionsColumns.get(5)).setCellValueFactory(value -> value.getValue().getUploadingSpeedProperty());
        ((TableColumn<InteractionTableData, String>) interactionsColumns.get(6)).setCellValueFactory(value -> value.getValue().getSharingSpeedProperty());
        ((TableColumn<InteractionTableData, String>) interactionsColumns.get(7)).setCellValueFactory(value -> value.getValue().getStateProperty());

        interactionsData = FXCollections.observableArrayList();
        interactionsList.setItems(interactionsData);
        interactionsList.setEditable(true);

        // Files TableView initialize.

        final ObservableList<TableColumn<FileTableData, ?>> filesColumns = filesList.getColumns();
        filesColumns.get(0).setCellValueFactory(value -> new ReadOnlyObjectWrapper(filesList.getItems().indexOf(value.getValue())));
        ((TableColumn<FileTableData, String>) filesColumns.get(1)).setCellValueFactory(value -> value.getValue().getPathProperty());
        ((TableColumn<FileTableData, String>) filesColumns.get(2)).setCellValueFactory(value -> value.getValue().getSizeProperty());
        ((TableColumn<FileTableData, Float>) filesColumns.get(3)).setCellValueFactory(value -> value.getValue().getPercentProperty().asObject());

        filesData = FXCollections.observableArrayList();
        filesList.setItems(filesData);
        filesList.setEditable(true);

        // Trackers TableView initialize.

        final ObservableList<TableColumn<TrackerTableData, ?>> trackersColumns = trackersList.getColumns();
        trackersColumns.get(0).setCellValueFactory(value -> new ReadOnlyObjectWrapper(trackersList.getItems().indexOf(value.getValue())));
        ((TableColumn<TrackerTableData, String>) trackersColumns.get(1)).setCellValueFactory(value -> value.getValue().getUrlProperty());
        ((TableColumn<TrackerTableData, String>) trackersColumns.get(2)).setCellValueFactory(value -> value.getValue().getStateProperty());
        ((TableColumn<TrackerTableData, Integer>) trackersColumns.get(3)).setCellValueFactory(value -> value.getValue().getPeersProperty().asObject());
        ((TableColumn<TrackerTableData, Integer>) trackersColumns.get(4)).setCellValueFactory(value -> value.getValue().getSeedersProperty().asObject());
        ((TableColumn<TrackerTableData, Integer>) trackersColumns.get(5)).setCellValueFactory(value -> value.getValue().getLeechersProperty().asObject());

        trackersData = FXCollections.observableArrayList();
        trackersList.setItems(trackersData);
        trackersList.setEditable(true);

        // Peers TableView initialize.

        final ObservableList<TableColumn<PeerTableData, ?>> peersColumns = peersList.getColumns();
        peersColumns.get(0).setCellValueFactory(value -> new ReadOnlyObjectWrapper(peersList.getItems().indexOf(value.getValue())));
        ((TableColumn<PeerTableData, String>) peersColumns.get(1)).setCellValueFactory(value -> value.getValue().getIpAddressProperty());
        ((TableColumn<PeerTableData, Integer>) peersColumns.get(2)).setCellValueFactory(value -> value.getValue().getPortProperty().asObject());
        ((TableColumn<PeerTableData, String>) peersColumns.get(3)).setCellValueFactory(value -> value.getValue().getUploadingSpeedProperty());
        ((TableColumn<PeerTableData, String>) peersColumns.get(4)).setCellValueFactory(value -> value.getValue().getSharingSpeedProperty());
        ((TableColumn<PeerTableData, String>) peersColumns.get(5)).setCellValueFactory(value -> value.getValue().getStateProperty());

        peersData = FXCollections.observableArrayList();
        peersList.setItems(peersData);
        peersList.setEditable(true);

        mInteractionCreator = InteractionCreator.getInstance();
        menuFileOpen.setOnAction(sMenuFileOpenEvent);

        Log.d("Controller initialization finished.");
    }

    public void setStage(@NotNull final Stage stage) {
        mStage = stage;
        mStage.setOnCloseRequest(sExitEvent);
    }

    private final @NotNull EventHandler<WindowEvent> sExitEvent = event -> mInteractionCreator.interruptAllThreads();

    private final @NotNull EventHandler<ActionEvent> sMenuFileOpenEvent = event -> {
        final FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Open torrent file");
        fileChooser.getExtensionFilters().add(new FileChooser.ExtensionFilter("Torrent file (*.torrent)", "*.torrent"));

        final File file = fileChooser.showOpenDialog(mStage);
        if(file != null)
            mInteractionCreator.createNewInteraction(this, file);
    };
}
