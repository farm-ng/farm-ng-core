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
# ifndef FARM_PP_PREPROCESSOR_SEQ_REPLACE_HPP
# define FARM_PP_PREPROCESSOR_SEQ_REPLACE_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/comparison/equal.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/seq/first_n.hpp>
# include <farm_pp/preprocessor/seq/rest_n.hpp>
# include <farm_pp/preprocessor/seq/size.hpp>
#
# /* FARM_PP_SEQ_REPLACE */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_REPLACE(seq, i, elem) FARM_PP_SEQ_FIRST_N(i, seq) (elem) FARM_PP_SEQ_REPLACE_DETAIL_REST(seq, i)
# else
#    define FARM_PP_SEQ_REPLACE(seq, i, elem) FARM_PP_SEQ_REPLACE_I(seq, i, elem)
#    define FARM_PP_SEQ_REPLACE_I(seq, i, elem) FARM_PP_SEQ_FIRST_N(i, seq) (elem) FARM_PP_SEQ_REPLACE_DETAIL_REST(seq, i)
# endif
#
#    define FARM_PP_SEQ_REPLACE_DETAIL_REST_EMPTY(seq, i)
#    define FARM_PP_SEQ_REPLACE_DETAIL_REST_VALID(seq, i) FARM_PP_SEQ_REST_N(FARM_PP_INC(i), seq)
#    define FARM_PP_SEQ_REPLACE_DETAIL_REST(seq, i) \
        FARM_PP_IIF \
            ( \
            FARM_PP_EQUAL(i,FARM_PP_DEC(FARM_PP_SEQ_SIZE(seq))), \
            FARM_PP_SEQ_REPLACE_DETAIL_REST_EMPTY, \
            FARM_PP_SEQ_REPLACE_DETAIL_REST_VALID \
            ) \
        (seq, i) \
/**/
#
# endif
