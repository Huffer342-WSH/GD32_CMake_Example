# 添加子文件夹 ，将 /Drivers目录下的标准库编译成静态库
add_subdirectory(${PROJECT_SOURCE_DIR}/Drivers Drivers)
add_subdirectory(${PROJECT_SOURCE_DIR}/Retarget Retarget)

# 添加头文件搜索路径
target_include_directories(${EXEC_NAME}_CMSIS PRIVATE .)
target_include_directories(${EXEC_NAME}_standard_peripherals PRIVATE .)

# 添加静态库 包括 /Drivers/CMSIS 和 /Drivers/GD32F3x0_standard_peripheral 目录下文件编译得到的静态库
target_link_libraries(${EXEC_NAME}
    ${EXEC_NAME}_CMSIS
    ${EXEC_NAME}_standard_peripherals
    ${EXEC_NAME}_Retarget
    ${EXEC_NAME}_RTT
)