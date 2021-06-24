cmake_minimum_required(VERSION 3.5)

project(ScheidSpotify VERSION 0.1 LANGUAGES CXX)

enable_testing(true)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SPOTIFY_CLIENT_KEY $ENV{SPOTIFY_CLIENT_KEY})
set(SPOTIFY_CLIENT_SECRET $ENV{SPOTIFY_CLIENT_SECRET})

# cria o header com a versão
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/definitions.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/definitions.h
)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Network NetworkAuth  Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network NetworkAuth  Test REQUIRED)

find_package(OpenSSL REQUIRED)
if( OpenSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    link_directories(${OPENSSL_LIBRARIES})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
endif()

file(GLOB_RECURSE PROJECT_TESTS
    "test/*.h"
    "test/*.hpp"
    "test/*.cpp"
)

file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.h"
    "src/*.hpp"
    "src/*.cpp"
    "src/*.ui"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ScheidSpotify
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_add_executable(ScheidSpotifyTest
        MANUAL_FINALIZATION
        ${PROJECT_TESTS}
        src/spotifywrapper.h
        src/spotifywrapper.cpp
    )

else()
    if(ANDROID)
        add_library(ScheidSpotify SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ScheidSpotify
            ${PROJECT_SOURCES}
        )
        add_executable(ScheidSpotifyTest
            ${PROJECT_TESTS}
            src/spotifywrapper.h
            src/spotifywrapper.cpp
        )
    endif()
endif()

target_link_libraries(ScheidSpotify PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::NetworkAuth ${OPENSSL_LIBRARIES})

target_link_libraries(ScheidSpotifyTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::NetworkAuth Qt${QT_VERSION_MAJOR}::Test ${OPENSSL_LIBRARIES})

set_target_properties(ScheidSpotify PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ScheidSpotify)
    qt_finalize_executable(ScheidSpotifyTest)
endif()

add_test(ScheidSpotifyTest ${PROJECT_TESTS})
