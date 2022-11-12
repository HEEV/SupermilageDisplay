
set(FOONATHAN_MEMORY_BUILD_VARS -DBUILD_MEMORY_BUILD_VARS=OFF
                                -DFOONATHAN_MEMORY_BUILD_EXAMPLES=OFF
                                -DFOONATHAN_MEMORY_BUILD_TESTS=OFF
                                -DFOONATHAN_MEMORY_BUILD_TOOLS=ON)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/memory
                                ${FOONATHAN_MEMORY_BUILD_VARS}
                                .
                                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/memory
                                RESULT_VARIABLE configure_ret)
execute_process(COMMAND "${CMAKE_COMMAND}" --build . --config Debug --target install
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/memory
                RESULT_VARIABLE debug_build_ret)
execute_process(COMMAND "${CMAKE_COMMAND}" --build . --config RelWithDebInfo --target install
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/memory
                RESULT_VARIABLE relDeb_build_ret)
execute_process(COMMAND "${CMAKE_COMMAND}" --build . --config Release --target install
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/memory
                RESULT_VARIABLE rel_build_ret)
if(configure_ret OR debug_build_ret OR relDeb_build_ret OR rel_build_ret)
    message(FATAL_ERROR "Failed to create Foonanthan memory: 
                        Configure: ${configure_ret}
                        Debug: ${debug_build_ret}
                        RelDebug: ${relDeb_build_ret}
                        Release: ${rel_build_ret}")
endif()
set(foonathan_memory_DIR ${CMAKE_CURRENT_BINARY_DIR}/memory/lib/foonathan_memory/cmake)
set(THIRDPARTY_Asio FORCE CACHE INTERNAL "" FORCE)
set(THIRDPARTY_fastcdr FORCE CACHE INTERNAL "" FORCE)
set(THIRDPARTY_TinyXML2 FORCE CACHE INTERNAL "" FORCE)
set(COMPILE_TOOLS OFF CACHE INTERNAL "" FORCE)
set(BUILD_TESTING OFF CACHE INTERNAL "" FORCE)
set(SQLITE3_SUPPORT OFF CACHE INTERNAL "" FORCE)

