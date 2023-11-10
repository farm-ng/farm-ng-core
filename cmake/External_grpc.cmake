# When using Brew on MacOS, OpenSSL is not installed in a standard search
# location. This will attempt to find it.
include(farm_ng_macos_ssl_fix)

# inspired by:
# https://github.com/grpc/grpc/blob/master/examples/cpp/helloworld/cmake_externalproject/CMakeLists.txt
ExternalProject_Add(absl
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 273292d1cfc0a94a65082ee350509af1d113344d # 20220623.rc1, 20220623.0)
  GIT_SHALLOW ON
  PREFIX ${farm_ng_EXT_PREFIX}
  CMAKE_ARGS
    ${farm_ng_DEFAULT_ARGS}
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_TESTING=OFF
    -DBUILD_SHARED_LIBS=ON
    -DABSL_PROPAGATE_CXX_STD=OFF
    -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
 )

ExternalProject_Add(c-ares
  GIT_REPOSITORY https://github.com/c-ares/c-ares.git
  GIT_TAG 6654436a307a5a686b008c1d4c93b0085da6e6d8 # cares-1_17_2
  GIT_SHALLOW ON
  PREFIX ${farm_ng_EXT_PREFIX}
  CMAKE_ARGS
      ${farm_ng_DEFAULT_ARGS}
      -DCMAKE_BUILD_TYPE=Release
      -DCARES_SHARED:BOOL=ON
      -DCARES_STATIC:BOOL=OFF
      -DCARES_STATIC_PIC:BOOL=ON
      )

ExternalProject_Add(protobuf
  GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
  GIT_TAG 24487dd1045c7f3d64a21f38a3f0c06cc4cf2edb # v3.21.6
  GIT_SHALLOW ON
  PREFIX ${farm_ng_EXT_PREFIX}
  CMAKE_ARGS
  ${farm_ng_DEFAULT_ARGS}
  -DCMAKE_BUILD_TYPE=Release
  -Dprotobuf_BUILD_TESTS:BOOL=OFF
  -Dprotobuf_WITH_ZLIB:BOOL=OFF
  -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF
)

ExternalProject_Add(re2
  GIT_REPOSITORY https://github.com/google/re2.git
  GIT_TAG 8e08f47b11b413302749c0d8b17a1c94777495d5 # 2021-09-01
  GIT_SHALLOW ON
  PREFIX ${farm_ng_EXT_PREFIX}
  CMAKE_ARGS
  ${farm_ng_DEFAULT_ARGS}
  -DCMAKE_BUILD_TYPE=Release
  -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
  -DBUILD_TESTING=OFF
  -DRE2_BUILD_TESTING=OFF
  )

ExternalProject_Add(grpc
  DEPENDS c-ares protobuf re2 absl
  GIT_REPOSITORY https://github.com/grpc/grpc.git
  GIT_TAG 0a82c02a9b817a53574994374dcff53f2e961df2 # (tag: v1.51.1)
  GIT_SHALLOW ON
  PREFIX ${farm_ng_EXT_PREFIX}
  CMAKE_ARGS
  ${farm_ng_DEFAULT_ARGS}
  -DCMAKE_BUILD_TYPE=Release
  -DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR} # Populated on MacOS
  -DgRPC_INSTALL:BOOL=ON
  -DgRPC_BUILD_TESTS:BOOL=OFF
  -DgRPC_BUILD_MSVC_MP_COUNT:STRING=-1
  -DgRPC_PROTOBUF_PROVIDER:STRING=package
  -DgRPC_PROTOBUF_PACKAGE_TYPE:STRING=CONFIG
  -DgRPC_RE2_PROVIDER:STRING=package
  -DgRPC_ZLIB_PROVIDER:STRING=package
  -DgRPC_ABSL_PROVIDER:STRING=package
  -DgRPC_CARES_PROVIDER:STRING=package
  -DgRPC_SSL_PROVIDER:STRING=package
)
