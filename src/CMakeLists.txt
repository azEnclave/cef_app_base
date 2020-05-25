#
# Shared configuration.
#

# Target static library names.
set(SHARED_TARGET "shared")

# Target executable names.
set(EXECUTABLE_NAME "${PROJECT_TARGET_EXECUTABLE_NAME}")

# Set the configuration-specific binary output directory.
if(GEN_NINJA OR GEN_MAKEFILES)
    # Force Ninja and Make to create a subdirectory named after the configuration.
    set(TARGET_OUTPUT_DIR "${BUILD_DIR}/${CMAKE_BUILD_TYPE}")
else()
    set(TARGET_OUTPUT_DIR "${BUILD_DIR}/$<CONFIGURATION>")
endif()

# Set properties common to all example targets.
macro(SET_PROJECT_TARGET_PROPERTIES target)
     # Output all binaries to the configuration-specific build directory.
     set_target_properties(${target} PROPERTIES
             ARCHIVE_OUTPUT_DIRECTORY "${TARGET_OUT_DIR}"
             RUNTIME_OUTPUT_DIRECTORY "${TARGET_OUT_DIR}"
             LIBRARY_OUTPUT_DIRECTORY "${TARGET_OUT_DIR}")

     if(OS_MACOSX OR OS_WINDOWS)
         # Place the target in the "${PROJECT_TARGET_EXECUTABLE_NAME}" folder in Visual Studio and Xcode.
         set_property(TARGET ${target} PROPERTY FOLDER ${PROJECT_TARGET_EXECUTABLE_NAME})
     endif()
endmacro()

# Set properties on an example library target.
macro(SET_PROJECT_LIBRARY_TARGET_PROPERTIES target)
     SET_LIBRARY_TARGET_PROPERTIES(${target})
     SET_PROJECT_TARGET_PROPERTIES(${target})
endmacro()

# Set properties on an example executable target.
macro(SET_PROJECT_EXECUTABLE_TARGET_PROPERTIES target)
     SET_EXECUTABLE_TARGET_PROPERTIES(${target})
     SET_PROJECT_TARGET_PROPERTIES(${target})

     if(OS_LINUX)
         # Set rpath so that libraries can be placed next to the executable.
         set_target_properties(${target} PROPERTIES INSTALL_RPATH "$ORIGIN")
         set_target_properties(${target} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)

     elseif(OS_WINDOWS)
         # Add the custom Windows manifest files to the executable.
         add_custom_command(
                 TARGET ${target}
                 POST_BUILD
                 COMMAND "mt.exe" -nologo
                 -manifest \"${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_EXE_MANIFEST}\"
                 \"${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_COMPATIBILITY_MANIFEST}\"
                 -outputresource:"${TARGET_OUT_DIR}/${target}.exe"\;\#1
                 COMMENT "Adding manifest..."
         )
     endif()
endmacro()

#
# Target configuration.
#

# Static library shared by all example executable targets.
add_subdirectory(shared)

# Base target
add_subdirectory(base)
