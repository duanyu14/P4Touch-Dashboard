# ESP-IDF v5.5.4 最低要求 CMake 3.16（官方推荐）
cmake_minimum_required(VERSION 3.16)

# 引入 ESP-IDF 核心构建脚本（v5.5.4 标准写法）
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# 项目名称（保持你原有命名）
project(esp32p4_secondary_screen)

# ===================== ESP-IDF v5.5.4 专属配置 =====================
# 1. 自定义组件路径（核心！指向项目内的功能组件）
set(EXTRA_COMPONENT_DIRS
    ${CMAKE_SOURCE_DIR}/components/custom_widgets
    ${CMAKE_SOURCE_DIR}/components/esp_lvgl_adapter
    ${CMAKE_SOURCE_DIR}/components/waveshare_dsi_driver
)

# 2. 编译选项（v5.5.4 兼容的优化/警告配置）
add_compile_options(
    -O2                    # 优化等级：平衡性能与调试（v5.5.4 推荐）
    -Wall                  # 开启基础警告
    -Wextra                # 开启额外警告
    -Wno-unused-parameter  # 屏蔽嵌入式场景常见的"未使用参数"警告
    -Wno-implicit-fallthrough  # 屏蔽"隐式贯穿"警告（switch-case 场景）
)

# 3. LVGL 组件强制配置（v5.5.4 避免内置 LVGL 版本冲突）
idf_build_set_property(COMPONENTS lvgl APPEND)  # 加入 LVGL 组件
set(LVGL_DIR ${CMAKE_SOURCE_DIR}/components/lvgl CACHE PATH "LVGL library path")
# 强制使用项目内 LVGL，禁用 IDF 内置 LVGL（v5.5.4 关键配置）
idf_build_set_property(COMPONENT_REQUIRES "-lvgl" REMOVE)

# 4. 闪存分区与烧录配置（适配 ESP32-P4 + 16MB Flash，v5.5.4 兼容）
set(ESPTOOLPY_FLASH_MODE "dio" CACHE STRING "Flash mode")
set(ESPTOOLPY_FLASH_FREQ "40m" CACHE STRING "Flash frequency")
set(ESPTOOLPY_FLASH_SIZE "16MB" CACHE STRING "Flash size")
# 自定义固件输出路径（v5.5.4 支持该写法）
set(BUILD_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/firmware)
file(MAKE_DIRECTORY ${BUILD_OUTPUT_DIR})
set_property(GLOBAL PROPERTY ESPTOOLPY_BINNAME ${BUILD_OUTPUT_DIR}/${PROJECT_NAME}.bin)

# 5. v5.5.4 特有：启用 C++ 异常（如需 LVGL 高级功能/自定义组件）
idf_build_set_property(CXX_COMPILE_OPTIONS "-fexceptions" APPEND)
idf_build_set_property(C_COMPILE_OPTIONS "-fexceptions" APPEND)

# 6. 打印构建信息（调试用，v5.5.4 兼容）
message(STATUS "========================================")
message(STATUS "ESP-IDF Version: v5.5.4")
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Extra Components: ${EXTRA_COMPONENT_DIRS}")
message(STATUS "LVGL Path: ${LVGL_DIR}")
message(STATUS "Flash Config: ${ESPTOOLPY_FLASH_MODE} | ${ESPTOOLPY_FLASH_FREQ} | ${ESPTOOLPY_FLASH_SIZE}")
message(STATUS "Firmware Output: ${BUILD_OUTPUT_DIR}")
message(STATUS "========================================")

# 执行项目构建（v5.5.4 标准收尾）
project_build_main(${PROJECT_NAME})