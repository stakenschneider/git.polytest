package gui;

import com.sun.istack.internal.NotNull;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;

/**
 * Data model class for peers TableView.
 * @author Boyarkin Nikita.
 */
public class PeerTableData {
    /**
     * Table columns.
     */
    private @NotNull final SimpleIntegerProperty mIndex;
    private @NotNull final SimpleStringProperty mIp;
    private @NotNull final SimpleIntegerProperty mPort;
    private @NotNull final SimpleStringProperty mUploadingSpeed;
    private @NotNull final SimpleStringProperty mSharingSpeed;
    private @NotNull final SimpleStringProperty mState;

    /**
     * Creates PeerTableData object.
     * @param index - Index of the peer (optional in autoincrement mode).
     * @param ip - IP address of the peer.
     * @param port - Port of the peer.
     * @param uploadingSpeed - Current uploading speed of the peer.
     * @param sharingSpeed - Current sharing speed of the peer.
     * @param state - Current state of the peer
     */
    public PeerTableData(@NotNull final Integer index, @NotNull final String ip, @NotNull final Integer port,
                          @NotNull final String uploadingSpeed, @NotNull final String sharingSpeed, @NotNull final String state) {
        mIndex = new SimpleIntegerProperty(index);
        mIp = new SimpleStringProperty(ip);
        mPort = new SimpleIntegerProperty(port);
        mUploadingSpeed = new SimpleStringProperty(uploadingSpeed);
        mSharingSpeed = new SimpleStringProperty(sharingSpeed);
        mState = new SimpleStringProperty(state);
    }

    /**
     * Getters and setters for 'index' column.
     */

    public @NotNull int getIndex() {
        return mIndex.get();
    }

    public @NotNull SimpleIntegerProperty getIndexProperty() {
        return mIndex;
    }

    public void setIndex(@NotNull final Integer index) {
        mIndex.set(index);
    }

    /**
     * Getters and setters for 'ip' column.
     */

    public @NotNull String getIpAddress() {
        return mIp.get();
    }

    public @NotNull SimpleStringProperty getIpAddressProperty() {
        return mIp;
    }

    public void setIpAddress(@NotNull final String ip) {
        mIp.set(ip);
    }

    /**
     * Getters and setters for 'port' column.
     */

    public @NotNull int getPort() {
        return mPort.get();
    }

    public @NotNull SimpleIntegerProperty getPortProperty() {
        return mPort;
    }

    public void setPort(@NotNull final Integer port) {
        mPort.set(port);
    }

    /**
     * Getters and setters for 'uploadingSpeed' column.
     */

    public @NotNull String getUploadingSpeed() {
        return mUploadingSpeed.get();
    }

    public @NotNull SimpleStringProperty getUploadingSpeedProperty() {
        return mUploadingSpeed;
    }

    public void setUploadingSpeed(@NotNull final String uploadingSpeed) {
        mUploadingSpeed.set(uploadingSpeed);
    }

    /**
     * Getters and setters for 'sharingSpeed' column.
     */

    public @NotNull String getSharingSpeed() {
        return mSharingSpeed.get();
    }

    public @NotNull SimpleStringProperty getSharingSpeedProperty() {
        return mSharingSpeed;
    }

    public void setSharingSpeed(@NotNull final String sharingSpeed) {
        mSharingSpeed.set(sharingSpeed);
    }

    /**
     * Getters and setters for 'state' column.
     */

    public @NotNull String getState() {
        return mState.get();
    }

    public @NotNull SimpleStringProperty getStateProperty() {
        return mState;
    }

    public void setState(@NotNull final String state) {
        mState.set(state);
    }
}
