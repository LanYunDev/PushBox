cmake_minimum_required(VERSION 3.22) #设置CMake的最低版本

project(PushBox
        LANGUAGES C
        VERSION 1.1.4
        DESCRIPTION "免费，开源，推箱子小游戏.A Free, Open Source, PushBox Game."
        HOMEPAGE_URL https://github.com/LanYunDev/PushBox
        ) #设置项目名称,语言类型，项目版本号,项目描述，项目主页。

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR) #判断项目目录是否相同
    message(WARNING "PROJECT_BINARY_DIR and PROJECT_SOURCE_DIR are the same. This is not recommended.") #输出警告信息
endif () #结束

if (NOT CMAKE_BUILD_TYPE)   #判断是否有环境变量CMAKE_BUILD_TYPE
    set(CMAKE_BUILD_TYPE Release) #如果没有，则设置为Release
endif () #结束

if (WIN32) #判断是否是Windows平台
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES) #在windows下，禁用min和max宏，并设置环境变量，防止C++编译器报错
endif () #结束

if (NOT MSVC) #判断是否MSVC编译器,不是则进入.
    find_program(CCACHE_PROGRAM ccache) #查找ccache编译器.
    if (CCACHE_PROGRAM) #如果有CCache编译器,则设置环境变量,能让编译带有缓存功能,提高编译速度,提高编译效率,提高编译稳定性.
        message(STATUS "Found CCache: CCACHE_PROGRAM: ${CCACHE_PROGRAM}") #输出提示信息
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM}) #设置编译命令
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM}) #设置链接命令
    endif () #结束
endif () #结束

# 若用户未指定BUILD_SHARED_LIBS，默认为OFF，即编译为STATIC静态库.设置为ON，则编译为SHARED动态库.不过推荐使用OBJECT对象库(此处未展开)，虽然动态库更加稳定，更加安全.不过对Windows不友好.
if (NOT DEFINED BUILD_SHARED_LIBS) #判断是否定义变量BUILD_SHARED_LIBS
    set(BUILD_SHARED_LIBS ON) #如果没有，则设置为ON
endif () #结束

set(CMAKE_C_STANDARD 23) #设置C语言标准为C 23
set(CMAKE_C_STANDARD_REQUIRED ON) #编译器必须支持C 23，否则出错
set(CMAKE_C_EXTENSIONS ON) #开启GCC编译器对C语言扩展，只支持GCC编译器，降低对(Windows平台的)MSVC的兼容性

find_package(Curses REQUIRED) #查找Curses库

include_directories(${CURSES_INCLUDE_DIR}) #添加Curses库的头文件目录

file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.h"
        ) #GLOB自动查找当前目录下指定拓展名的文件📃，实现批量添加源文件。添加CONFIGURE_DEPENDS选项，当添加新文件或源文件发生变化时，CMake会自动重新编译项目。

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES}) #可执行文件名为PushBox，源文件为PushBox.c

if (CMAKE_SYSTEM_NAME MATCHES "Darwin") #判断是否是Mac平台
    target_link_libraries(${CMAKE_PROJECT_NAME} ${CURSES_LIBRARY}) #链接Curses库
ELSE () #其他Linux平台
    target_link_libraries(${CMAKE_PROJECT_NAME} -lncursesw) #链接Curses库
endif () #结束
