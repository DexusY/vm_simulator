set(VM_SOURCES
    ${CMAKE_SOURCE_DIR}/cpu.c
    ${CMAKE_SOURCE_DIR}/flag.c
    ${CMAKE_SOURCE_DIR}/program_loader.c
    ${CMAKE_SOURCE_DIR}/instructions_alu.c
    ${CMAKE_SOURCE_DIR}/instructions_control.c
    ${CMAKE_SOURCE_DIR}/instructions_memory.c
    ${CMAKE_SOURCE_DIR}/instructions_stack.c
)

set(TEST_TARGETS
    test_alu
    test_memory
    test_stack
    test_control
    test_flag
    test_program_loader
    test_cpu
)

foreach(test_name IN LISTS TEST_TARGETS)
    add_executable(${test_name} ${test_name}.c ${VM_SOURCES})
    target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
