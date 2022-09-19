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
# ifndef FARM_PP_PREPROCESSOR_SELECTION_MAX_HPP
# define FARM_PP_PREPROCESSOR_SELECTION_MAX_HPP
#
# include <farm_pp/preprocessor/comparison/less_equal.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
#
# /* FARM_PP_MAX */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_MAX(x, y) FARM_PP_IIF(FARM_PP_LESS_EQUAL(x, y), y, x)
# else
#    define FARM_PP_MAX(x, y) FARM_PP_MAX_I(x, y)
#    define FARM_PP_MAX_I(x, y) FARM_PP_IIF(FARM_PP_LESS_EQUAL(x, y), y, x)
# endif
#
# /* FARM_PP_MAX_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_MAX_D(d, x, y) FARM_PP_IIF(FARM_PP_LESS_EQUAL_D(d, x, y), y, x)
# else
#    define FARM_PP_MAX_D(d, x, y) FARM_PP_MAX_D_I(d, x, y)
#    define FARM_PP_MAX_D_I(d, x, y) FARM_PP_IIF(FARM_PP_LESS_EQUAL_D(d, x, y), y, x)
# endif
#
# endif
