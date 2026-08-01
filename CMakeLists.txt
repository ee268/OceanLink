cmake_minimum_required(VERSION 3.16)

project(OceanLink VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 强制MSVC使用UTF-8编码
add_compile_options(/utf-8)

# 查找Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network WidgetsPrivate)

# 查找ElaWidgetTools
set(ELAWIDGETTOOLS_INSTALL_DIR "D:/1code/ElaWidgetTools/Install/ElaWidgetTools" CACHE PATH "ElaWidgetTools prebuilt installation path")
set(ElaWidgetTools_DIR "${ELAWIDGETTOOLS_INSTALL_DIR}/lib/cmake" CACHE PATH "ElaWidgetTools CMake config directory")
find_package(ElaWidgetTools REQUIRED)

# 收集源文件
file(GLOB ORIGIN *.cpp *.h)
file(GLOB COMPONENTS components/*.h components/*.cpp)
file(GLOB CONTROLS components/controls/*.h components/controls/*.cpp)
file(GLOB GLOBAL global/*.h global/*.cpp)
file(GLOB VIEWS components/views/*.h components/views/*.cpp
                components/views/settings/*.h components/views/settings/*.cpp
                components/views/contact/*.h components/views/contact/*.cpp)
file(GLOB NETWORK network/*.cpp network/*.h)

set(PROJECT_SOURCES
    ${ORIGIN}
    ${COMPONENTS}
    ${VIEWS}
    ${CONTROLS}
    ${GLOBAL}
    ${NETWORK}
    OceanLink.qrc
    ./resource/appIcon.rc
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


target_link_libraries(OceanLink PRIVATE Qt5::Widgets)
# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::WidgetsPrivate
    Ela::WidgetTools
)

# 模糊窗口需要链接D3D11和DXGI
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        D3D11
        DXGI
    )
endif()

# 设置属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)
