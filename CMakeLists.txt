cmake_minimum_required(VERSION 3.5)

# 设置项目名称
project(SFML_PRACTICE)

# 导出compile_commands.json文件
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 设置C++编译器
set(CMAKE_CXX_COMPILER g++)
# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Werror -Wall")
# 查找SFML库
set(LIB_SFML $ENV{SFML}/lib)
set(SFML_DIR ${LIB_SFML}/cmake/SFML)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# 添加源文件目录
add_subdirectory(src)