package gui;

import com.sun.istack.internal.NotNull;
import javafx.beans.property.SimpleFloatProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;

/**
 * Data model class for files TableView.
 * @author Boyarkin Nikita.
 */
public class FileTableData {
    /**
     * Table columns.
     */
    private @NotNull final SimpleIntegerProperty mIndex;
    private @NotNull final SimpleStringProperty mSize;
    private @NotNull final SimpleStringProperty mPath;
    private @NotNull final SimpleFloatProperty mPercent;

    /**
     * Creates FileTableData object.
     * @param index - Index of the file (optional in autoincrement mode).
     * @param path - Path of the file.
     * @param size - The total size of the file.
     * @param percent - Percentage of uploaded pieces.
     */
    public FileTableData(@NotNull final Integer index, @NotNull final String path, @NotNull final String size, @NotNull final Float percent) {
        mIndex = new SimpleIntegerProperty(index);
        mPath = new SimpleStringProperty(path);
        mSize = new SimpleStringProperty(size);
        mPercent = new SimpleFloatProperty(percent);
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
     * Getters and setters for 'path' column.
     */

    public @NotNull String getPath() {
        return mPath.get();
    }

    public @NotNull SimpleStringProperty getPathProperty() {
        return mPath;
    }

    public void setPath(@NotNull final String path) {
        mPath.set(path);
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
}
