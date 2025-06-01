cmake_minimum_required(VERSION 3.19)
project(TritonMusic LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动处理 Qt 的元对象系统、资源和 UI 文件
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找 Qt6 组件
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Multimedia)

# 查找 TagLib 库
find_package(TagLib REQUIRED)

# 项目源文件
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/lyricpage.cpp
    src/lyricpage.h
    src/musicInfo.cpp
)

# 生成 Qt 可执行文件
qt_add_executable(TritonMusic
    WIN32 MACOSX_BUNDLE
    ${PROJECT_SOURCES}
)


# 链接 Qt 组件
target_link_libraries(TritonMusic
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia
        TagLib::TagLib
)

target_include_directories(TritonMusic PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src      # src目录
)


# 安装与部署配置（保留部分默认设置）
include(GNUInstallDirs)
install(TARGETS TritonMusic
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TritonMusic
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})