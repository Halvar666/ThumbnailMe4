cmake_minimum_required(VERSION 3.21)

project(ThumbnailMe4 LANGUAGES CXX)

if(WIN32)
    enable_language(RC)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if(WIN32)
    if(NOT DEFINED ENV{FFMPEG_ROOT})
        message(FATAL_ERROR "FFMPEG_ROOT environment variable is not set")
    endif()

    set(FFMPEG_ROOT "$ENV{FFMPEG_ROOT}")
    set(FFMPEG_INCLUDE_DIR "${FFMPEG_ROOT}/include")
    set(FFMPEG_LIB_DIR "${FFMPEG_ROOT}/lib")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
    pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
    pkg_check_modules(AVUTIL REQUIRED IMPORTED_TARGET libavutil)
    pkg_check_modules(SWSCALE REQUIRED IMPORTED_TARGET libswscale)
    pkg_check_modules(SWRESAMPLE REQUIRED IMPORTED_TARGET libswresample)
endif()

add_library(ThumbnailMeCore STATIC
    src/VideoTypes.h
    src/VideoProbe.cpp
    src/VideoProbe.h
    src/FrameGrabber.cpp
    src/FrameGrabber.h
    src/ContactSheetComposer.cpp
    src/ContactSheetComposer.h
    src/MediaInfoReader.cpp
    src/MediaInfoReader.h
)

target_include_directories(ThumbnailMeCore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(WIN32)
    target_include_directories(ThumbnailMeCore PUBLIC
        "${FFMPEG_INCLUDE_DIR}"
    )

    target_link_directories(ThumbnailMeCore PUBLIC
        "${FFMPEG_LIB_DIR}"
    )
endif()

target_link_libraries(ThumbnailMeCore PUBLIC
    Qt6::Widgets
)

if(WIN32)
    target_link_libraries(ThumbnailMeCore PUBLIC
        avformat
        avcodec
        avutil
        swscale
        swresample
    )
else()
    target_link_libraries(ThumbnailMeCore PUBLIC
        PkgConfig::AVFORMAT
        PkgConfig::AVCODEC
        PkgConfig::AVUTIL
        PkgConfig::SWSCALE
        PkgConfig::SWRESAMPLE
    )
endif()

set(THUMBNAILME4_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/app_resources.qrc
)

if(WIN32)
    list(APPEND THUMBNAILME4_SOURCES
        src/app_icon.rc
    )
endif()

if(WIN32)
    add_executable(ThumbnailMe4 WIN32
        ${THUMBNAILME4_SOURCES}
    )
else()
    add_executable(ThumbnailMe4
        ${THUMBNAILME4_SOURCES}
    )
endif()

target_link_libraries(ThumbnailMe4 PRIVATE
    ThumbnailMeCore
)

add_executable(ThumbnailMeWorker
    src/WorkerMain.cpp
)

target_link_libraries(ThumbnailMeWorker PRIVATE
    ThumbnailMeCore
)

if(MSVC)
    target_compile_options(ThumbnailMeCore PRIVATE /utf-8)
    target_compile_options(ThumbnailMe4 PRIVATE /utf-8)
    target_compile_options(ThumbnailMeWorker PRIVATE /utf-8)
endif()
