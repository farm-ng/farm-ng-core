# https://github.com/Sarcasm/cmake-superbuild
# https://github.com/arnaudgelas/ExternalProject/blob/master/External-Boost.cmake
set( Boost_Bootstrap_Command )
if( UNIX )
  set( Boost_Bootstrap_Command ./bootstrap.sh --with-libraries=coroutine,date_time,filesystem,system,thread)
  set( Boost_b2_Command ./b2 )
else()
  if( WIN32 )
    set( Boost_Bootstrap_Command bootstrap.bat )
    set( Boost_b2_Command b2.exe )
  endif()
endif()
ExternalProject_Add (boost
  PREFIX ${farm_ng_EXT_PREFIX}
  URL "https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz"
  URL_HASH SHA256=205666dea9f6a7cfed87c7a6dfbeb52a2c1b9de55712c9c1a87735d7181452b6
  DOWNLOAD_EXTRACT_TIMESTAMP ON
  CONFIGURE_COMMAND ${Boost_Bootstrap_Command}
  BUILD_IN_SOURCE 1
  BUILD_COMMAND  ${Boost_b2_Command} install
    --without-python
    --without-mpi
    --disable-icu
    --prefix=${farm_ng_INSTALL_PREFIX}
    --threading=single,multi
    --link=shared
    --variant=release
    -j8
  INSTALL_COMMAND ""
  INSTALL_DIR ${farm_ng_INSTALL_PREFIX}
  )
