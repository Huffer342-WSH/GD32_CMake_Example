cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(GD32_CMake_Example)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置二进制执行文件的生成位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/)


# 开启链接时优化
cmake_policy(SET CMP0069 NEW)
option(USE_LTO "Enable LTO" OFF)

if(USE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT supported OUTPUT error)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(isMultiConfig)
    set(CMAKE_CROSS_CONFIGS "Release;Debug")
    set(CMAKE_DEFAULT_BUILD_TYPE "Debug")
    set(CMAKE_DEFAULT_CONFIGS "Release;Debug")
endif()

# 彩色日志输出
set(CMAKE_COLOR_DIAGNOSTICS ON)

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)

if(${FORCE_COLORED_OUTPUT})
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-fdiagnostics-color=always)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-fcolor-diagnostics)
    endif()
endif()

# 添加全局define
add_definitions(
    -DGD32F310
)

# 添加编译指令
add_compile_options(

    "$<$<CONFIG:Debug>:-Og;-DDEBUG;-g;-funwind-tables>"
    "$<$<CONFIG:Release>:-O3;-DNDEBUG>"
    "$<$<CONFIG:MinSizeRel>:-Os;-DNDEBUG>"
    "$<$<CONFIG:RelWithDebInfo>:-O2;-g;-DNDEBUG>"
)

# 添加链接指令
add_link_options(

)

# 设置可执行文件后缀
set(CMAKE_EXECUTABLE_SUFFIX .elf)

# 添加源文件目录
add_subdirectory(Core)