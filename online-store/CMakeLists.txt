cmake_minimum_required(VERSION 3.10)
project(online_store)

set(CMAKE_CXX_STANDARD 17)

find_package(PostgreSQL REQUIRED)
find_package(libpqxx REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
include_directories(${LIBPQXX_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(online_store
    src/main.cpp
    src/DatabaseConnection.cpp
    src/User.cpp
    src/Order.cpp
    src/OrderItem.cpp
    src/Product.cpp
    src/Payment.cpp
    src/PaymentStrategy.cpp
)

target_link_libraries(online_store
    ${PostgreSQL_LIBRARIES}
    ${LIBPQXX_LIBRARIES}
)
