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
# ifndef FARM_PP_PREPROCESSOR_SEQ_SUBSEQ_HPP
# define FARM_PP_PREPROCESSOR_SEQ_SUBSEQ_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/seq/first_n.hpp>
# include <farm_pp/preprocessor/seq/rest_n.hpp>
#
# /* FARM_PP_SEQ_SUBSEQ */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_SUBSEQ(seq, i, len) FARM_PP_SEQ_FIRST_N(len, FARM_PP_SEQ_REST_N(i, seq))
# else
#    define FARM_PP_SEQ_SUBSEQ(seq, i, len) FARM_PP_SEQ_SUBSEQ_I(seq, i, len)
#    define FARM_PP_SEQ_SUBSEQ_I(seq, i, len) FARM_PP_SEQ_FIRST_N(len, FARM_PP_SEQ_REST_N(i, seq))
# endif
#
# endif
