add_executable(TinyMaixD1
    boot/interrupt.c
    boot/start.S
    boot/memset.S
    boot/memcpy.S
    boot/trap.S
    common/common.c
    common/printf.c
    drivers/clint.c
    drivers/clk.c
    drivers/gpio.c
    drivers/plint.c
    drivers/timer.c
    drivers/uart.c
    drivers/watchdog.c
    main.c
)
set_target_properties(TinyMaixD1 PROPERTIES LINK_DEPENDS "${LINK_SCRIPT}")
target_link_libraries(TinyMaixD1 PRIVATE)

add_custom_command(TARGET TinyMaixD1 POST_BUILD COMMAND ${CMAKE_OBJCOPY} -v -O binary TinyMaixD1 TinyMaixD1.bin COMMENT "Copy Binary")