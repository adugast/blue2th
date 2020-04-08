#########
# CMAKE #
#########

cmake_minimum_required(VERSION 2.8.12)

project(blue2th)

## set compilation flags
set(CMAKE_C_FLAGS "-W -Wall -pedantic -std=c99 -std=gnu99 -lbluetooth")

## set the target name and source
add_executable(
    blue2th
    src/main.c
    src/blue2th.c
)

