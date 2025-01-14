idf_component_register(SRCS "main.c" "rbftest.c"
                    INCLUDE_DIRS "" "${PROJECT_DIR}/rbfsdk/include")
add_prebuilt_library(rbfsdk "${PROJECT_DIR}/rbfsdk/librbfsdk.a" REQUIRES rbfsdk_platform)
target_link_libraries(${COMPONENT_LIB} PRIVATE rbfsdk)
