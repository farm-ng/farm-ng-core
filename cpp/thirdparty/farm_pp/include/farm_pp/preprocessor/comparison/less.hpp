# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_COMPARISON_LESS_HPP
# define FARM_PP_PREPROCESSOR_COMPARISON_LESS_HPP
#
# include <farm_pp/preprocessor/comparison/less_equal.hpp>
# include <farm_pp/preprocessor/comparison/not_equal.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/logical/bitand.hpp>
# include <farm_pp/preprocessor/tuple/eat.hpp>
#
# /* FARM_PP_LESS */
#
# if FARM_PP_CONFIG_FLAGS() & (FARM_PP_CONFIG_MWCC() | FARM_PP_CONFIG_DMC())
#    define FARM_PP_LESS(x, y) FARM_PP_BITAND(FARM_PP_NOT_EQUAL(x, y), FARM_PP_LESS_EQUAL(x, y))
# elif ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LESS(x, y) FARM_PP_IIF(FARM_PP_NOT_EQUAL(x, y), FARM_PP_LESS_EQUAL, 0 FARM_PP_TUPLE_EAT_2)(x, y)
# else
#    define FARM_PP_LESS(x, y) FARM_PP_LESS_I(x, y)
#    define FARM_PP_LESS_I(x, y) FARM_PP_IIF(FARM_PP_NOT_EQUAL(x, y), FARM_PP_LESS_EQUAL, 0 FARM_PP_TUPLE_EAT_2)(x, y)
# endif
#
# /* FARM_PP_LESS_D */
#
# if FARM_PP_CONFIG_FLAGS() & (FARM_PP_CONFIG_MWCC() | FARM_PP_CONFIG_DMC())
#    define FARM_PP_LESS_D(d, x, y) FARM_PP_BITAND(FARM_PP_NOT_EQUAL(x, y), FARM_PP_LESS_EQUAL_D(d, x, y))
# elif ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LESS_D(d, x, y) FARM_PP_IIF(FARM_PP_NOT_EQUAL(x, y), FARM_PP_LESS_EQUAL_D, 0 FARM_PP_TUPLE_EAT_3)(d, x, y)
# else
#    define FARM_PP_LESS_D(d, x, y) FARM_PP_LESS_D_I(d, x, y)
#    define FARM_PP_LESS_D_I(d, x, y) FARM_PP_IIF(FARM_PP_NOT_EQUAL(x, y), FARM_PP_LESS_EQUAL_D, 0 FARM_PP_TUPLE_EAT_3)(d, x, y)
# endif
#
# endif
