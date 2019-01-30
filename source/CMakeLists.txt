set(PROGRAM_NAME "DirectoryUtils")#设置程序名

FILE(GLOB SRC_FILES "*.cpp")#源文件
FILE(GLOB HEAD_FILES "*.h")#头文件
FILE(GLOB RC_FILES "*.rc")#资源文件

#生成资源文件
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
        ${CMAKE_CURRENT_SOURCE_DIR}/version.rc
        NEWLINE_STYLE UNIX)
#生成版本号头文件
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h
        NEWLINE_STYLE UNIX)

add_executable(exe ${SRC_FILES} ${HEAD_FILES} ${RC_FILES})
set_target_properties(exe PROPERTIES
        OUTPUT_NAME ${PROGRAM_NAME})
