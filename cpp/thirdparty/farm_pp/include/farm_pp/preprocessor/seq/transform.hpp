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
# ifndef FARM_PP_PREPROCESSOR_SEQ_TRANSFORM_HPP
# define FARM_PP_PREPROCESSOR_SEQ_TRANSFORM_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/seq/fold_left.hpp>
# include <farm_pp/preprocessor/seq/seq.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_SEQ_TRANSFORM */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_TRANSFORM(op, data, seq) FARM_PP_SEQ_TAIL(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_SEQ_FOLD_LEFT(FARM_PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# else
#    define FARM_PP_SEQ_TRANSFORM(op, data, seq) FARM_PP_SEQ_TRANSFORM_I(op, data, seq)
#    define FARM_PP_SEQ_TRANSFORM_I(op, data, seq) FARM_PP_SEQ_TAIL(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_SEQ_FOLD_LEFT(FARM_PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# endif
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_SEQ_TRANSFORM_O(s, state, elem) FARM_PP_SEQ_TRANSFORM_O_IM(s, FARM_PP_TUPLE_REM_3 state, elem)
#    define FARM_PP_SEQ_TRANSFORM_O_IM(s, im, elem) FARM_PP_SEQ_TRANSFORM_O_I(s, im, elem)
# else
#    define FARM_PP_SEQ_TRANSFORM_O(s, state, elem) FARM_PP_SEQ_TRANSFORM_O_I(s, FARM_PP_TUPLE_ELEM(3, 0, state), FARM_PP_TUPLE_ELEM(3, 1, state), FARM_PP_TUPLE_ELEM(3, 2, state), elem)
# endif
#
# define FARM_PP_SEQ_TRANSFORM_O_I(s, op, data, res, elem) (op, data, res (op(s, data, elem)))
#
# /* FARM_PP_SEQ_TRANSFORM_S */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_TRANSFORM_S(s, op, data, seq) FARM_PP_SEQ_TAIL(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_SEQ_FOLD_LEFT_ ## s(FARM_PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# else
#    define FARM_PP_SEQ_TRANSFORM_S(s, op, data, seq) FARM_PP_SEQ_TRANSFORM_S_I(s, op, data, seq)
#    define FARM_PP_SEQ_TRANSFORM_S_I(s, op, data, seq) FARM_PP_SEQ_TAIL(FARM_PP_TUPLE_ELEM(3, 2, FARM_PP_SEQ_FOLD_LEFT_ ## s(FARM_PP_SEQ_TRANSFORM_O, (op, data, (nil)), seq)))
# endif
#
# endif
