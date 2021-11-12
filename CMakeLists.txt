cmake_minimum_required(VERSION 3.19)
project(FFMPEGTest)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 14)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets PrintSupport Network Svg OpenGL MultimediaWidgets QuickWidgets Multimedia Quick Sql XmlPatterns Qml Network Xml WebEngine WebEngineWidgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::Network Qt5::Svg Qt5::OpenGL Qt5::MultimediaWidgets
        Qt5::QuickWidgets Qt5::Multimedia Qt5::Quick Qt5::Sql Qt5::XmlPatterns Qt5::Qml Qt5::Network Qt5::Xml Qt5::WebEngine Qt5::WebEngineWidgets)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdParty/ffmpeg3.4.2/include)
file(GLOB LIB_FFMPEG ${PROJECT_SOURCE_DIR}/thirdParty/ffmpeg3.4.2/lib/*.lib)

link_directories(${PROJECT_SOURCE_DIR}/thirdParty/ffmpeg3.4.2/lib)

aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRC)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

file(GLOB INCLUDE ${PROJECT_SOURCE_DIR}/include/*.h)
add_executable(${PROJECT_NAME} WIN32 main.cpp ${DIR_GUI} ${DIR_SRC} ${INCLUDE} ${GUIBUI_DIR} ${COREBUI_DIR} )

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} ${LIB_FFMPEG})
