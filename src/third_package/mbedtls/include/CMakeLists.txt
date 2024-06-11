option(INSTALL_MBEDTLS_HEADERS "Install Mbed TLS headers." ON)

if(INSTALL_MBEDTLS_HEADERS)

    file(GLOB headers "mbedtls/*.h")
    file(GLOB psa_headers "psa/*.h")
    file(GLOB MBEDTLS_SRCS
        
    )

    install(FILES ${headers}
        DESTINATION include/mbedtls
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

    install(FILES ${psa_headers}
        DESTINATION include/psa
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

endif(INSTALL_MBEDTLS_HEADERS)

# Make mbedtls_config.h available in an out-of-source build. ssl-opt.sh requires it.
if (ENABLE_TESTING AND NOT ${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    link_to_source(mbedtls)
    link_to_source(psa)
endif()

# **新增加的部分**
# Collect all necessary source files for mbedtls
file(GLOB MBEDTLS_SRCS
    ../library/aes.c
    ../library/cipher.c
    ../library/ssl_tls.c
    ../library/x509_crt.c
    # Add all other necessary source files here
)

# Define the static library
add_library(mbedtls_static STATIC ${MBEDTLS_SRCS})

# Include the necessary directories
target_include_directories(mbedtls_static PUBLIC ../include)