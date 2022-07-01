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
# ifndef FARM_PP_PREPROCESSOR_LIST_TRANSFORM_HPP
# define FARM_PP_PREPROCESSOR_LIST_TRANSFORM_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/list/fold_right.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_LIST_TRANSFORM */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_TRANSFORM(op, data, list) FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_LIST_FOLD_RIGHT(FARM_PP_LIST_TRANSFORM_O, (op, data, FARM_PP_NIL), list))
# else
#    define FARM_PP_LIST_TRANSFORM(op, data, list) FARM_PP_LIST_TRANSFORM_I(op, data, list)
#    define FARM_PP_LIST_TRANSFORM_I(op, data, list) FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_LIST_FOLD_RIGHT(FARM_PP_LIST_TRANSFORM_O, (op, data, FARM_PP_NIL), list))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_TRANSFORM_O(d, odr, elem) FARM_PP_LIST_TRANSFORM_O_D(d, FARM_PP_TUPLE_ELEM(3, 0, odr), FARM_PP_TUPLE_ELEM(3, 1, odr), FARM_PP_TUPLE_ELEM(3, 2, odr), elem)
# else
#    define FARM_PP_LIST_TRANSFORM_O(d, odr, elem) FARM_PP_LIST_TRANSFORM_O_I(d, FARM_PP_TUPLE_REM_3 odr, elem)
#    define FARM_PP_LIST_TRANSFORM_O_I(d, im, elem) FARM_PP_LIST_TRANSFORM_O_D(d, im, elem)
# endif
#
# define FARM_PP_LIST_TRANSFORM_O_D(d, op, data, res, elem) (op, data, (op(d, data, elem), res))
#
# /* FARM_PP_LIST_TRANSFORM_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_TRANSFORM_D(d, op, data, list) FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_LIST_FOLD_RIGHT_ ## d(FARM_PP_LIST_TRANSFORM_O, (op, data, FARM_PP_NIL), list))
# else
#    define FARM_PP_LIST_TRANSFORM_D(d, op, data, list) FARM_PP_LIST_TRANSFORM_D_I(d, op, data, list)
#    define FARM_PP_LIST_TRANSFORM_D_I(d, op, data, list) FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_LIST_FOLD_RIGHT_ ## d(FARM_PP_LIST_TRANSFORM_O, (op, data, FARM_PP_NIL), list))
# endif
#
# endif
