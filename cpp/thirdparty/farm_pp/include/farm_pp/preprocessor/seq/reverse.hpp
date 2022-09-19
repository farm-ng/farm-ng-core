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
# ifndef FARM_PP_PREPROCESSOR_SEQ_REVERSE_HPP
# define FARM_PP_PREPROCESSOR_SEQ_REVERSE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/facilities/empty.hpp>
# include <farm_pp/preprocessor/seq/fold_left.hpp>
#
# /* FARM_PP_SEQ_REVERSE */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_REVERSE(seq) FARM_PP_SEQ_FOLD_LEFT(FARM_PP_SEQ_REVERSE_O, FARM_PP_EMPTY, seq)()
# else
#    define FARM_PP_SEQ_REVERSE(seq) FARM_PP_SEQ_REVERSE_I(seq)
#    define FARM_PP_SEQ_REVERSE_I(seq) FARM_PP_SEQ_FOLD_LEFT(FARM_PP_SEQ_REVERSE_O, FARM_PP_EMPTY, seq)()
# endif
#
# define FARM_PP_SEQ_REVERSE_O(s, state, elem) (elem) state
#
# /* FARM_PP_SEQ_REVERSE_S */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_REVERSE_S(s, seq) FARM_PP_SEQ_FOLD_LEFT_ ## s(FARM_PP_SEQ_REVERSE_O, FARM_PP_EMPTY, seq)()
# else
#    define FARM_PP_SEQ_REVERSE_S(s, seq) FARM_PP_SEQ_REVERSE_S_I(s, seq)
#    define FARM_PP_SEQ_REVERSE_S_I(s, seq) FARM_PP_SEQ_FOLD_LEFT_ ## s(FARM_PP_SEQ_REVERSE_O, FARM_PP_EMPTY, seq)()
# endif
#
# endif
