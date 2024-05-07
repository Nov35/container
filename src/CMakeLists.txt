set(src_files
    "list/list.h"
    "list/list_iterator.h"
    "lifetime_helper/lifetime_helper.h"
    "lifetime_helper/lifetime_helper.cpp"
)

add_library(Container STATIC "${src_files}")

target_include_directories(Container PUBLIC "./")
target_include_directories(Container PUBLIC "./list")
