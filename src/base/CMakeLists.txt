#
# Source files.
#

set(SRC_BASE
        app_browser.cpp
        app_other.cpp
        app_renderer.cpp
        client.cpp
        client.hpp
        main.cpp)

#
# Shared configuration.
#

if(OS_MACOSX)
    set(EXECUTABLE_HELPER_TARGET "${PROJECT_TARGET_EXECUTABLE_NAME}_Helper")
    set(EXECUTABLE_HELPER_OUTPUT_NAME "${PROJECT_TARGET_EXECUTABLE_NAME} Helper")
endif()

#
# Linux configuration.
#

if(OS_LINUX)
    # Executable target.
    add_executable(${EXECUTABLE_NAME} ${SRC_BASE})

    SET_PROJECT_EXECUTABLE_TARGET_PROPERTIES(${EXECUTABLE_NAME})

    add_dependencies(${EXECUTABLE_NAME} "${SHARED_TARGET}")
    target_link_libraries(${EXECUTABLE_NAME} "${SHARED_TARGET}")

    # copies the actual generated executable file to the ${BUILD_DIR} i.e Release, Debug etc.
    COPY_FILES("${EXECUTABLE_NAME}" "${EXECUTABLE_NAME}" "${CMAKE_CURRENT_BINARY_DIR}" "${TARGET_OUTPUT_DIR}")

endif()