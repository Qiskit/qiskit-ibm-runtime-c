cmake_minimum_required(VERSION 3.21)
project(qiskit-ibm-runtime LANGUAGES C VERSION 0.1.0)

# ---- Tooling / options -------------------------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Build type -> Cargo profile
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
string(TOLOWER "${CMAKE_BUILD_TYPE}" _bt_lower)
if(_bt_lower STREQUAL "debug")
    set(CARGO_PROFILE dev)
else()
    # map RelWithDebInfo/Release/MinSizeRel to 'release'
    set(CARGO_PROFILE release)
endif()

# ---- Programs ---------------------------------------------------------------
find_package(Git REQUIRED)
find_program(CARGO_EXECUTABLE cargo REQUIRED)
find_program(MAKE_EXECUTABLE NAMES gmake make REQUIRED)

# ---- Paths / names -----------------------------------------------------------
set(QISKIT_REPO https://github.com/Qiskit/qiskit.git)

set(QISKIT_SRCDIR ${CMAKE_BINARY_DIR}/qiskit_srcdir)
set(QISKIT_INCDIR ${QISKIT_SRCDIR}/dist/c/include)
set(QISKIT_LIBPATH
        ${QISKIT_SRCDIR}/dist/c/lib/${CMAKE_SHARED_LIBRARY_PREFIX}qiskit${CMAKE_SHARED_LIBRARY_SUFFIX})

set(PROJECT_INCLUDEDIR ${CMAKE_SOURCE_DIR}/include)

if(CARGO_PROFILE STREQUAL "dev")
    set(_CARGO_DIR debug)
else()
    set(_CARGO_DIR ${CARGO_PROFILE})  # 'release' stays 'release'
endif()

# Put Cargo artifacts inside the build tree instead of the source tree
set(CARGO_TARGET_DIR ${CMAKE_BINARY_DIR}/cargo)

set(RUST_LIBNAME ${CMAKE_SHARED_LIBRARY_PREFIX}qiskit_ibm_runtime${CMAKE_SHARED_LIBRARY_SUFFIX})
set(RUST_OUTDIR ${CARGO_TARGET_DIR}/${_CARGO_DIR})
set(RUST_LIBPATH ${RUST_OUTDIR}/${RUST_LIBNAME})

# Make sure these exist at configure time so CMake doesn't error
file(MAKE_DIRECTORY ${QISKIT_INCDIR})
file(MAKE_DIRECTORY ${QISKIT_SRCDIR}/dist/c/lib)

# ---- Fetch & build qiskit C library -----------------------------------------
include(ExternalProject)

ExternalProject_Add(qiskit_src
        GIT_REPOSITORY    ${QISKIT_REPO}
        GIT_TAG           main
        GIT_SHALLOW       1
        UPDATE_COMMAND    ${GIT_EXECUTABLE} fetch --depth 1 origin main
        COMMAND ${GIT_EXECUTABLE} reset --hard origin/main
        CONFIGURE_COMMAND ""
        BUILD_IN_SOURCE   1
        BUILD_COMMAND     ${MAKE_EXECUTABLE} c
        INSTALL_COMMAND   ""
        SOURCE_DIR        ${QISKIT_SRCDIR}
        BUILD_BYPRODUCTS  ${QISKIT_LIBPATH}
)

# Imported target for the built libqiskit
add_library(qiskit SHARED IMPORTED GLOBAL)
set_property(TARGET qiskit PROPERTY IMPORTED_LOCATION ${QISKIT_LIBPATH})
set_property(TARGET qiskit PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${QISKIT_INCDIR})
add_dependencies(qiskit qiskit_src)

# ---- Build Rust cdylib via cargo --------------------------------------------
add_custom_command(
        OUTPUT ${RUST_LIBPATH}
        COMMAND ${CARGO_EXECUTABLE} build
        --manifest-path ${CMAKE_SOURCE_DIR}/crates/client/Cargo.toml
        --profile ${CARGO_PROFILE}
        --locked
        --target-dir ${CARGO_TARGET_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        BYPRODUCTS ${RUST_LIBPATH}
        COMMENT "Building Rust library (cargo ${CARGO_PROFILE} → ${RUST_LIBPATH})"
        VERBATIM
)

add_custom_target(qiskit_ibm_runtime_build ALL DEPENDS ${RUST_LIBPATH})

add_library(qiskit_ibm_runtime SHARED IMPORTED GLOBAL)
set_property(TARGET qiskit_ibm_runtime PROPERTY IMPORTED_LOCATION ${RUST_LIBPATH})
set_property(TARGET qiskit_ibm_runtime PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${PROJECT_INCLUDEDIR})
add_dependencies(qiskit_ibm_runtime qiskit_ibm_runtime_build)

# ---- Samples -----------------------------------------------------------------
add_executable(test_job_submit samples/test_job_submit.c)
target_include_directories(test_job_submit PRIVATE ${PROJECT_INCLUDEDIR})
target_link_libraries(test_job_submit PRIVATE qiskit qiskit_ibm_runtime)

add_executable(test_ghz_run samples/test_ghz_run.c)
target_include_directories(test_ghz_run PRIVATE ${PROJECT_INCLUDEDIR})
target_link_libraries(test_ghz_run PRIVATE qiskit qiskit_ibm_runtime)

add_executable(test_lucj_fe4s4 samples/test_lucj_fe4s4.c)
target_include_directories(test_lucj_fe4s4 PRIVATE ${PROJECT_INCLUDEDIR})
target_link_libraries(test_lucj_fe4s4 PRIVATE qiskit qiskit_ibm_runtime)

# ---- Tests -------------------------------------------------------------------
enable_testing()

add_executable(test_job_create tests/test_job_create.c)
target_include_directories(test_job_create PRIVATE ${PROJECT_INCLUDEDIR})
target_link_libraries(test_job_create PRIVATE qiskit qiskit_ibm_runtime)
add_test(NAME job_create COMMAND test_job_create)

add_executable(test_qpy tests/test_qpy.c)
target_include_directories(test_qpy PRIVATE ${PROJECT_INCLUDEDIR})
target_link_libraries(test_qpy PRIVATE qiskit qiskit_ibm_runtime)
add_test(NAME qpy COMMAND test_qpy)

# ---- Notes -------------------------------------------------------------------
message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Cargo profile: ${CARGO_PROFILE}")
message(STATUS "qiskit include dir: ${QISKIT_INCDIR}")
message(STATUS "qiskit lib: ${QISKIT_LIBPATH}")
message(STATUS "Rust lib: ${RUST_LIBPATH}")
