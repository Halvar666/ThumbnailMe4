cmake_minimum_required(VERSION 3.21)

project(ThumbnailMe4 LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if(NOT DEFINED ENV{FFMPEG_ROOT})
    message(FATAL_ERROR "FFMPEG_ROOT environment variable is not set")
endif()

set(FFMPEG_ROOT "$ENV{FFMPEG_ROOT}")
set(FFMPEG_INCLUDE_DIR "${FFMPEG_ROOT}/include")
set(FFMPEG_LIB_DIR "${FFMPEG_ROOT}/lib")

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
    "${FFMPEG_INCLUDE_DIR}"
)

target_link_directories(ThumbnailMeCore PUBLIC
    "${FFMPEG_LIB_DIR}"
)

target_link_libraries(ThumbnailMeCore PUBLIC
    Qt6::Widgets
    avformat
    avcodec
    avutil
    swscale
    swresample
)

add_executable(ThumbnailMe4 WIN32
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/app_resources.qrc
    src/app_icon.rc
)

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
