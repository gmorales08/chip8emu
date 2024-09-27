set(SRCS "main.cpp"
         "emulator.cpp"
         "machine.cpp"
         "graphics.cpp"
         "input.cpp"
)

add_executable(${PROJECT_NAME} ${SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -pedantic
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
)
