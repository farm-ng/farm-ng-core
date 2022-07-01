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
# ifndef FARM_PP_PREPROCESSOR_SEQ_FIRST_N_HPP
# define FARM_PP_PREPROCESSOR_SEQ_FIRST_N_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/seq/detail/split.hpp>
# include <farm_pp/preprocessor/tuple/eat.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_SEQ_FIRST_N */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_FIRST_N(n, seq) FARM_PP_IF(n, FARM_PP_TUPLE_ELEM, FARM_PP_TUPLE_EAT_3)(2, 0, FARM_PP_SEQ_SPLIT(n, seq (nil)))
# else
#    define FARM_PP_SEQ_FIRST_N(n, seq) FARM_PP_SEQ_FIRST_N_I(n, seq)
#    define FARM_PP_SEQ_FIRST_N_I(n, seq) FARM_PP_IF(n, FARM_PP_TUPLE_ELEM, FARM_PP_TUPLE_EAT_3)(2, 0, FARM_PP_SEQ_SPLIT(n, seq (nil)))
# endif
#
# endif
