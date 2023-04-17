# Add GNU Coding Standards support.
include(GNUInstallDirs)

function(add_configuration config_name)
    # Create library with INTERFACE parameter because we want to use headers from outside.
    add_library(FakeIt-${config_name} INTERFACE)

    # Add include directories to the interface library.
    # If we use FakeIt from build-tree then we use BUILD_INTERFACE wrapper.
    # If we use FakeIt from install-tree then we use INSTALL_INTERFACE wrapper.
    target_include_directories(FakeIt-${config_name} SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${config_name}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${config_name}>
    )

    # Add alias for the library with namespace.
    add_library(FakeIt::FakeIt-${config_name} ALIAS FakeIt-${config_name})

    # Install the directory containing the config to the install directory.
    # "${config_name}" instead of "${config_name}/" string means that the header will be
    # installed at ${CMAKE_INSTALL_PREFIX}/include/${config_name}/fakeit.hpp
    # instead of ${CMAKE_INSTALL_PREFIX}/include/fakeit.hpp.
    install(DIRECTORY ${config_name} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    # Add the new config target to the FakeItConfig.cmake file which will be used to find FakeIt from outside.
    install(TARGETS FakeIt-${config_name} EXPORT FakeItConfig)
endfunction()

add_configuration("boost")
add_configuration("catch")
add_configuration("cute")
add_configuration("doctest")
add_configuration("gtest")
add_configuration("mettle")
add_configuration("mstest")
add_configuration("nunit")
add_configuration("qtest")
add_configuration("standalone")
add_configuration("tpunit")

set(FakeIt_CONFIG_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/FakeIt)

# Install FakeItConfig.cmake file to the ${CMAKE_INSTALL_PREFIX}/${FakeIt_CONFIG_DIR} directory and all correlated
# targets should be called from outside by the namespace.
install(EXPORT FakeItConfig NAMESPACE FakeIt:: DESTINATION ${FakeIt_CONFIG_DIR})

# Add version control.
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/FakeItConfigVersion.cmake"
    VERSION
        ${PROJECT_VERSION}
    COMPATIBILITY
        SameMajorVersion
    ARCH_INDEPENDENT
)
install(FILES "${PROJECT_BINARY_DIR}/FakeItConfigVersion.cmake" DESTINATION ${FakeIt_CONFIG_DIR})
