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
# ifndef FARM_PP_PREPROCESSOR_LIST_REST_N_HPP
# define FARM_PP_PREPROCESSOR_LIST_REST_N_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_LIST_REST_N */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_REST_N(count, list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_LIST_REST_N_P, FARM_PP_LIST_REST_N_O, (list, count)))
# else
#    define FARM_PP_LIST_REST_N(count, list) FARM_PP_LIST_REST_N_I(count, list)
#    define FARM_PP_LIST_REST_N_I(count, list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_LIST_REST_N_P, FARM_PP_LIST_REST_N_O, (list, count)))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_REST_N_P(d, lc) FARM_PP_TUPLE_ELEM(2, 1, lc)
# else
#    define FARM_PP_LIST_REST_N_P(d, lc) FARM_PP_LIST_REST_N_P_I lc
#    define FARM_PP_LIST_REST_N_P_I(list, count) count
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_REST_N_O(d, lc) (FARM_PP_LIST_REST(FARM_PP_TUPLE_ELEM(2, 0, lc)), FARM_PP_DEC(FARM_PP_TUPLE_ELEM(2, 1, lc)))
# else
#    define FARM_PP_LIST_REST_N_O(d, lc) FARM_PP_LIST_REST_N_O_I lc
#    define FARM_PP_LIST_REST_N_O_I(list, count) (FARM_PP_LIST_REST(list), FARM_PP_DEC(count))
# endif
#
# /* FARM_PP_LIST_REST_N_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_REST_N_D(d, count, list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_LIST_REST_N_P, FARM_PP_LIST_REST_N_O, (list, count)))
# else
#    define FARM_PP_LIST_REST_N_D(d, count, list) FARM_PP_LIST_REST_N_D_I(d, count, list)
#    define FARM_PP_LIST_REST_N_D_I(d, count, list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_LIST_REST_N_P, FARM_PP_LIST_REST_N_O, (list, count)))
# endif
#
# endif
