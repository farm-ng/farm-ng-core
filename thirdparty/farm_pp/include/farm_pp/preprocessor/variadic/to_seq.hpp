# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_VARIADIC_TO_SEQ_HPP
# define FARM_PP_PREPROCESSOR_VARIADIC_TO_SEQ_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/tuple/to_seq.hpp>
#
# /* FARM_PP_VARIADIC_TO_SEQ */
#
# define FARM_PP_VARIADIC_TO_SEQ(...) FARM_PP_TUPLE_TO_SEQ((__VA_ARGS__))
#
# endif
