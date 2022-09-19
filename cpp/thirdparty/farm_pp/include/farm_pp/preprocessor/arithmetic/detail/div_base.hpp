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
# ifndef FARM_PP_PREPROCESSOR_ARITHMETIC_DETAIL_DIV_BASE_HPP
# define FARM_PP_PREPROCESSOR_ARITHMETIC_DETAIL_DIV_BASE_HPP
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/arithmetic/sub.hpp>
# include <farm_pp/preprocessor/comparison/less_equal.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_DIV_BASE */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_DIV_BASE(x, y) FARM_PP_WHILE(FARM_PP_DIV_BASE_P, FARM_PP_DIV_BASE_O, (0, x, y))
# else
#    define FARM_PP_DIV_BASE(x, y) FARM_PP_DIV_BASE_I(x, y)
#    define FARM_PP_DIV_BASE_I(x, y) FARM_PP_WHILE(FARM_PP_DIV_BASE_P, FARM_PP_DIV_BASE_O, (0, x, y))
# endif
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_DIV_BASE_P(d, rxy) FARM_PP_DIV_BASE_P_IM(d, FARM_PP_TUPLE_REM_3 rxy)
#    define FARM_PP_DIV_BASE_P_IM(d, im) FARM_PP_DIV_BASE_P_I(d, im)
# else
#    define FARM_PP_DIV_BASE_P(d, rxy) FARM_PP_DIV_BASE_P_I(d, FARM_PP_TUPLE_ELEM(3, 0, rxy), FARM_PP_TUPLE_ELEM(3, 1, rxy), FARM_PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define FARM_PP_DIV_BASE_P_I(d, r, x, y) FARM_PP_LESS_EQUAL_D(d, y, x)
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_DIV_BASE_O(d, rxy) FARM_PP_DIV_BASE_O_IM(d, FARM_PP_TUPLE_REM_3 rxy)
#    define FARM_PP_DIV_BASE_O_IM(d, im) FARM_PP_DIV_BASE_O_I(d, im)
# else
#    define FARM_PP_DIV_BASE_O(d, rxy) FARM_PP_DIV_BASE_O_I(d, FARM_PP_TUPLE_ELEM(3, 0, rxy), FARM_PP_TUPLE_ELEM(3, 1, rxy), FARM_PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define FARM_PP_DIV_BASE_O_I(d, r, x, y) (FARM_PP_INC(r), FARM_PP_SUB_D(d, x, y), y)
#
# /* FARM_PP_DIV_BASE_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_DIV_BASE_D(d, x, y) FARM_PP_WHILE_ ## d(FARM_PP_DIV_BASE_P, FARM_PP_DIV_BASE_O, (0, x, y))
# else
#    define FARM_PP_DIV_BASE_D(d, x, y) FARM_PP_DIV_BASE_D_I(d, x, y)
#    define FARM_PP_DIV_BASE_D_I(d, x, y) FARM_PP_WHILE_ ## d(FARM_PP_DIV_BASE_P, FARM_PP_DIV_BASE_O, (0, x, y))
# endif
#
# endif
