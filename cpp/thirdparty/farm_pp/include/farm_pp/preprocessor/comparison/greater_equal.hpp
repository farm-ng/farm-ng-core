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
# ifndef FARM_PP_PREPROCESSOR_COMPARISON_GREATER_EQUAL_HPP
# define FARM_PP_PREPROCESSOR_COMPARISON_GREATER_EQUAL_HPP
#
# include <farm_pp/preprocessor/comparison/less_equal.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_GREATER_EQUAL */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_GREATER_EQUAL(x, y) FARM_PP_LESS_EQUAL(y, x)
# else
#    define FARM_PP_GREATER_EQUAL(x, y) FARM_PP_GREATER_EQUAL_I(x, y)
#    define FARM_PP_GREATER_EQUAL_I(x, y) FARM_PP_LESS_EQUAL(y, x)
# endif
#
# /* FARM_PP_GREATER_EQUAL_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_GREATER_EQUAL_D(d, x, y) FARM_PP_LESS_EQUAL_D(d, y, x)
# else
#    define FARM_PP_GREATER_EQUAL_D(d, x, y) FARM_PP_GREATER_EQUAL_D_I(d, x, y)
#    define FARM_PP_GREATER_EQUAL_D_I(d, x, y) FARM_PP_LESS_EQUAL_D(d, y, x)
# endif
#
# endif
