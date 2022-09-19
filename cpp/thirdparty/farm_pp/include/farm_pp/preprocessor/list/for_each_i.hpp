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
# ifndef FARM_PP_PREPROCESSOR_LIST_LIST_FOR_EACH_I_HPP
# define FARM_PP_PREPROCESSOR_LIST_LIST_FOR_EACH_I_HPP
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/repetition/for.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_LIST_FOR_EACH_I */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG() && ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_LIST_FOR_EACH_I(macro, data, list) FARM_PP_FOR((macro, data, list, 0), FARM_PP_LIST_FOR_EACH_I_P, FARM_PP_LIST_FOR_EACH_I_O, FARM_PP_LIST_FOR_EACH_I_M)
# else
#    define FARM_PP_LIST_FOR_EACH_I(macro, data, list) FARM_PP_LIST_FOR_EACH_I_I(macro, data, list)
#    define FARM_PP_LIST_FOR_EACH_I_I(macro, data, list) FARM_PP_FOR((macro, data, list, 0), FARM_PP_LIST_FOR_EACH_I_P, FARM_PP_LIST_FOR_EACH_I_O, FARM_PP_LIST_FOR_EACH_I_M)
# endif
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_LIST_FOR_EACH_I_P(r, x) FARM_PP_LIST_FOR_EACH_I_P_D x
#    define FARM_PP_LIST_FOR_EACH_I_P_D(m, d, l, i) FARM_PP_LIST_IS_CONS(l)
# else
#    define FARM_PP_LIST_FOR_EACH_I_P(r, x) FARM_PP_LIST_IS_CONS(FARM_PP_TUPLE_ELEM(4, 2, x))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_LIST_FOR_EACH_I_O(r, x) FARM_PP_LIST_FOR_EACH_I_O_D x
#    define FARM_PP_LIST_FOR_EACH_I_O_D(m, d, l, i) (m, d, FARM_PP_LIST_REST(l), FARM_PP_INC(i))
# else
#    define FARM_PP_LIST_FOR_EACH_I_O(r, x) (FARM_PP_TUPLE_ELEM(4, 0, x), FARM_PP_TUPLE_ELEM(4, 1, x), FARM_PP_LIST_REST(FARM_PP_TUPLE_ELEM(4, 2, x)), FARM_PP_INC(FARM_PP_TUPLE_ELEM(4, 3, x)))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FOR_EACH_I_M(r, x) FARM_PP_LIST_FOR_EACH_I_M_D(r, FARM_PP_TUPLE_ELEM(4, 0, x), FARM_PP_TUPLE_ELEM(4, 1, x), FARM_PP_TUPLE_ELEM(4, 2, x), FARM_PP_TUPLE_ELEM(4, 3, x))
# else
#    define FARM_PP_LIST_FOR_EACH_I_M(r, x) FARM_PP_LIST_FOR_EACH_I_M_I(r, FARM_PP_TUPLE_REM_4 x)
#    define FARM_PP_LIST_FOR_EACH_I_M_I(r, x_e) FARM_PP_LIST_FOR_EACH_I_M_D(r, x_e)
# endif
#
# define FARM_PP_LIST_FOR_EACH_I_M_D(r, m, d, l, i) m(r, d, i, FARM_PP_LIST_FIRST(l))
#
# /* FARM_PP_LIST_FOR_EACH_I_R */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FOR_EACH_I_R(r, macro, data, list) FARM_PP_FOR_ ## r((macro, data, list, 0), FARM_PP_LIST_FOR_EACH_I_P, FARM_PP_LIST_FOR_EACH_I_O, FARM_PP_LIST_FOR_EACH_I_M)
# else
#    define FARM_PP_LIST_FOR_EACH_I_R(r, macro, data, list) FARM_PP_LIST_FOR_EACH_I_R_I(r, macro, data, list)
#    define FARM_PP_LIST_FOR_EACH_I_R_I(r, macro, data, list) FARM_PP_FOR_ ## r((macro, data, list, 0), FARM_PP_LIST_FOR_EACH_I_P, FARM_PP_LIST_FOR_EACH_I_O, FARM_PP_LIST_FOR_EACH_I_M)
# endif
#
# endif
