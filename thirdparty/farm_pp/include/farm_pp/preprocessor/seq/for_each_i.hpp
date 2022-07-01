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
# ifndef FARM_PP_PREPROCESSOR_SEQ_FOR_EACH_I_HPP
# define FARM_PP_PREPROCESSOR_SEQ_FOR_EACH_I_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/repetition/for.hpp>
# include <farm_pp/preprocessor/seq/seq.hpp>
# include <farm_pp/preprocessor/seq/size.hpp>
# include <farm_pp/preprocessor/seq/detail/is_empty.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_SEQ_FOR_EACH_I */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_FOR_EACH_I(macro, data, seq) FARM_PP_SEQ_FOR_EACH_I_DETAIL_CHECK(macro, data, seq)
# else
#    define FARM_PP_SEQ_FOR_EACH_I(macro, data, seq) FARM_PP_SEQ_FOR_EACH_I_I(macro, data, seq)
#    define FARM_PP_SEQ_FOR_EACH_I_I(macro, data, seq) FARM_PP_SEQ_FOR_EACH_I_DETAIL_CHECK(macro, data, seq)
# endif
#
#    define FARM_PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EXEC(macro, data, seq) FARM_PP_FOR((macro, data, seq, 0, FARM_PP_SEQ_SIZE(seq)), FARM_PP_SEQ_FOR_EACH_I_P, FARM_PP_SEQ_FOR_EACH_I_O, FARM_PP_SEQ_FOR_EACH_I_M)
#    define FARM_PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EMPTY(macro, data, seq)
#
#    define FARM_PP_SEQ_FOR_EACH_I_DETAIL_CHECK(macro, data, seq) \
        FARM_PP_IIF \
            ( \
            FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
            FARM_PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EXEC, \
            FARM_PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EMPTY \
            ) \
        (macro, data, seq) \
/**/
#
# define FARM_PP_SEQ_FOR_EACH_I_P(r, x) FARM_PP_TUPLE_ELEM(5, 4, x)
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_SEQ_FOR_EACH_I_O(r, x) FARM_PP_SEQ_FOR_EACH_I_O_I x
# else
#    define FARM_PP_SEQ_FOR_EACH_I_O(r, x) FARM_PP_SEQ_FOR_EACH_I_O_I(FARM_PP_TUPLE_ELEM(5, 0, x), FARM_PP_TUPLE_ELEM(5, 1, x), FARM_PP_TUPLE_ELEM(5, 2, x), FARM_PP_TUPLE_ELEM(5, 3, x), FARM_PP_TUPLE_ELEM(5, 4, x))
# endif
#
# define FARM_PP_SEQ_FOR_EACH_I_O_I(macro, data, seq, i, sz) \
    FARM_PP_SEQ_FOR_EACH_I_O_I_DEC(macro, data, seq, i, FARM_PP_DEC(sz)) \
/**/
# define FARM_PP_SEQ_FOR_EACH_I_O_I_DEC(macro, data, seq, i, sz) \
    ( \
    macro, \
    data, \
    FARM_PP_IF \
        ( \
        sz, \
        FARM_PP_SEQ_FOR_EACH_I_O_I_TAIL, \
        FARM_PP_SEQ_FOR_EACH_I_O_I_NIL \
        ) \
    (seq), \
    FARM_PP_INC(i), \
    sz \
    ) \
/**/
# define FARM_PP_SEQ_FOR_EACH_I_O_I_TAIL(seq) FARM_PP_SEQ_TAIL(seq)
# define FARM_PP_SEQ_FOR_EACH_I_O_I_NIL(seq) FARM_PP_NIL
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_SEQ_FOR_EACH_I_M(r, x) FARM_PP_SEQ_FOR_EACH_I_M_IM(r, FARM_PP_TUPLE_REM_5 x)
#    define FARM_PP_SEQ_FOR_EACH_I_M_IM(r, im) FARM_PP_SEQ_FOR_EACH_I_M_I(r, im)
# else
#    define FARM_PP_SEQ_FOR_EACH_I_M(r, x) FARM_PP_SEQ_FOR_EACH_I_M_I(r, FARM_PP_TUPLE_ELEM(5, 0, x), FARM_PP_TUPLE_ELEM(5, 1, x), FARM_PP_TUPLE_ELEM(5, 2, x), FARM_PP_TUPLE_ELEM(5, 3, x), FARM_PP_TUPLE_ELEM(5, 4, x))
# endif
#
# define FARM_PP_SEQ_FOR_EACH_I_M_I(r, macro, data, seq, i, sz) macro(r, data, i, FARM_PP_SEQ_HEAD(seq))
#
# /* FARM_PP_SEQ_FOR_EACH_I_R */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_FOR_EACH_I_R(r, macro, data, seq) FARM_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK(r, macro, data, seq)
# else
#    define FARM_PP_SEQ_FOR_EACH_I_R(r, macro, data, seq) FARM_PP_SEQ_FOR_EACH_I_R_I(r, macro, data, seq)
#    define FARM_PP_SEQ_FOR_EACH_I_R_I(r, macro, data, seq) FARM_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK(r, macro, data, seq)
# endif
#
#    define FARM_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EXEC(r, macro, data, seq) FARM_PP_FOR_ ## r((macro, data, seq, 0, FARM_PP_SEQ_SIZE(seq)), FARM_PP_SEQ_FOR_EACH_I_P, FARM_PP_SEQ_FOR_EACH_I_O, FARM_PP_SEQ_FOR_EACH_I_M)
#    define FARM_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EMPTY(r, macro, data, seq)
#
#    define FARM_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK(r, macro, data, seq) \
        FARM_PP_IIF \
            ( \
            FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
            FARM_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EXEC, \
            FARM_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EMPTY \
            ) \
        (r, macro, data, seq) \
/**/
#
# endif
