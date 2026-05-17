cmake_minimum_required(VERSION 3.16)
project(vm_simulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(vm_simulator
    main.c
    program_loader.c
    instructions_alu.c
    instructions_control.c
    instructions_memory.c
    instructions_stack.c

)

target_compile_options(vm_simulator PRIVATE -Wall -Wextra)
