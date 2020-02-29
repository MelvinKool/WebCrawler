cmake_minimum_required(VERSION 3.9)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.249.tar.gz"
    SHA1 "d45d77d8bba9da13e9290a180e0477e90accd89b"
)

project(WebCrawler)

hunter_add_package(CURL)
hunter_add_package(cxxopts)
hunter_add_package(gumbo)
# hunter package for mysql-client is horribly out of date, maybe work with upstream to add a package for mariadb-connector-c
# hunter_add_package(MySQL-client)
hunter_add_package(spdlog)

find_package(CURL REQUIRED)
find_package(cxxopts REQUIRED)
find_package(gumbo REQUIRED)
# find_package(MySQL-client REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
target_link_libraries(${PROJECT_NAME} PUBLIC CURL::libcurl cxxopts::cxxopts gumbo::gumbo "mysqlclient" spdlog::spdlog)
