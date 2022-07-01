# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_ARRAY_DETAIL_GET_DATA_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_DETAIL_GET_DATA_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/facilities/is_1.hpp>
#
# /* FARM_PP_ARRAY_DETAIL_GET_DATA */
#
# define FARM_PP_ARRAY_DETAIL_GET_DATA_NONE(size, data)

# if !(FARM_PP_VARIADICS_MSVC && _MSC_VER <= 1400)
#    if FARM_PP_VARIADICS_MSVC
#       define FARM_PP_ARRAY_DETAIL_GET_DATA_ANY_VC_DEFAULT(size, data) FARM_PP_TUPLE_REM(size) data
#       define FARM_PP_ARRAY_DETAIL_GET_DATA_ANY_VC_CAT(size, data) FARM_PP_TUPLE_REM_CAT(size) data
#       define FARM_PP_ARRAY_DETAIL_GET_DATA_ANY(size, data) \
            FARM_PP_IIF \
                ( \
                FARM_PP_IS_1(size), \
                FARM_PP_ARRAY_DETAIL_GET_DATA_ANY_VC_CAT, \
                FARM_PP_ARRAY_DETAIL_GET_DATA_ANY_VC_DEFAULT \
                ) \
            (size,data) \
/**/
#    else
#       define FARM_PP_ARRAY_DETAIL_GET_DATA_ANY(size, data) FARM_PP_TUPLE_REM(size) data
#    endif
# else
#    define FARM_PP_ARRAY_DETAIL_GET_DATA_ANY(size, data) FARM_PP_TUPLE_REM(size) data
# endif

# define FARM_PP_ARRAY_DETAIL_GET_DATA(size, data) \
    FARM_PP_IF \
        ( \
        size, \
        FARM_PP_ARRAY_DETAIL_GET_DATA_ANY, \
        FARM_PP_ARRAY_DETAIL_GET_DATA_NONE \
        ) \
    (size,data) \
/**/
#
# endif /* FARM_PP_PREPROCESSOR_ARRAY_DETAIL_GET_DATA_HPP */
