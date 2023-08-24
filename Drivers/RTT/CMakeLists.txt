add_library(${EXEC_NAME}_RTT EXCLUDE_FROM_ALL
	SEGGER_RTT_printf.c
	SEGGER_RTT_Extended.c
	SEGGER_RTT.c
	SEGGER_RTT_ASM_ARMv7M.S
)

target_include_directories(${EXEC_NAME}_RTT PUBLIC
	./
)

target_compile_options(
	${EXEC_NAME}_RTT
	PRIVATE -Og -g
)
