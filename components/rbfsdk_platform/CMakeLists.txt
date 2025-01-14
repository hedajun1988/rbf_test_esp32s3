set(srcs "")
set(include_dirs "")

list(APPEND srcs  "source/rbf_mem.c"
                    "source/rbf_dbg_log.c"
                    "source/rbf_queue.c"
                    "source/rbf_mutex.c"
                    "source/rbf_thread.c"
                    "source/rbf_time.c"
                    "source/rbf_event.c")

list(APPEND include_dirs  "include")


idf_component_register(SRCS "${srcs}"
                        INCLUDE_DIRS "${include_dirs}")


# After build, copy the archive file and header file to parent example directory's main component
add_custom_command(TARGET ${COMPONENT_LIB}
                  POST_BUILD
                  COMMENT "Copying built archive file and header to parent example directory...")                     