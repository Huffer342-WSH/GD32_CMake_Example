# 添加静态库 启动文件与启动函数
add_library(${EXEC_NAME}_CMSIS EXCLUDE_FROM_ALL
	GD/GD32F3x0/Source/GNU/startup_gd32f3x0.s
	GD/GD32F3x0/Source/system_gd32f3x0.c
)

target_include_directories(${EXEC_NAME}_CMSIS PUBLIC
	GD/GD32F3x0/Include
	${PROJECT_SOURCE_DIR}/Core/inc
	./
)

target_compile_options(${EXEC_NAME}_CMSIS
	PRIVATE -Os
)
target_link_libraries(${EXEC_NAME}_CMSIS ${EXEC_NAME}_standard_peripherals)