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

add_test(arithmetic arithmetic)
add_test(bit_operators bit_operators)
add_test(functions functions)
add_test(comparison comparison)
add_test(ordering ordering)
add_test(logical ordering)
