if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_program(_brew_exec NAMES brew)
    if(NOT _brew_exec STREQUAL "_brew_exec-NOTFOUND")
        execute_process(
            COMMAND ${_brew_exec} --prefix openssl
            OUTPUT_VARIABLE OPENSSL_ROOT_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        find_package(OpenSSL)
    endif()
endif()
