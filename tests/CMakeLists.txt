enable_testing()

# Simple test executable (only if test file exists)
file(GLOB TEST_SOURCES "*.cpp")
if(TEST_SOURCES)
    add_executable(orderbook_tests ${TEST_SOURCES})
    target_link_libraries(orderbook_tests orderbook_lib)
    add_test(NAME OrderBookTests COMMAND orderbook_tests)
endif()
