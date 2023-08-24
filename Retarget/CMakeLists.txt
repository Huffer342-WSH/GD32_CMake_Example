# 添加静态库 启动文件与启动函数
add_library(${EXEC_NAME}_Retarget EXCLUDE_FROM_ALL
    ./printf/printf.c
    ./retarget.c
    ./scanf/scanf.c
)


target_include_directories(${EXEC_NAME}_Retarget PUBLIC
    ${PROJECT_SOURCE_DIR}/Drivers/GD32F3x0_standard_peripheral/Include
    ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/GD/GD32F3x0/Include
    ${PROJECT_SOURCE_DIR}/Drivers/CMSIS
    ${PROJECT_SOURCE_DIR}/Core/inc
    ./printf
    ./scanf
)

target_compile_options(${EXEC_NAME}_Retarget
    PRIVATE -Os
)

# target_link_libraries(${EXEC_NAME}_CMSIS ${EXEC_NAME}_standard_peripherals)