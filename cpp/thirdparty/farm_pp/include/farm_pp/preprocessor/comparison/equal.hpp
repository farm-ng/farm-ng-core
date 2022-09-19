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
# ifndef FARM_PP_PREPROCESSOR_COMPARISON_EQUAL_HPP
# define FARM_PP_PREPROCESSOR_COMPARISON_EQUAL_HPP
#
# include <farm_pp/preprocessor/comparison/not_equal.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/logical/compl.hpp>
#
# /* FARM_PP_EQUAL */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_EQUAL(x, y) FARM_PP_COMPL(FARM_PP_NOT_EQUAL(x, y))
# else
#    define FARM_PP_EQUAL(x, y) FARM_PP_EQUAL_I(x, y)
#    define FARM_PP_EQUAL_I(x, y) FARM_PP_COMPL(FARM_PP_NOT_EQUAL(x, y))
# endif
#
# /* FARM_PP_EQUAL_D */
#
# define FARM_PP_EQUAL_D(d, x, y) FARM_PP_EQUAL(x, y)
#
# endif
