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
# ifndef FARM_PP_PREPROCESSOR_SEQ_FOR_EACH_HPP
# define FARM_PP_PREPROCESSOR_SEQ_FOR_EACH_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
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
# /* FARM_PP_SEQ_FOR_EACH */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_FOR_EACH(macro, data, seq) FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK(macro, data, seq)
# else
#    define FARM_PP_SEQ_FOR_EACH(macro, data, seq) FARM_PP_SEQ_FOR_EACH_D(macro, data, seq)
#    define FARM_PP_SEQ_FOR_EACH_D(macro, data, seq) FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK(macro, data, seq)
# endif
#
#    define FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_EXEC(macro, data, seq) FARM_PP_FOR((macro, data, seq, FARM_PP_SEQ_SIZE(seq)), FARM_PP_SEQ_FOR_EACH_P, FARM_PP_SEQ_FOR_EACH_O, FARM_PP_SEQ_FOR_EACH_M)
#    define FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_EMPTY(macro, data, seq)
#
#    define FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK(macro, data, seq) \
        FARM_PP_IIF \
            ( \
            FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
            FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_EXEC, \
            FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_EMPTY \
            ) \
        (macro, data, seq) \
/**/
#
# define FARM_PP_SEQ_FOR_EACH_P(r, x) FARM_PP_TUPLE_ELEM(4, 3, x)
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_SEQ_FOR_EACH_O(r, x) FARM_PP_SEQ_FOR_EACH_O_I x
# else
#    define FARM_PP_SEQ_FOR_EACH_O(r, x) FARM_PP_SEQ_FOR_EACH_O_I(FARM_PP_TUPLE_ELEM(4, 0, x), FARM_PP_TUPLE_ELEM(4, 1, x), FARM_PP_TUPLE_ELEM(4, 2, x), FARM_PP_TUPLE_ELEM(4, 3, x))
# endif
#
# define FARM_PP_SEQ_FOR_EACH_O_I(macro, data, seq, sz) \
    FARM_PP_SEQ_FOR_EACH_O_I_DEC(macro, data, seq, FARM_PP_DEC(sz)) \
/**/
# define FARM_PP_SEQ_FOR_EACH_O_I_DEC(macro, data, seq, sz) \
    ( \
    macro, \
    data, \
    FARM_PP_IF \
        ( \
        sz, \
        FARM_PP_SEQ_FOR_EACH_O_I_TAIL, \
        FARM_PP_SEQ_FOR_EACH_O_I_NIL \
        ) \
    (seq), \
    sz \
    ) \
/**/
# define FARM_PP_SEQ_FOR_EACH_O_I_TAIL(seq) FARM_PP_SEQ_TAIL(seq)
# define FARM_PP_SEQ_FOR_EACH_O_I_NIL(seq) FARM_PP_NIL
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_SEQ_FOR_EACH_M(r, x) FARM_PP_SEQ_FOR_EACH_M_IM(r, FARM_PP_TUPLE_REM_4 x)
#    define FARM_PP_SEQ_FOR_EACH_M_IM(r, im) FARM_PP_SEQ_FOR_EACH_M_I(r, im)
# else
#    define FARM_PP_SEQ_FOR_EACH_M(r, x) FARM_PP_SEQ_FOR_EACH_M_I(r, FARM_PP_TUPLE_ELEM(4, 0, x), FARM_PP_TUPLE_ELEM(4, 1, x), FARM_PP_TUPLE_ELEM(4, 2, x), FARM_PP_TUPLE_ELEM(4, 3, x))
# endif
#
# define FARM_PP_SEQ_FOR_EACH_M_I(r, macro, data, seq, sz) macro(r, data, FARM_PP_SEQ_HEAD(seq))
#
# /* FARM_PP_SEQ_FOR_EACH_R */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_FOR_EACH_R(r, macro, data, seq) FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_R(r, macro, data, seq)
# else
#    define FARM_PP_SEQ_FOR_EACH_R(r, macro, data, seq) FARM_PP_SEQ_FOR_EACH_R_I(r, macro, data, seq)
#    define FARM_PP_SEQ_FOR_EACH_R_I(r, macro, data, seq) FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_R(r, macro, data, seq)
# endif
#
#    define FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_EXEC_R(r, macro, data, seq) FARM_PP_FOR_ ## r((macro, data, seq, FARM_PP_SEQ_SIZE(seq)), FARM_PP_SEQ_FOR_EACH_P, FARM_PP_SEQ_FOR_EACH_O, FARM_PP_SEQ_FOR_EACH_M)
#    define FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_EMPTY_R(r, macro, data, seq)
#
#    define FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_R(r, macro, data, seq) \
        FARM_PP_IIF \
            ( \
            FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
            FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_EXEC_R, \
            FARM_PP_SEQ_FOR_EACH_DETAIL_CHECK_EMPTY_R \
            ) \
        (r, macro, data, seq) \
/**/
#
# endif
