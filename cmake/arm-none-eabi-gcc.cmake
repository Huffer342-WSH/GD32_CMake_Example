#
#
# arm-none-eabi-gcc 交叉编译器，添加到环境变量
# windows下 下载链接https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-win32.zip
#
#

# 调用函数来添加路径到 PATH 环境变量
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

if(MINGW OR CYGWIN OR WIN32)
  set(UTIL_SEARCH_CMD where)
elseif(UNIX OR APPLE)
  set(UTIL_SEARCH_CMD which)
endif()

set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(TOOLCHAIN_SUFFIX .exe)

execute_process(
  COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc${TOOLCHAIN_SUFFIX}
  OUTPUT_VARIABLE BINUTILS_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# set(TOOLCHAIN_PREFIX "E:/SDK-Win/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-")

message("==${BINUTILS_PATH}==")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc${TOOLCHAIN_SUFFIX})
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++${TOOLCHAIN_SUFFIX})

set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}ld${TOOLCHAIN_SUFFIX})

set(CMAKE_AR ${TOOLCHAIN_PREFIX}gcc-ar${TOOLCHAIN_SUFFIX})
set(CMAKE_C_COMPILER_AR ${CMAKE_AR})
set(CMAKE_CXX_COMPILER_AR ${CMAKE_AR})
set(CMAKE_ASM_COMPILER_AR ${CMAKE_AR})

set(CMAKE_RANLIB ${TOOLCHAIN_PREFIX}gcc-ranlib${TOOLCHAIN_SUFFIX})
set(CMAKE_C_COMPILER_RANLIB ${CMAKE_RANLIB})
set(CMAKE_CXX_COMPILER_RANLIB ${CMAKE_RANLIB})
set(CMAKE_ASM_COMPILER_RANLIB ${CMAKE_RANLIB})

set(CMAKE_NM ${TOOLCHAIN_PREFIX}gcc-nm${TOOLCHAIN_SUFFIX})

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy${TOOLCHAIN_SUFFIX})
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PREFIX}size${TOOLCHAIN_SUFFIX})

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# 添加编译指令
add_compile_options(

    # 指定处理器的架构为 ARM Cortex-M4
    -mcpu=cortex-m4

    # 指定浮点单元的类型。fpv4-sp-d16 表示使用单精度浮点单元
    -mfpu=fpv4-sp-d16

    # 指定浮点运算的 ABI（Application Binary Interface）。hard 表示浮点运算将使用硬件浮点寄存器
    -mfloat-abi=hard

    # 指定生成 Thumb 指令集的代码
    -mthumb

    # 将每个函数编译为一个单独的节（section）。这样可以使链接器在需要时仅链接使用到的函数，减小最终的可执行文件的大小
    -ffunction-sections

    # 将每个数据对象编译为一个单独的节
    -fdata-sections

    # 开启大部分警告信息
    -Wall

    # 生成栈使用信息
    -fstack-usage

    #
    --specs=nano.specs
    -pipe
    # --specs=nosys.specs
    "$<$<CONFIG:Debug>:-Og;-DDEBUG;-g;-funwind-tables>"
    "$<$<CONFIG:Release>:-O3;-DNDEBUG>"
    "$<$<CONFIG:MinSizeRel>:-Os;-DNDEBUG>"
    "$<$<CONFIG:RelWithDebInfo>:-O2;-g;-DNDEBUG>"
)

# 添加链接指令
add_link_options(

    # 指定链接时使用的连接脚本
    -T${PROJECT_SOURCE_DIR}/gd32f3x0.ld

    -mcpu=cortex-m4

    # 指定生成 Thumb 指令集的代码
    -mthumb

    # 指定浮点单元的类型。fpv4-sp-d16 表示使用单精度浮点单元
    -mfpu=fpv4-sp-d16

    # 指定浮点运算的 ABI（Application Binary Interface）。hard 表示浮点运算将使用硬件浮点寄存器
    -mfloat-abi=hard

    # 使用 Newlib Nano 库，这是一个更小、更适合嵌入式系统的 C 库，可以减小生成的代码大小并提高执行效率
    # --specs=nano.specs
    --specs=nosys.specs

    # 将 printf 函数与浮点数支持链接
    -u_printf_float

    # 于强制链接静态库，适用于裸机
    -static

    # 启用垃圾回收,删除不使用的段，配个编译选项-ffunction-sections 和 -fdata-sections
    -Wl,--gc-sections
    -Wl,--as-needed

    # 添加libc和libm
    -Wl,--start-group -lc -lm -Wl,--end-group

    # 生成map文件，用于查看生成的可执行文件中的符号、节和地址的映射关系
    -Wl,-Map=output.map
)

