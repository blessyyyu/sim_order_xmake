# CMakeList.txt: xmf 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required(VERSION 3.8)
project(xmf)

# 设置RPATH
set(CMAKE_SKIP_BUILD_RPATH FALSE)           # 编译时加上RPATH
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)   # 编译时RPATH不要使用安装的RPATH
set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) # 安装的执行文件加上RPATH

# 查找src目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src DIR_SRCS)

# 将源代码添加到库文件
add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})

# 添加依赖库
target_link_libraries(${PROJECT_NAME} PUBLIC utils)

# 设置导出头文件
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}_export.h)
target_compile_definitions(${PROJECT_NAME} PRIVATE "${PROJECT_NAME}_EXPORTS")

# 添加头文件搜索路径
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${TRD_PARTY_INSTALL_DIR}/include)

# 设置安装属性
set(MY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS ${PROJECT_NAME}
    #PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    #CONFIGURATIONS "Release"
    # 以绝对路径安装。可执行文件无其它输出
    #ARCHIVE DESTINATION ${MY_INSTALL_PATH}
    LIBRARY DESTINATION ${MY_INSTALL_PATH}
    RUNTIME DESTINATION ${MY_INSTALL_PATH}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/
)
