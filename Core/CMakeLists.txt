cmake_minimum_required(VERSION 3.17.0)

# 设置可执行文件目标名称
set(EXEC_NAME "${PROJECT_NAME}")

# 添加可执行文件
add_executable(${EXEC_NAME})

# 添加头文件目录
target_include_directories(${EXEC_NAME} PRIVATE
	./inc
)

# 添加源文件
FILE(GLOB_RECURSE CORE_SRC src/*.c)

target_sources(${EXEC_NAME} PRIVATE

	${CORE_SRC}
)

# 添加库
include(add_libs)

# 添加编译后命令
include(create_bin)

message("\nEXECUTABLE_OUTPUT_PATH:${EXECUTABLE_OUTPUT_PATH}\n")

# 添加编译后命令-打印可执行文件尺寸信息
add_custom_command(
	TARGET ${EXEC_NAME}
	POST_BUILD
	COMMAND echo executable size info
	COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE_OUTPUT_PATH}/${EXEC_NAME}.elf
	VERBATIM
)

# 定义自定义构建目标：下载到目标单片机
set(OPENOCD_EXECUTABLE "E:/SDK-Win/OpenOCD/bin/openocd.exe")
# set(OPENOCD_EXECUTABLE "openocd.exe")

install(CODE
	CODE "MESSAGE(\"Flash......\")"
	CODE "execute_process(COMMAND ${OPENOCD_EXECUTABLE} -f ${PROJECT_SOURCE_DIR}/OpenOCD/openocd.cfg -c \"program ${EXECUTABLE_OUTPUT_PATH}/${EXEC_NAME}.elf verify reset\" -c shutdown)"
)
