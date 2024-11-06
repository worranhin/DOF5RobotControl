cmake_minimum_required(VERSION 3.10) # 设置CMake的最低版本需求

project(NatorControl VERSION 0.1) # 定义项目名称

# # 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(NatorControl NatorControl.cpp)
add_library(NatorMotor NatorMotor.cpp)

add_library(NTSDK SHARED IMPORTED)

set_target_properties(NTSDK PROPERTIES
  IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/SDK/lib64/NTControl.lib"
)

set_target_properties(NTSDK PROPERTIES
  IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/SDK/lib64/NTControl.dll"
)

target_link_libraries(NatorControl NTSDK)
target_link_libraries(NatorMotor NTSDK)

# add_executable(main main.c)

# target_link_libraries(main NatorControl)