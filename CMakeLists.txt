cmake_minimum_required(VERSION 3.8)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(bayan VERSION 0.0.${VERSION_BUILD})


add_executable(
bayan
main.cpp
)

set_property(TARGET bayan PROPERTY CXX_STANDARD 17)

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(bayan
    PRIVATE "${CMAKE_BINARY_DIR}" .
)

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

find_package(Boost REQUIRED COMPONENTS filesystem program_options regex system )	
find_package(OpenSSL COMPONENTS Crypto)

if (${OPENSSL_FOUND})
    target_link_libraries(bayan OpenSSL::Crypto)
    add_definitions(-DWITH_OPENSSL)
else(${OPENSSL_FOUND})
    message(WARNING "OpenSSL::Crypto not found - only Boost hashes will be available")
endif(${OPENSSL_FOUND})

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(bayan ${Boost_LIBRARIES})
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT askimarhond@gmail.com)

include(CPack)
