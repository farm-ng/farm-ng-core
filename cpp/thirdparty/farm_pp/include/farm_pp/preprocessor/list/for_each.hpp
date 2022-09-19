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
# ifndef FARM_PP_PREPROCESSOR_LIST_FOR_EACH_HPP
# define FARM_PP_PREPROCESSOR_LIST_FOR_EACH_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/list/for_each_i.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_LIST_FOR_EACH */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FOR_EACH(macro, data, list) FARM_PP_LIST_FOR_EACH_I(FARM_PP_LIST_FOR_EACH_O, (macro, data), list)
# else
#    define FARM_PP_LIST_FOR_EACH(macro, data, list) FARM_PP_LIST_FOR_EACH_X(macro, data, list)
#    define FARM_PP_LIST_FOR_EACH_X(macro, data, list) FARM_PP_LIST_FOR_EACH_I(FARM_PP_LIST_FOR_EACH_O, (macro, data), list)
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FOR_EACH_O(r, md, i, elem) FARM_PP_LIST_FOR_EACH_O_D(r, FARM_PP_TUPLE_ELEM(2, 0, md), FARM_PP_TUPLE_ELEM(2, 1, md), elem)
# else
#    define FARM_PP_LIST_FOR_EACH_O(r, md, i, elem) FARM_PP_LIST_FOR_EACH_O_I(r, FARM_PP_TUPLE_REM_2 md, elem)
#    define FARM_PP_LIST_FOR_EACH_O_I(r, im, elem) FARM_PP_LIST_FOR_EACH_O_D(r, im, elem)
# endif
#
# define FARM_PP_LIST_FOR_EACH_O_D(r, m, d, elem) m(r, d, elem)
#
# /* FARM_PP_LIST_FOR_EACH_R */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FOR_EACH_R(r, macro, data, list) FARM_PP_LIST_FOR_EACH_I_R(r, FARM_PP_LIST_FOR_EACH_O, (macro, data), list)
# else
#    define FARM_PP_LIST_FOR_EACH_R(r, macro, data, list) FARM_PP_LIST_FOR_EACH_R_X(r, macro, data, list)
#    define FARM_PP_LIST_FOR_EACH_R_X(r, macro, data, list) FARM_PP_LIST_FOR_EACH_I_R(r, FARM_PP_LIST_FOR_EACH_O, (macro, data), list)
# endif
#
# endif
