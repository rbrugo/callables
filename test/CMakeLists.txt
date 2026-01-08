# arithmetic tests
add_executable(arithmetic arithmetic.cpp)
target_include_directories(arithmetic PRIVATE include)
target_link_libraries(arithmetic PRIVATE callables)

# bit manipulation tests
add_executable(bit_operators bit_operators.cpp)
target_include_directories(bit_operators PRIVATE include)
target_link_libraries(bit_operators PRIVATE callables)

# function related stuff tests
add_executable(functions functions.cpp)
target_include_directories(functions PRIVATE include)
target_link_libraries(functions PRIVATE callables)
target_compile_options(functions PRIVATE "-fdiagnostics-color=always")  # "-fconcepts-diagnostics-depth=3" 

# comparison related stuff tests
add_executable(comparison comparison.cpp)
target_include_directories(comparison PRIVATE include)
target_link_libraries(comparison PRIVATE callables)

# ordering related stuff tests
add_executable(ordering ordering.cpp)
target_include_directories(ordering PRIVATE include)
target_link_libraries(ordering PRIVATE callables)

# logical operators tests
add_executable(logical logical.cpp)
target_include_directories(logical PRIVATE include)
target_link_libraries(logical PRIVATE callables)
target_compile_options(logical PRIVATE "-fdiagnostics-color=always")

# format tests
add_executable(format format.cpp)
target_include_directories(format PRIVATE include)
target_link_libraries(format PRIVATE callables)
target_compile_options(format PRIVATE "-fdiagnostics-color=always")

add_test(arithmetic arithmetic)
add_test(bit_operators bit_operators)
add_test(functions functions)
add_test(comparison comparison)
add_test(ordering ordering)
add_test(logical logical)
add_test(format format)
