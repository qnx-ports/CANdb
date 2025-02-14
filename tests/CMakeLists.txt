include(GoogleTest)
include(GNUInstallDirs)

#path to tests on target
set(tests_path ${CMAKE_INSTALL_BINDIR}/CANdb_tests)

function(candb_add_test test_name)
  add_executable(${test_name} ${test_name}.cpp)
if(QNX)
  if(__NTO_VERSION GREATER_EQUAL "800")
    target_link_libraries(${test_name} CANdb gtest gtest_main test_helper regex)
  else()
    target_link_libraries(${test_name} CANdb gtest gtest_main test_helper regex c++fs)
  endif()
  set(artifacts_list ${artifacts_list} ${test_name} PARENT_SCOPE)
else()
  gtest_discover_tests(${test_name})
  target_link_libraries(${test_name} CANdb gtest gtest_main test_helper)
  add_test(NAME ${test_name} COMMAND ${test_name})
  if (TARGET check)
      add_dependencies(check ${test_name})
  else()
      add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
      add_dependencies(check ${test_name})
  endif()
endif()
endfunction()

try_compile(
  HAS_STD_FILESYSTEM "${CMAKE_BINARY_DIR}/temp" "${CMAKE_SOURCE_DIR}/cmake/has_filesystem.cpp"
  CMAKE_FLAGS -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON
)

if(NOT HAS_STD_FILESYSTEM)
    message(STATUS "Tests required C++17 and std::filesystem")
    return()
endif()


add_library(test_helper STATIC test_helper.cpp)
target_link_libraries(test_helper PUBLIC spdlog)
if(QNX)
  target_compile_definitions(test_helper PUBLIC DBC_DIR="dbc/")
else()
  target_compile_definitions(test_helper PUBLIC DBC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/dbc/")
endif()


target_compile_features(test_helper PUBLIC cxx_std_17)
target_compile_definitions(test_helper PUBLIC HAS_STD_FILESYSTEM)

#initialize empty artifacts list
set(artifacts_list CANdb gtest gtest_main)

candb_add_test(dbcparser_tests)
# As opendbc is a submodule we can't use it with Conan
if(NOT CANDB__WITH_CONAN)
    candb_add_test(opendbc_tests)
endif()
candb_add_test(dbcparser_single_tests)
candb_add_test(prop_tests)
candb_add_test(stringutils_tests)

if (QNX)
  install(
    TARGETS ${artifacts_list}
    LIBRARY DESTINATION ${tests_path}/lib
    RUNTIME DESTINATION ${tests_path}
  )
  install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dbc
    DESTINATION ${tests_path}
    FILES_MATCHING PATTERN "*.dbc"
    PATTERN "*/.git*" EXCLUDE
  )
  install(
    PROGRAMS ${QNX_TEST_SCRIPT}
    DESTINATION ${tests_path}
  )
endif ()

if(CANDB__WITH_CONAN)
    add_executable(dbc_parser_benchmark benchmark.cpp)
    target_compile_features(dbc_parser_benchmark PUBLIC cxx_std_17)
    target_link_libraries(dbc_parser_benchmark benchmark CANdb test_helper)
    add_custom_target(benchmarks DEPENDS dbc_parser_benchmark
        COMMAND $<TARGET_FILE:dbc_parser_benchmark>
        )
endif()

find_program(VALGRIND "valgrind")
if(VALGRIND)
  add_custom_target(
    valgrind
    COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ctest
  )
endif()
