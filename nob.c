#define BUILD_DIR "build/"
#define SRC_DIR "src/"
#define INCLUDE_DIR "include/"

#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD

#include "include/nob.h"

int main(int argc, char** argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, 
        "clang", 
        "-g3", 
        "-o",
        BUILD_DIR"tracker.exe", 
        SRC_DIR"*.c", 
        "-std=c99", 
        "-Wall", 
        "-Wextra", 
        "-lraylib", 
        "-lglfw3", 
        "-I"INCLUDE_DIR
    );
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

    nob_set_current_dir(BUILD_DIR);

    nob_cmd_append(&cmd, "tracker.exe");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

    return 0;
}