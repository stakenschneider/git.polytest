package gui;

import com.sun.istack.internal.NotNull;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;

/**
 * Data model class for trackers TableView.
 * @author Boyarkin Nikita.
 */
public class TrackerTableData {
    /**
     * Table columns.
     */
    private @NotNull final SimpleIntegerProperty mIndex;
    private @NotNull final SimpleStringProperty mUrl;
    private @NotNull final SimpleStringProperty mState;
    private @NotNull final SimpleIntegerProperty mPeers;
    private @NotNull final SimpleIntegerProperty mSeeders;
    private @NotNull final SimpleIntegerProperty mLeechers;

    /**
     * Creates TrackerTableData object.
     * @param index - Index of the tracker (optional in autoincrement mode).
     * @param url - URL of the tracker.
     * @param state - Current state of the tracker.
     * @param peers - Count of peers at the tracker.
     * @param seeders - Count of seeders at the tracker.
     * @param leechers - Count of leechers at the tracker.
     */
    public TrackerTableData(@NotNull final Integer index, @NotNull final String url, @NotNull final String state, @NotNull final Integer peers, @NotNull final Integer seeders, @NotNull final Integer leechers) {
        mIndex = new SimpleIntegerProperty(index);
        mUrl = new SimpleStringProperty(url);
        mState = new SimpleStringProperty(state);
        mPeers = new SimpleIntegerProperty(peers);
        mSeeders = new SimpleIntegerProperty(seeders);
        mLeechers = new SimpleIntegerProperty(leechers);
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
     * Getters and setters for 'url' column.
     */

    public @NotNull String getUrl() {
        return mUrl.get();
    }

    public @NotNull SimpleStringProperty getUrlProperty() {
        return mUrl;
    }

    public void setUrl(@NotNull final String url) {
        mUrl.set(url);
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
     * Getters and setters for 'seeders' column.
     */

    public @NotNull int getSeeders() {
        return mSeeders.get();
    }

    public @NotNull SimpleIntegerProperty getSeedersProperty() {
        return mSeeders;
    }

    public void setSeeders(@NotNull final Integer seeders) {
        mSeeders.set(seeders);
    }

    /**
     * Getters and setters for 'leechers' column.
     */

    public @NotNull int getLeechers() {
        return mLeechers.get();
    }

    public @NotNull SimpleIntegerProperty getLeechersProperty() {
        return mLeechers;
    }

    public void setLeechers(@NotNull final Integer leechers) {
        mLeechers.set(leechers);
    }
}
