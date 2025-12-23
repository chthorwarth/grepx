typedef struct {
    bool ignore_case;      // -i
    bool invert_match;     // -v
    bool show_line_number; // -n
    bool count_only;       // -c
    bool list_files;       // -l
    bool quiet;            // -q
    bool recursive;        // -r

    int context;           // -C <n>

    // Pattern handling
    char **patterns;       // -e, -f, oder Positions-PATTERN
    int pattern_count;

    // Files / paths
    char **paths;          // file1 file2 dir1 ...
    int path_count;

} grep_options_t;