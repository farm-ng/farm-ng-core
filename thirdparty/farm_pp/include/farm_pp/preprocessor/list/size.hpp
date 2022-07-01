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
# ifndef FARM_PP_PREPROCESSOR_LIST_SIZE_HPP
# define FARM_PP_PREPROCESSOR_LIST_SIZE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_LIST_SIZE */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_SIZE(list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_LIST_SIZE_P, FARM_PP_LIST_SIZE_O, (0, list)))
# else
#    define FARM_PP_LIST_SIZE(list) FARM_PP_LIST_SIZE_I(list)
#    define FARM_PP_LIST_SIZE_I(list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_LIST_SIZE_P, FARM_PP_LIST_SIZE_O, (0, list)))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_SIZE_P(d, rl) FARM_PP_LIST_IS_CONS(FARM_PP_TUPLE_ELEM(2, 1, rl))
# else
#    define FARM_PP_LIST_SIZE_P(d, rl) FARM_PP_LIST_SIZE_P_I(FARM_PP_TUPLE_REM_2 rl)
#    define FARM_PP_LIST_SIZE_P_I(im) FARM_PP_LIST_SIZE_P_II(im)
#    define FARM_PP_LIST_SIZE_P_II(r, l) FARM_PP_LIST_IS_CONS(l)
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_SIZE_O(d, rl) (FARM_PP_INC(FARM_PP_TUPLE_ELEM(2, 0, rl)), FARM_PP_LIST_REST(FARM_PP_TUPLE_ELEM(2, 1, rl)))
# else
#    define FARM_PP_LIST_SIZE_O(d, rl) FARM_PP_LIST_SIZE_O_I(FARM_PP_TUPLE_REM_2 rl)
#    define FARM_PP_LIST_SIZE_O_I(im) FARM_PP_LIST_SIZE_O_II(im)
#    define FARM_PP_LIST_SIZE_O_II(r, l) (FARM_PP_INC(r), FARM_PP_LIST_REST(l))
# endif
#
# /* FARM_PP_LIST_SIZE_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_SIZE_D(d, list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_LIST_SIZE_P, FARM_PP_LIST_SIZE_O, (0, list)))
# else
#    define FARM_PP_LIST_SIZE_D(d, list) FARM_PP_LIST_SIZE_D_I(d, list)
#    define FARM_PP_LIST_SIZE_D_I(d, list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_LIST_SIZE_P, FARM_PP_LIST_SIZE_O, (0, list)))
# endif
#
# else
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/facilities/identity.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_LIST_SIZE */
#
#    define FARM_PP_LIST_SIZE(list) FARM_PP_IIF(FARM_PP_LIST_IS_CONS(list),FARM_PP_LIST_SIZE_S,FARM_PP_IDENTITY_N(0,1))(list)
#    define FARM_PP_LIST_SIZE_S(list) FARM_PP_LIST_SIZE_DO(FARM_PP_LIST_REST(list))
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_SIZE_DO(list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_LIST_SIZE_P, FARM_PP_LIST_SIZE_O, (1, list)))
# else
#    define FARM_PP_LIST_SIZE_DO(list) FARM_PP_LIST_SIZE_I(list)
#    define FARM_PP_LIST_SIZE_I(list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_LIST_SIZE_P, FARM_PP_LIST_SIZE_O, (1, list)))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_SIZE_P(d, rl) FARM_PP_LIST_IS_CONS(FARM_PP_TUPLE_ELEM(2, 1, rl))
# else
#    define FARM_PP_LIST_SIZE_P(d, rl) FARM_PP_LIST_SIZE_P_I(FARM_PP_TUPLE_REM_2 rl)
#    define FARM_PP_LIST_SIZE_P_I(im) FARM_PP_LIST_SIZE_P_II(im)
#    define FARM_PP_LIST_SIZE_P_II(r, l) FARM_PP_LIST_IS_CONS(l)
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_SIZE_O(d, rl) (FARM_PP_INC(FARM_PP_TUPLE_ELEM(2, 0, rl)), FARM_PP_LIST_REST(FARM_PP_TUPLE_ELEM(2, 1, rl)))
# else
#    define FARM_PP_LIST_SIZE_O(d, rl) FARM_PP_LIST_SIZE_O_I(FARM_PP_TUPLE_REM_2 rl)
#    define FARM_PP_LIST_SIZE_O_I(im) FARM_PP_LIST_SIZE_O_II(im)
#    define FARM_PP_LIST_SIZE_O_II(r, l) (FARM_PP_INC(r), FARM_PP_LIST_REST(l))
# endif
#
# /* FARM_PP_LIST_SIZE_D */
#
#    define FARM_PP_LIST_SIZE_D(d, list) FARM_PP_IIF(FARM_PP_LIST_IS_CONS(list),FARM_PP_LIST_SIZE_S_D,FARM_PP_IDENTITY_N(0,2))(d,list)
#    define FARM_PP_LIST_SIZE_S_D(d,list) FARM_PP_LIST_SIZE_DO_D(d,FARM_PP_LIST_REST(list))
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_SIZE_DO_D(d, list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_LIST_SIZE_P, FARM_PP_LIST_SIZE_O, (1, list)))
# else
#    define FARM_PP_LIST_SIZE_DO_D(d, list) FARM_PP_LIST_SIZE_D_I(d, list)
#    define FARM_PP_LIST_SIZE_D_I(d, list) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_LIST_SIZE_P, FARM_PP_LIST_SIZE_O, (1, list)))
# endif
#
# endif
#
# endif
