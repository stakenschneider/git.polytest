package interaction.fileAnalyser;

import com.sun.istack.internal.NotNull;
import com.sun.istack.internal.Nullable;
import util.MD5;

import java.util.Collections;
import java.util.List;

/**
 *  Information about the distributed file.
 *  @author Boyarkin Nikita.
 */
public class DistributedFile {
    /** File name. */
    public @NotNull final String name;

    /** File length. */
    public @NotNull final Long length;

    /** File MD5 hash. */
    public @Nullable final MD5 hash;

    /** Path to file. */
    public @Nullable final List<String> directories;

    /**
     * Creates DistributedFile object.
     * @param name - File name.
     * @param length - File length.
     * @param hash - File MD5 hash.
     * @param directories - Path to file.
     */
    public DistributedFile(@NotNull final String name, @NotNull final Long length, @Nullable final MD5 hash, @Nullable final List<String> directories) {
        this.name = name;
        this.length = length;
        this.hash = hash;
        this.directories = (directories != null) ? Collections.unmodifiableList(directories): null;
    }
}