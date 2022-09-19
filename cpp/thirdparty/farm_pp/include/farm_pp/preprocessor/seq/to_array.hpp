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
# ifndef FARM_PP_PREPROCESSOR_SEQ_TO_ARRAY_HPP
# define FARM_PP_PREPROCESSOR_SEQ_TO_ARRAY_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/seq/enum.hpp>
# include <farm_pp/preprocessor/seq/size.hpp>
#
# /* FARM_PP_SEQ_TO_ARRAY */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_TO_ARRAY(seq) (FARM_PP_SEQ_SIZE(seq), (FARM_PP_SEQ_ENUM(seq)))
# else
#    define FARM_PP_SEQ_TO_ARRAY(seq) FARM_PP_SEQ_TO_ARRAY_I(seq)
#    define FARM_PP_SEQ_TO_ARRAY_I(seq) (FARM_PP_SEQ_SIZE(seq), (FARM_PP_SEQ_ENUM(seq)))
# endif
#
# endif
