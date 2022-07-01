# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_ARRAY_ENUM_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_ENUM_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/array/size.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
# include <farm_pp/preprocessor/variadic/has_opt.hpp>
#
# /* FARM_PP_ARRAY_ENUM */
#
# if FARM_PP_VARIADIC_HAS_OPT()
#    if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#        define FARM_PP_ARRAY_ENUM_NOT_EMPTY(array) FARM_PP_ARRAY_ENUM_I(FARM_PP_TUPLE_REM_CTOR, array)
#        define FARM_PP_ARRAY_ENUM_I(m, args) FARM_PP_ARRAY_ENUM_II(m, args)
#        define FARM_PP_ARRAY_ENUM_II(m, args) FARM_PP_CAT(m ## args,)
#    elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#        define FARM_PP_ARRAY_ENUM_NOT_EMPTY(array) FARM_PP_ARRAY_ENUM_I(array)
#        define FARM_PP_ARRAY_ENUM_I(array) FARM_PP_TUPLE_REM_CTOR ## array
#    else
#        define FARM_PP_ARRAY_ENUM_NOT_EMPTY(array) FARM_PP_TUPLE_REM_CTOR array
#    endif
#    define FARM_PP_ARRAY_ENUM_EMPTY(array)
#    define FARM_PP_ARRAY_ENUM(array) FARM_PP_IF(FARM_PP_ARRAY_SIZE(array),FARM_PP_ARRAY_ENUM_NOT_EMPTY,FARM_PP_ARRAY_ENUM_EMPTY)(array)
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_ARRAY_ENUM(array) FARM_PP_ARRAY_ENUM_I(FARM_PP_TUPLE_REM_CTOR, array)
#    define FARM_PP_ARRAY_ENUM_I(m, args) FARM_PP_ARRAY_ENUM_II(m, args)
#    define FARM_PP_ARRAY_ENUM_II(m, args) FARM_PP_CAT(m ## args,)
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_ARRAY_ENUM(array) FARM_PP_ARRAY_ENUM_I(array)
#    define FARM_PP_ARRAY_ENUM_I(array) FARM_PP_TUPLE_REM_CTOR ## array
# else
#    define FARM_PP_ARRAY_ENUM(array) FARM_PP_TUPLE_REM_CTOR array
# endif
#
# endif
