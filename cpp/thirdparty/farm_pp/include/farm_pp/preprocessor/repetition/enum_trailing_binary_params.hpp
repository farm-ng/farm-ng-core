# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_REPETITION_ENUM_TRAILING_BINARY_PARAMS_HPP
# define FARM_PP_PREPROCESSOR_REPETITION_ENUM_TRAILING_BINARY_PARAMS_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/repetition/repeat.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_ENUM_TRAILING_BINARY_PARAMS */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS(count, p1, p2) FARM_PP_REPEAT(count, FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M, (p1, p2))
# else
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS(count, p1, p2) FARM_PP_ENUM_TRAILING_BINARY_PARAMS_I(count, p1, p2)
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_I(count, p1, p2) FARM_PP_REPEAT(count, FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M, (p1, p2))
# endif
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M(z, n, pp) FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M_IM(z, n, FARM_PP_TUPLE_REM_2 pp)
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M_IM(z, n, im) FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M_I(z, n, im)
# else
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M(z, n, pp) FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M_I(z, n, FARM_PP_TUPLE_ELEM(2, 0, pp), FARM_PP_TUPLE_ELEM(2, 1, pp))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M_I(z, n, p1, p2) FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M_II(z, n, p1, p2)
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M_II(z, n, p1, p2) , p1 ## n p2 ## n
# else
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M_I(z, n, p1, p2) , FARM_PP_CAT(p1, n) FARM_PP_CAT(p2, n)
# endif
#
# /* FARM_PP_ENUM_TRAILING_BINARY_PARAMS_Z */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, count, p1, p2) FARM_PP_REPEAT_ ## z(count, FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M, (p1, p2))
# else
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, count, p1, p2) FARM_PP_ENUM_TRAILING_BINARY_PARAMS_Z_I(z, count, p1, p2)
#    define FARM_PP_ENUM_TRAILING_BINARY_PARAMS_Z_I(z, count, p1, p2) FARM_PP_REPEAT_ ## z(count, FARM_PP_ENUM_TRAILING_BINARY_PARAMS_M, (p1, p2))
# endif
#
# endif
