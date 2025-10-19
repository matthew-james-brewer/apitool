cmake_minimum_required(VERSION 3.20)
project(APItool4C LANGUAGES C)

if (MSVC)
    add_compile_options(/W4 /Ox)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

# APItool.c includes all the other .c files

add_executable(apitool4c main.c APItool.c)

install(TARGETS apitool4c RUNTIME DESTINATION bin)
install(FILES APItool.h DESTINATION include)
install(FILES apitool.nanorc DESTINATION share/nano)
