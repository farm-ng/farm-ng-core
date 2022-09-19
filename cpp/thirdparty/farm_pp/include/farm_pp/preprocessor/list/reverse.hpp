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
# ifndef FARM_PP_PREPROCESSOR_LIST_REVERSE_HPP
# define FARM_PP_PREPROCESSOR_LIST_REVERSE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/list/fold_left.hpp>
#
# /* FARM_PP_LIST_REVERSE */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_REVERSE(list) FARM_PP_LIST_FOLD_LEFT(FARM_PP_LIST_REVERSE_O, FARM_PP_NIL, list)
# else
#    define FARM_PP_LIST_REVERSE(list) FARM_PP_LIST_REVERSE_I(list)
#    define FARM_PP_LIST_REVERSE_I(list) FARM_PP_LIST_FOLD_LEFT(FARM_PP_LIST_REVERSE_O, FARM_PP_NIL, list)
# endif
#
# define FARM_PP_LIST_REVERSE_O(d, s, x) (x, s)
#
# /* FARM_PP_LIST_REVERSE_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_REVERSE_D(d, list) FARM_PP_LIST_FOLD_LEFT_ ## d(FARM_PP_LIST_REVERSE_O, FARM_PP_NIL, list)
# else
#    define FARM_PP_LIST_REVERSE_D(d, list) FARM_PP_LIST_REVERSE_D_I(d, list)
#    define FARM_PP_LIST_REVERSE_D_I(d, list) FARM_PP_LIST_FOLD_LEFT_ ## d(FARM_PP_LIST_REVERSE_O, FARM_PP_NIL, list)
# endif
#
# else
#
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/facilities/identity.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/list/fold_left.hpp>
#
# /* FARM_PP_LIST_REVERSE */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_REVERSE(list) FARM_PP_IIF(FARM_PP_LIST_IS_CONS(list),FARM_PP_LIST_REVERSE_CONS,FARM_PP_IDENTITY_N(FARM_PP_NIL,1))(list)
# else
#    define FARM_PP_LIST_REVERSE(list) FARM_PP_LIST_REVERSE_I(list)
#    define FARM_PP_LIST_REVERSE_I(list) FARM_PP_IIF(FARM_PP_LIST_IS_CONS(list),FARM_PP_LIST_REVERSE_CONS,FARM_PP_IDENTITY_N(FARM_PP_NIL,1))(list)
# endif
#
# define FARM_PP_LIST_REVERSE_O(d, s, x) (x, s)
#
# /* FARM_PP_LIST_REVERSE_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_REVERSE_D(d, list) FARM_PP_IIF(FARM_PP_LIST_IS_CONS(list),FARM_PP_LIST_REVERSE_CONS_D,FARM_PP_IDENTITY_N(FARM_PP_NIL,2))(d,list)
# else
#    define FARM_PP_LIST_REVERSE_D(d, list) FARM_PP_LIST_REVERSE_D_I(d, list)
#    define FARM_PP_LIST_REVERSE_D_I(d, list) FARM_PP_IIF(FARM_PP_LIST_IS_CONS(list),FARM_PP_LIST_REVERSE_CONS_D,FARM_PP_IDENTITY_N(FARM_PP_NIL,2))(d,list)
# endif
#
# define FARM_PP_LIST_REVERSE_CONS(list) FARM_PP_LIST_FOLD_LEFT(FARM_PP_LIST_REVERSE_O, (FARM_PP_LIST_FIRST(list),FARM_PP_NIL), FARM_PP_LIST_REST(list))
# define FARM_PP_LIST_REVERSE_CONS_D(d, list) FARM_PP_LIST_FOLD_LEFT_ ## d(FARM_PP_LIST_REVERSE_O, (FARM_PP_LIST_FIRST(list),FARM_PP_NIL), FARM_PP_LIST_REST(list))
#
# endif
#
# endif
