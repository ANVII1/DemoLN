cmake_minimum_required (VERSION 3.8)

# ------------------ Подключение дебагера VISUAL STUDIO
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# ------------------ База
set(PROJECT_NAME DemoLN)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} "src/DemoLN.cpp"  "src/Window/window.hpp" "src/Window/window.cpp")

# ------------------ Выбор стандарта языка
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

set(ExternalPath external)

# ------------------ Сборка Библиотек
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(${ExternalPath}/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(${ExternalPath}/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(${ExternalPath}/glm)
target_link_libraries(${PROJECT_NAME} glm)

# ------------------ Перенос Ресурсов к бинарникам 
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
  COMMENT "Copying Resources"
)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#					COMMAND ${CMAKE_COMMAND} -E copy_directory
#					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)