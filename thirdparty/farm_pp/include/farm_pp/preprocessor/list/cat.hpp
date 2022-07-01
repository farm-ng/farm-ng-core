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
# ifndef FARM_PP_PREPROCESSOR_LIST_CAT_HPP
# define FARM_PP_PREPROCESSOR_LIST_CAT_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/list/fold_left.hpp>
#
# /* FARM_PP_LIST_CAT */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_CAT(list) FARM_PP_LIST_FOLD_LEFT(FARM_PP_LIST_CAT_O, FARM_PP_LIST_FIRST(list), FARM_PP_LIST_REST(list))
# else
#    define FARM_PP_LIST_CAT(list) FARM_PP_LIST_CAT_I(list)
#    define FARM_PP_LIST_CAT_I(list) FARM_PP_LIST_FOLD_LEFT(FARM_PP_LIST_CAT_O, FARM_PP_LIST_FIRST(list), FARM_PP_LIST_REST(list))
# endif
#
# define FARM_PP_LIST_CAT_O(d, s, x) FARM_PP_CAT(s, x)
#
# /* FARM_PP_LIST_CAT_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_CAT_D(d, list) FARM_PP_LIST_FOLD_LEFT_ ## d(FARM_PP_LIST_CAT_O, FARM_PP_LIST_FIRST(list), FARM_PP_LIST_REST(list))
# else
#    define FARM_PP_LIST_CAT_D(d, list) FARM_PP_LIST_CAT_D_I(d, list)
#    define FARM_PP_LIST_CAT_D_I(d, list) FARM_PP_LIST_FOLD_LEFT_ ## d(FARM_PP_LIST_CAT_O, FARM_PP_LIST_FIRST(list), FARM_PP_LIST_REST(list))
# endif
#
# endif
