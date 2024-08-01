# 指定CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和使用的编程语言
project(DOF5RobotControl VERSION 0.1)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
# include_directories(include)

add_subdirectory(lib/RMDControl)
add_subdirectory(lib/NatorControl)

add_library(DOF5RobotControl SHARED DOF5RobotControl.cpp)

target_include_directories(DOF5RobotControl PUBLIC "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/lib/RMDControl" "${PROJECT_SOURCE_DIR}/lib/NatorControl")

target_link_libraries(DOF5RobotControl PUBLIC RMDControl)
target_link_libraries(DOF5RobotControl PUBLIC NatorControl)

target_compile_definitions(DOF5RobotControl PRIVATE "D5R_EXPORTS")

# 定义可执行文件及其源文件
add_executable(D5R_Main main.cpp)

# 链接库到可执行文件

target_include_directories(D5R_Main PUBLIC "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/lib/RMDControl" "${PROJECT_SOURCE_DIR}/lib/NatorControl")

target_link_libraries(D5R_Main PUBLIC RMDControl)
target_link_libraries(D5R_Main PUBLIC NatorControl)
target_link_libraries(D5R_Main PUBLIC DOF5RobotControl)

# 启用测试
# enable_testing()

# 定义测试用例
# add_test(NAME MyTest COMMAND MyExecutable)

# 设置安装规则
# install(TARGETS MyExecutable RUNTIME DESTINATION bin)
# install(FILES datafile.txt DESTINATION share/data)

# 设置包信息
# set(CPACK_PACKAGE_NAME "DOF5RobotControl")
# set(CPACK_PACKAGE_VERSION "${MyProject_VERSION}")
# set(CPACK_PACKAGE_CONTACT "Hin <worranhin@foxmail.com>")
# include(CPack)

# enable_testing()