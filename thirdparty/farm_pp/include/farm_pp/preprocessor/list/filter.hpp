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
# ifndef FARM_PP_PREPROCESSOR_LIST_FILTER_HPP
# define FARM_PP_PREPROCESSOR_LIST_FILTER_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/list/fold_right.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_LIST_FILTER */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FILTER(pred, data, list) FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_LIST_FOLD_RIGHT(FARM_PP_LIST_FILTER_O, (pred, data, FARM_PP_NIL), list))
# else
#    define FARM_PP_LIST_FILTER(pred, data, list) FARM_PP_LIST_FILTER_I(pred, data, list)
#    define FARM_PP_LIST_FILTER_I(pred, data, list) FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_LIST_FOLD_RIGHT(FARM_PP_LIST_FILTER_O, (pred, data, FARM_PP_NIL), list))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FILTER_O(d, pdr, elem) FARM_PP_LIST_FILTER_O_D(d, FARM_PP_TUPLE_ELEM(3, 0, pdr), FARM_PP_TUPLE_ELEM(3, 1, pdr), FARM_PP_TUPLE_ELEM(3, 2, pdr), elem)
# else
#    define FARM_PP_LIST_FILTER_O(d, pdr, elem) FARM_PP_LIST_FILTER_O_I(d, FARM_PP_TUPLE_REM_3 pdr, elem)
#    define FARM_PP_LIST_FILTER_O_I(d, im, elem) FARM_PP_LIST_FILTER_O_D(d, im, elem)
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_DMC()
#    define FARM_PP_LIST_FILTER_O_D(d, pred, data, res, elem) (pred, data, FARM_PP_IF(pred(d, data, elem), (elem, res), res))
# else
#    define FARM_PP_LIST_FILTER_O_D(d, pred, data, res, elem) (pred, data, FARM_PP_IF(pred##(d, data, elem), (elem, res), res))
# endif
#
# /* FARM_PP_LIST_FILTER_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_FILTER_D(d, pred, data, list) FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_LIST_FOLD_RIGHT_ ## d(FARM_PP_LIST_FILTER_O, (pred, data, FARM_PP_NIL), list))
# else
#    define FARM_PP_LIST_FILTER_D(d, pred, data, list) FARM_PP_LIST_FILTER_D_I(d, pred, data, list)
#    define FARM_PP_LIST_FILTER_D_I(d, pred, data, list) FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_LIST_FOLD_RIGHT_ ## d(FARM_PP_LIST_FILTER_O, (pred, data, FARM_PP_NIL), list))
# endif
#
# endif
