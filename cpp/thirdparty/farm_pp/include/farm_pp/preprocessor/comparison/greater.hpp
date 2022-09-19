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
# ifndef FARM_PP_PREPROCESSOR_COMPARISON_GREATER_HPP
# define FARM_PP_PREPROCESSOR_COMPARISON_GREATER_HPP
#
# include <farm_pp/preprocessor/comparison/less.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_GREATER */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_GREATER(x, y) FARM_PP_LESS(y, x)
# else
#    define FARM_PP_GREATER(x, y) FARM_PP_GREATER_I(x, y)
#    define FARM_PP_GREATER_I(x, y) FARM_PP_LESS(y, x)
# endif
#
# /* FARM_PP_GREATER_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_GREATER_D(d, x, y) FARM_PP_LESS_D(d, y, x)
# else
#    define FARM_PP_GREATER_D(d, x, y) FARM_PP_GREATER_D_I(d, x, y)
#    define FARM_PP_GREATER_D_I(d, x, y) FARM_PP_LESS_D(d, y, x)
# endif
#
# endif
