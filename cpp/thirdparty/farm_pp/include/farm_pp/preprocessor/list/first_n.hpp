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
# ifndef FARM_PP_PREPROCESSOR_LIST_FIRST_N_HPP
# define FARM_PP_PREPROCESSOR_LIST_FIRST_N_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/list/reverse.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_LIST_FIRST_N */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FIRST_N(count, list) FARM_PP_LIST_REVERSE(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_WHILE(FARM_PP_LIST_FIRST_N_P, FARM_PP_LIST_FIRST_N_O, (count, list, FARM_PP_NIL))))
# else
#    define FARM_PP_LIST_FIRST_N(count, list) FARM_PP_LIST_FIRST_N_I(count, list)
#    define FARM_PP_LIST_FIRST_N_I(count, list) FARM_PP_LIST_REVERSE(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_WHILE(FARM_PP_LIST_FIRST_N_P, FARM_PP_LIST_FIRST_N_O, (count, list, FARM_PP_NIL))))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FIRST_N_P(d, data) FARM_PP_TUPLE_ELEM(3, 0, data)
# else
#    define FARM_PP_LIST_FIRST_N_P(d, data) FARM_PP_LIST_FIRST_N_P_I data
#    define FARM_PP_LIST_FIRST_N_P_I(c, l, nl) c
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_LIST_FIRST_N_O(d, data) FARM_PP_LIST_FIRST_N_O_D data
# else
#    define FARM_PP_LIST_FIRST_N_O(d, data) FARM_PP_LIST_FIRST_N_O_D(FARM_PP_TUPLE_ELEM(3, 0, data), FARM_PP_TUPLE_ELEM(3, 1, data), FARM_PP_TUPLE_ELEM(3, 2, data))
# endif
#
# define FARM_PP_LIST_FIRST_N_O_D(c, l, nl) (FARM_PP_DEC(c), FARM_PP_LIST_REST(l), (FARM_PP_LIST_FIRST(l), nl))
#
# /* FARM_PP_LIST_FIRST_N_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FIRST_N_D(d, count, list) FARM_PP_LIST_REVERSE_D(d, FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_WHILE_ ## d(FARM_PP_LIST_FIRST_N_P, FARM_PP_LIST_FIRST_N_O, (count, list, FARM_PP_NIL))))
# else
#    define FARM_PP_LIST_FIRST_N_D(d, count, list) FARM_PP_LIST_FIRST_N_D_I(d, count, list)
#    define FARM_PP_LIST_FIRST_N_D_I(d, count, list) FARM_PP_LIST_REVERSE_D(d, FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_WHILE_ ## d(FARM_PP_LIST_FIRST_N_P, FARM_PP_LIST_FIRST_N_O, (count, list, FARM_PP_NIL))))
# endif
#
# endif
