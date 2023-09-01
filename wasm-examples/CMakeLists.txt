include_directories(${CMAKE_SOURCE_DIR})

if(BUILD_SHARED_LIBS)
  target_compile_definitions(sherpa-ncnn-wasm PRIVATE SHERPA_NCNN_BUILD_SHARED_LIBS=1)
  target_compile_definitions(sherpa-ncnn-wasm PRIVATE SHERPA_NCNN_BUILD_MAIN_LIB=1)
endif()

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s FORCE_FILESYSTEM=1 -s INITIAL_MEMORY=256MB -s EXIT_RUNTIME=1 -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=['$$stringToNewUTF8','$$UTF8ToString']")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s FORCE_FILESYSTEM=1 -s INITIAL_MEMORY=256MB -s EXIT_RUNTIME=1 -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=['$$stringToNewUTF8','$$UTF8ToString']")
# set(CMAKE_EXECUTBLE_LINKER_FLAGS "${CMAKE_EXECUTBLE_LINKER_FLAGS} -s FORCE_FILESYSTEM=1 -s INITIAL_MEMORY=256MB -s EXIT_RUNTIME=1 -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=['$$stringToNewUTF8','$$UTF8ToString']")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s FORCE_FILESYSTEM=1 -s INITIAL_MEMORY=256MB -s EXIT_RUNTIME=1 -sASSERTIONS -sALLOW_MEMORY_GROWTH -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=['$$stringToNewUTF8','$$UTF8ToString'] -s EXPORTED_RUNTIME_METHODS=['getValue']")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s FORCE_FILESYSTEM=1 -s INITIAL_MEMORY=256MB -s EXIT_RUNTIME=1 -sASSERTIONS -sALLOW_MEMORY_GROWTH -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=['$$stringToNewUTF8','$$UTF8ToString'] -s EXPORTED_RUNTIME_METHODS=['getValue']")
set(CMAKE_EXECUTBLE_LINKER_FLAGS "${CMAKE_EXECUTBLE_LINKER_FLAGS} -s FORCE_FILESYSTEM=1 -s INITIAL_MEMORY=256MB -s EXIT_RUNTIME=1 -sASSERTIONS -sALLOW_MEMORY_GROWTH -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=['$$stringToNewUTF8','$$UTF8ToString'] -s EXPORTED_RUNTIME_METHODS=['getValue']")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sEXPORTED_FUNCTIONS=['_sherpa_recognize_ncnn','_sherpa_query_text','_sherpa_query_n_timestamp','_sherpa_query_timestamps','_malloc','_free'] --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@.")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sEXPORTED_FUNCTIONS=['_sherpa_recognize_ncnn','_sherpa_query_text','_sherpa_query_n_timestamp','_sherpa_query_timestamps','_malloc','_free'] --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@.")
set(CMAKE_EXECUTBLE_LINKER_FLAGS "${CMAKE_EXECUTBLE_LINKER_FLAGS} -sEXPORTED_FUNCTIONS=['_sherpa_recognize_ncnn','_sherpa_query_text','_sherpa_query_n_timestamp','_sherpa_query_timestamps','_malloc','_free'] --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@.")

# It's a library but it should be compiler using `add_executable`.
add_executable(sherpa-ncnn-wasm-${WASM_FEATURE}-example wasm.cc)
target_link_libraries(sherpa-ncnn-wasm-${WASM_FEATURE}-example PUBLIC sherpa-ncnn-core)

# Create directory (ignore if exists).
add_custom_command(TARGET sherpa-ncnn-wasm-${WASM_FEATURE}-example POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/dist/
)
# Copy file into dist directory.
add_custom_command(TARGET sherpa-ncnn-wasm-${WASM_FEATURE}-example POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different 
  ${CMAKE_CURRENT_SOURCE_DIR}/index.html
  ${CMAKE_CURRENT_SOURCE_DIR}/serve.py
  ${CMAKE_BINARY_DIR}/bin/sherpa-ncnn-wasm-${WASM_FEATURE}-example.data
  ${CMAKE_BINARY_DIR}/bin/sherpa-ncnn-wasm-${WASM_FEATURE}-example.js
  ${CMAKE_BINARY_DIR}/bin/sherpa-ncnn-wasm-${WASM_FEATURE}-example.wasm
  ${CMAKE_BINARY_DIR}/bin/sherpa-ncnn-wasm-${WASM_FEATURE}-example.worker.js
  ${CMAKE_CURRENT_BINARY_DIR}/dist/
)


