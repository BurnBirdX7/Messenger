
set (OPENSSL_INC_DIR ""
        CACHE PATH "Path to OpenSSL-compatible library's include files")

set (OPENSSL_LIBSLL ""
        CACHE FILEPATH "Path to OpenSSL's (or analog's) libssl")

set (OPENSSL_LIBCRYPTO ""
        CACHE FILEPATH "Path to OpenSSL's (or analog's) libcrypto")

if (NOT "${OPENSSL_LIBSLL}" STREQUAL "")
    message("LibSSL - provided ~ ${OPENSSL_LIBSLL}")
else()
    message(SEND_ERROR "LibSSL - not provided")
endif()

if (NOT "${OPENSSL_LIBCRYPTO}" STREQUAL "")
    message("LibCrypto - provided ~ ${OPENSSL_LIBCRYPTO}")
else()
    message(SEND_ERROR "LibCrypto - not provided")
endif()

if (NOT "${OPENSSL_INC_DIR}" STREQUAL "")
    message("OpenSSL includes - provided ~ ${OPENSSL_INC_DIR}")
else()
    message(SEND_ERROR "OpenSSL includes - not provided")
endif()


add_library(LibSSL STATIC IMPORTED)
add_library(LibCrypto STATIC IMPORTED)

set_property(TARGET LibSSL
        PROPERTY IMPORTED_LOCATION ${OPENSSL_LIBSLL})

set_property(TARGET LibCrypto
        PROPERTY IMPORTED_LOCATION ${OPENSSL_LIBCRYPTO})

include_directories(${OPENSSL_INC_DIR})

