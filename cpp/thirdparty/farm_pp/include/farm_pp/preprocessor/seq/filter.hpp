# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_SEQ_FILTER_HPP
# define FARM_PP_PREPROCESSOR_SEQ_FILTER_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/expr_if.hpp>
# include <farm_pp/preprocessor/facilities/empty.hpp>
# include <farm_pp/preprocessor/seq/fold_left.hpp>
# include <farm_pp/preprocessor/seq/seq.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_SEQ_FILTER */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_FILTER(pred, data, seq) FARM_PP_SEQ_TAIL(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_SEQ_FOLD_LEFT(FARM_PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# else
#    define FARM_PP_SEQ_FILTER(pred, data, seq) FARM_PP_SEQ_FILTER_I(pred, data, seq)
#    define FARM_PP_SEQ_FILTER_I(pred, data, seq) FARM_PP_SEQ_TAIL(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_SEQ_FOLD_LEFT(FARM_PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# endif
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_SEQ_FILTER_O(s, st, elem) FARM_PP_SEQ_FILTER_O_IM(s, FARM_PP_TUPLE_REM_3 st, elem)
#    define FARM_PP_SEQ_FILTER_O_IM(s, im, elem) FARM_PP_SEQ_FILTER_O_I(s, im, elem)
# else
#    define FARM_PP_SEQ_FILTER_O(s, st, elem) FARM_PP_SEQ_FILTER_O_I(s, FARM_PP_TUPLE_ELEM(3, 0, st), FARM_PP_TUPLE_ELEM(3, 1, st), FARM_PP_TUPLE_ELEM(3, 2, st), elem)
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_DMC()
#   define FARM_PP_SEQ_FILTER_O_I(s, pred, data, res, elem) (pred, data, res FARM_PP_EXPR_IF(pred(s, data, elem), (elem)))
# else
#   define FARM_PP_SEQ_FILTER_O_I(s, pred, data, res, elem) (pred, data, res FARM_PP_EXPR_IF(pred##(s, data, elem), (elem)))
# endif
#
# /* FARM_PP_SEQ_FILTER_S */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_FILTER_S(s, pred, data, seq) FARM_PP_SEQ_TAIL(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_SEQ_FOLD_LEFT_ ## s(FARM_PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# else
#    define FARM_PP_SEQ_FILTER_S(s, pred, data, seq) FARM_PP_SEQ_FILTER_S_I(s, pred, data, seq)
#    define FARM_PP_SEQ_FILTER_S_I(s, pred, data, seq) FARM_PP_SEQ_TAIL(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_SEQ_FOLD_LEFT_ ## s(FARM_PP_SEQ_FILTER_O, (pred, data, (nil)), seq)))
# endif
#
# endif
