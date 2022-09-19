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
# ifndef FARM_PP_PREPROCESSOR_SEQ_CAT_HPP
# define FARM_PP_PREPROCESSOR_SEQ_CAT_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/seq/fold_left.hpp>
# include <farm_pp/preprocessor/seq/seq.hpp>
# include <farm_pp/preprocessor/seq/size.hpp>
# include <farm_pp/preprocessor/tuple/eat.hpp>
#
# /* FARM_PP_SEQ_CAT */
#
# define FARM_PP_SEQ_CAT(seq) \
    FARM_PP_IF( \
        FARM_PP_DEC(FARM_PP_SEQ_SIZE(seq)), \
        FARM_PP_SEQ_CAT_I, \
        FARM_PP_SEQ_HEAD \
    )(seq) \
    /**/
# define FARM_PP_SEQ_CAT_I(seq) FARM_PP_SEQ_FOLD_LEFT(FARM_PP_SEQ_CAT_O, FARM_PP_SEQ_HEAD(seq), FARM_PP_SEQ_TAIL(seq))
#
# define FARM_PP_SEQ_CAT_O(s, st, elem) FARM_PP_SEQ_CAT_O_I(st, elem)
# define FARM_PP_SEQ_CAT_O_I(a, b) a ## b
#
# /* FARM_PP_SEQ_CAT_S */
#
# define FARM_PP_SEQ_CAT_S(s, seq) \
    FARM_PP_IF( \
        FARM_PP_DEC(FARM_PP_SEQ_SIZE(seq)), \
        FARM_PP_SEQ_CAT_S_I_A, \
        FARM_PP_SEQ_CAT_S_I_B \
    )(s, seq) \
    /**/
# define FARM_PP_SEQ_CAT_S_I_A(s, seq) FARM_PP_SEQ_FOLD_LEFT_ ## s(FARM_PP_SEQ_CAT_O, FARM_PP_SEQ_HEAD(seq), FARM_PP_SEQ_TAIL(seq))
# define FARM_PP_SEQ_CAT_S_I_B(s, seq) FARM_PP_SEQ_HEAD(seq)
#
# endif
