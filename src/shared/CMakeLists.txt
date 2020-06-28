#
# Source files.
#

# Sources shared by all executables.
set(SRC_SHARED_COMMON
        app_factory.hpp
        browser_util.cpp
        browser_util.hpp
        client_manager.cpp
        client_manager.hpp
        client_util.cpp
        client_util.hpp
        main.hpp
        main_util.cpp
        main_util.hpp
        resources_util.cpp
        resources_util.hpp
        )

set(SRC_SHARED_LINUX
        client_util_linux.cpp
        main_linux.cpp
        resources_util_linux.cpp
        resources_util_posix.cpp
        )

set(SRC_SHARED_OSX
        resources_util_posix.cpp
        )

if(OS_LINUX)
    list(APPEND SRC_SHARED_COMMON ${SRC_SHARED_LINUX})
endif()

if(OS_MACOSX)
    list(APPEND SRC_SHARED_COMMON ${SRC_SHARED_OSX})
endif()

APPEND_PLATFORM_SOURCES(SRC_SHARED_COMMON)

#
# Shared configuration.
#
if(OS_MACOSX)
    set(SHARED_HELPER_TARGET "${SHARED_TARGET}_helper")
endif()

if(OS_LINUX OR OS_WINDOWS)
    # Logical target used to link the libcef library on Linux and Windows.
    # On macOS the CEF framework is loaded dynamically at startup.
    ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()

#
# Linux configuration.
#

if(OS_LINUX)
    # Static library target.
    # Create a library called "${SHARED_TARGET}" which includes the source files configured.
    add_library(${SHARED_TARGET} STATIC ${SRC_SHARED_COMMON})

    # Make sure the compiler can find include files for our ${SHARED_TARGET} library
    # when other libraries or executables link to ${SHARED_TARGET}
    # target_include_directories(${SHARED_TARGET} PUBLIC ${NAME_TARGET})

    SET_PROJECT_LIBRARY_TARGET_PROPERTIES(${SHARED_TARGET})

    add_dependencies(${SHARED_TARGET} libcef_dll_wrapper)
    target_link_libraries(${SHARED_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

    # Copy binary and resource files to the target output directory.
    COPY_FILES("${SHARED_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${TARGET_OUTPUT_DIR}")
    COPY_FILES("${SHARED_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${TARGET_OUTPUT_DIR}")

    # copies the actual generated library .a file to the ${BUILD_DIR} i.e Release, Debug etc.
    COPY_FILES("${SHARED_TARGET}" "$<TARGET_LINKER_FILE_NAME:${SHARED_TARGET}>" "${CMAKE_CURRENT_BINARY_DIR}" "${TARGET_OUTPUT_DIR}")

    # Output a message about setting SUID permissions on the chrome-sandbox target.
    SET_LINUX_SUID_PERMISSIONS("${SHARED_TARGET}" "${TARGET_OUTPUT_DIR}/chrome-sandbox")
endif()