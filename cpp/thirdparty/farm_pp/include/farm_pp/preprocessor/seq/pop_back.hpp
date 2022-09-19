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
# ifndef FARM_PP_PREPROCESSOR_SEQ_POP_BACK_HPP
# define FARM_PP_PREPROCESSOR_SEQ_POP_BACK_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/seq/first_n.hpp>
# include <farm_pp/preprocessor/seq/size.hpp>
#
# /* FARM_PP_SEQ_POP_BACK */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_POP_BACK(seq) FARM_PP_SEQ_FIRST_N(FARM_PP_DEC(FARM_PP_SEQ_SIZE(seq)), seq)
# else
#    define FARM_PP_SEQ_POP_BACK(seq) FARM_PP_SEQ_POP_BACK_I(seq)
#    define FARM_PP_SEQ_POP_BACK_I(seq) FARM_PP_SEQ_FIRST_N(FARM_PP_DEC(FARM_PP_SEQ_SIZE(seq)), seq)
# endif
#
# endif
