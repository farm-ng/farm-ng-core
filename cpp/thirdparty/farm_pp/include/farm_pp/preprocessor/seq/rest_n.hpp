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
# ifndef FARM_PP_PREPROCESSOR_SEQ_REST_N_HPP
# define FARM_PP_PREPROCESSOR_SEQ_REST_N_HPP
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/comparison/not_equal.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/facilities/identity.hpp>
# include <farm_pp/preprocessor/logical/bitand.hpp>
# include <farm_pp/preprocessor/seq/detail/is_empty.hpp>
# include <farm_pp/preprocessor/seq/detail/split.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_SEQ_REST_N */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_REST_N(n, seq) FARM_PP_SEQ_REST_N_DETAIL_EXEC(n, seq, FARM_PP_SEQ_DETAIL_EMPTY_SIZE(seq))
# else
#    define FARM_PP_SEQ_REST_N(n, seq) FARM_PP_SEQ_REST_N_I(n, seq)
#    define FARM_PP_SEQ_REST_N_I(n, seq) FARM_PP_SEQ_REST_N_DETAIL_EXEC(n, seq, FARM_PP_SEQ_DETAIL_EMPTY_SIZE(seq))
# endif
#
#    define FARM_PP_SEQ_REST_N_DETAIL_EXEC_NO_MATCH(n, seq)
#    define FARM_PP_SEQ_REST_N_DETAIL_EXEC_MATCH(n, seq) \
            FARM_PP_TUPLE_ELEM(2, 1, FARM_PP_SEQ_SPLIT(FARM_PP_INC(n), FARM_PP_IDENTITY( (nil) seq )))() \
/**/
#    define FARM_PP_SEQ_REST_N_DETAIL_EXEC(n, seq, size) \
        FARM_PP_IIF \
            ( \
            FARM_PP_BITAND \
                ( \
                FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(size), \
                FARM_PP_NOT_EQUAL(n,size) \
                ), \
            FARM_PP_SEQ_REST_N_DETAIL_EXEC_MATCH, \
            FARM_PP_SEQ_REST_N_DETAIL_EXEC_NO_MATCH \
            ) \
        (n, seq)  \
/**/
#
# endif
