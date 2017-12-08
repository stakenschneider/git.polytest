package interaction;

import com.sun.istack.internal.NotNull;
import gui.Controller;
import gui.InteractionTableData;
import interaction.fileAnalyser.DistributedFile;
import interaction.fileAnalyser.FileAnalyzer;
import interaction.fileAnalyser.PiecesPercentChangeable;
import interaction.peerToPeer.PeerToPeerManager;
import interaction.peerToPeer.SpeedChangeable;
import interaction.torrentParser.TorrentParser;
import interaction.trackerService.PeersCountChangeable;
import interaction.trackerService.TrackerService;
import javafx.application.Platform;
import javafx.scene.control.Alert;
import util.Log;
import util.Util;

import java.io.*;

public class Interaction implements PiecesPercentChangeable, PeersCountChangeable, SpeedChangeable {
    private @NotNull final Controller mContext;
    private @NotNull final File mTorrentFile;
    private @NotNull final int mPeerPort;

    private @NotNull
    TorrentParser mTorrentParser;
    private @NotNull
    FileAnalyzer mFileAnalyzer;
    private @NotNull
    TrackerService mTrackerService;
    private @NotNull
    PeerToPeerManager mPeerToPeerManager;

    private @NotNull InteractionTableData mInteractionTableData;

    public Interaction(@NotNull final Controller context, @NotNull final File torrentFile, @NotNull final int peerPort) {
        mContext = context;
        mTorrentFile = torrentFile;
        mPeerPort = peerPort;
    }

    public void startInteraction() throws InterruptedException {
        try {
            mTorrentParser = new TorrentParser(mTorrentFile);
            Log.i("Torrent file has been successfully parsed.");
        }
        catch(final TorrentParser.ParseException exception) {
            Log.e(exception.getMessage());
            Util.showMessageDialog(Alert.AlertType.ERROR, "Parse Exception", null, exception.getMessage());
            return;
        }

        Util.checkInterrupted(null);

        long totalLength = 0;
        if(mTorrentParser.getSingleMode())
            totalLength = mTorrentParser.getSingleFile().length;
        else
            for(final DistributedFile currentFile: mTorrentParser.getFiles())
                totalLength += currentFile.length;

        Util.checkInterrupted(null);

        mInteractionTableData = new InteractionTableData(-1, mTorrentParser.getHeader(), Util.lengthToHumanReadableFormat(totalLength, false),
                0, 0.f, "", "","Analyze");
        Platform.runLater(() -> mContext.interactionsData.add(mInteractionTableData));

        Util.checkInterrupted(null);

        try {
            mFileAnalyzer = new FileAnalyzer(mContext.filesData, mTorrentFile, mTorrentParser);
            Log.i("All files have been successfully analyzed.");
        }
        catch(final FileAnalyzer.AnalyzeException exception) {
            Log.e(exception.getMessage());
            Util.showMessageDialog(Alert.AlertType.ERROR, "Analyze Exception", null, exception.getMessage());
            return;
        }

        Util.checkInterrupted(null);

        mFileAnalyzer.setOnAllPiecesChanged(this);
        onPiecesPercentChanged(mFileAnalyzer.getAllPiecesPercent());

        try {
            mTrackerService = new TrackerService(mContext.trackersData, this, mTorrentParser, mFileAnalyzer, mPeerPort);
        }
        catch(final TrackerService.TrackerException exception) {
            Log.e(exception.getMessage());
            Util.showMessageDialog(Alert.AlertType.ERROR, "Tracker Exception", null, exception.getMessage());
            return;
        }

        mInteractionTableData.setState("Wait peers");

        while(mTrackerService.getPeerSet().isEmpty()) {
            Thread.sleep(1000);
        }

        mInteractionTableData.setState("Uploading");

        try {
            mPeerToPeerManager = new PeerToPeerManager(mContext.peersData, this, mTorrentParser, mFileAnalyzer, mTrackerService, mPeerPort);
        }
        catch(final IOException exception) {
            Log.e("Impossible to open server socket channel.");
            Util.showMessageDialog(Alert.AlertType.ERROR, "Tracker Exception", null, "Impossible to open server socket channel.");
            return;
        }
    }

    public void clearResources() {
        if(mTrackerService != null)
            mTrackerService.stopTimers();
    }

    @Override
    public void onPiecesPercentChanged(@NotNull final float percent) {
        mInteractionTableData.setPercent(Math.round(percent * 100.f) / 100.f);
    }

    @Override
    public void onPeersCountChanged(@NotNull final int peersCount) {
        mInteractionTableData.setPeers(peersCount);
    }

    @Override
    public void onSpeedChanged(@NotNull final long uploadingBytes, @NotNull final long sharingBytes) {
        mInteractionTableData.setUploadingSpeed(Util.lengthToHumanReadableFormat(uploadingBytes * Byte.SIZE, true) + "ps");
        mInteractionTableData.setSharingSpeed(Util.lengthToHumanReadableFormat(sharingBytes * Byte.SIZE, true) + "ps");
    }
}
