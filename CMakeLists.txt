cmake_minimum_required(VERSION 3.28) # Modern CMake version
project(nadicpp VERSION 1.0.0 LANGUAGES CXX)

# Define the INTERFACE library
add_library(nadicpp INTERFACE)
add_library(nadicpp::nadicpp ALIAS nadicpp)

if(NOT TARGET nadi::nadi)
    add_subdirectory(node-agnostic-datastream-interface)
endif()
target_link_libraries(nadicpp INTERFACE nadi::nadi)

# Specify include directories
target_include_directories(nadicpp
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set C++ standard (optional, customize as needed)
target_compile_features(nadicpp INTERFACE cxx_std_20)

# Installation rules
include(GNUInstallDirs)
install(TARGETS nadicpp
    EXPORT nadicppTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets for find_package
install(EXPORT nadicppTargets
    FILE nadicppTargets.cmake
    NAMESPACE nadicpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nadicpp
)

# Generate and install package config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/nadicppConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/nadicppConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/nadicppConfig.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/nadicppConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/nadicppConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nadicpp
)

# Support FetchContent
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(NADICPP_IS_TOP_LEVEL TRUE)
else()
    set(NADICPP_IS_TOP_LEVEL FALSE)
endif()