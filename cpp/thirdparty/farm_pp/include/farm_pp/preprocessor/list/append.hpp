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
# ifndef FARM_PP_PREPROCESSOR_LIST_APPEND_HPP
# define FARM_PP_PREPROCESSOR_LIST_APPEND_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/list/fold_right.hpp>
#
# /* FARM_PP_LIST_APPEND */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_APPEND(a, b) FARM_PP_LIST_FOLD_RIGHT(FARM_PP_LIST_APPEND_O, b, a)
# else
#    define FARM_PP_LIST_APPEND(a, b) FARM_PP_LIST_APPEND_I(a, b)
#    define FARM_PP_LIST_APPEND_I(a, b) FARM_PP_LIST_FOLD_RIGHT(FARM_PP_LIST_APPEND_O, b, a)
# endif
#
# define FARM_PP_LIST_APPEND_O(d, s, x) (x, s)
#
# /* FARM_PP_LIST_APPEND_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_APPEND_D(d, a, b) FARM_PP_LIST_FOLD_RIGHT_ ## d(FARM_PP_LIST_APPEND_O, b, a)
# else
#    define FARM_PP_LIST_APPEND_D(d, a, b) FARM_PP_LIST_APPEND_D_I(d, a, b)
#    define FARM_PP_LIST_APPEND_D_I(d, a, b) FARM_PP_LIST_FOLD_RIGHT_ ## d(FARM_PP_LIST_APPEND_O, b, a)
# endif
#
# endif
