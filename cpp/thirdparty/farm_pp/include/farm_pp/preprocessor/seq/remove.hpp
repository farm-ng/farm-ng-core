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
# ifndef FARM_PP_PREPROCESSOR_SEQ_REMOVE_HPP
# define FARM_PP_PREPROCESSOR_SEQ_REMOVE_HPP
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/seq/first_n.hpp>
# include <farm_pp/preprocessor/seq/rest_n.hpp>
#
# /* FARM_PP_SEQ_REMOVE */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_REMOVE(seq, i) FARM_PP_SEQ_FIRST_N(i, seq) FARM_PP_SEQ_REST_N(FARM_PP_INC(i), seq)
# else
#    define FARM_PP_SEQ_REMOVE(seq, i) FARM_PP_SEQ_REMOVE_I(seq, i)
#    define FARM_PP_SEQ_REMOVE_I(seq, i) FARM_PP_SEQ_FIRST_N(i, seq) FARM_PP_SEQ_REST_N(FARM_PP_INC(i), seq)
# endif
#
# endif
