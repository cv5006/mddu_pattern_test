cmake_minimum_required(VERSION 3.0.0)
project(mddu_pattern_test VERSION 0.1.0)

include(CPack)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CORE
    core/src/main.c
    core/src/threads.c
)

set(COMMON
    common/src/devices.c
    common/src/state_machine.c
    common/src/drive_routine.c
)

set(DEVICE
    device/src/uart_device.c
    device/src/eth_device.c
)

include_directories(core/inc)
include_directories(common/inc)
include_directories(device/inc)

find_package(Threads REQUIRED)

add_executable(main
    ${COMMON}
    ${DEVICE}
    ${CORE}
)
target_link_libraries(main PRIVATE Threads::Threads)

