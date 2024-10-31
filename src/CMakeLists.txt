add_subdirectory(Interpreter)

add_library(lox STATIC ${ALL_OBJECT_FILES})

set(LOX_LIBS lox_interpreter)

target_link_libraries(lox ${LOX_LIBS} ${LOX_THIRDPARTY_LIBS})

target_include_directories(
  lox PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
