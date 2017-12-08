package gui;

import com.sun.istack.internal.NotNull;
import javafx.beans.property.SimpleFloatProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;

/**
 * Data model class for interactions TableView.
 * @author Boyarkin Nikita.
 */
public class InteractionTableData {
    /**
     * Table columns.
     */
    private @NotNull final SimpleIntegerProperty mIndex;
    private @NotNull final SimpleStringProperty mName;
    private @NotNull final SimpleStringProperty mSize;
    private @NotNull final SimpleIntegerProperty mPeers;
    private @NotNull final SimpleFloatProperty mPercent;
    private @NotNull final SimpleStringProperty mUploadingSpeed;
    private @NotNull final SimpleStringProperty mSharingSpeed;
    private @NotNull final SimpleStringProperty mState;

    /**
     * Creates InteractionTableData object.
     * @param index - Index of the interaction (optional in autoincrement mode).
     * @param name - Name of the interaction.
     * @param size - The total size of all files in the interaction.
     * @param peers - Count of peers in the interaction.
     * @param percent - Percentage of uploaded pieces.
     * @param uploadingSpeed - Current uploading speed.
     * @param sharingSpeed - Current sharing speed.
     * @param state - Current state of the interaction.
     */
    public InteractionTableData(@NotNull final Integer index, @NotNull final String name, @NotNull final String size, @NotNull final Integer peers,
                                @NotNull final Float percent, @NotNull final String uploadingSpeed, @NotNull final String sharingSpeed, @NotNull final String state) {
        mIndex = new SimpleIntegerProperty(index);
        mName = new SimpleStringProperty(name);
        mSize = new SimpleStringProperty(size);
        mPeers = new SimpleIntegerProperty(peers);
        mPercent = new SimpleFloatProperty(percent);
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
     * Getters and setters for 'name' column.
     */

    public @NotNull String getName() {
        return mName.get();
    }

    public @NotNull SimpleStringProperty getNameProperty() {
        return mName;
    }

    public void setName(@NotNull final String name) {
        mName.set(name);
    }

    /**
     * Getters and setters for 'size' column.
     */

    public @NotNull String getSize() {
        return mSize.get();
    }

    public @NotNull SimpleStringProperty getSizeProperty() {
        return mSize;
    }

    public void setSize(@NotNull final String size) {
        mSize.set(size);
    }

    /**
     * Getters and setters for 'percent' column.
     */

    public @NotNull float getPercent() {
        return mPercent.get();
    }

    public @NotNull SimpleFloatProperty getPercentProperty() {
        return mPercent;
    }

    public void setPercent(@NotNull final Float percent) {
        mPercent.set(percent);
    }

    /**
     * Getters and setters for 'peers' column.
     */

    public @NotNull int getPeers() {
        return mPeers.get();
    }

    public @NotNull SimpleIntegerProperty getPeersProperty() {
        return mPeers;
    }

    public void setPeers(@NotNull final Integer peers) {
        mPeers.set(peers);
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
