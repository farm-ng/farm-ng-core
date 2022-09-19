# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2012.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_SEQ_VARIADIC_SEQ_TO_SEQ_HPP
# define FARM_PP_PREPROCESSOR_SEQ_VARIADIC_SEQ_TO_SEQ_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_VARIADIC_SEQ_TO_SEQ */
#
#    define FARM_PP_VARIADIC_SEQ_TO_SEQ(vseq) FARM_PP_CAT(FARM_PP_VARIADIC_SEQ_TO_SEQ_A vseq, 0)
#    define FARM_PP_VARIADIC_SEQ_TO_SEQ_A(...) ((__VA_ARGS__)) FARM_PP_VARIADIC_SEQ_TO_SEQ_B
#    define FARM_PP_VARIADIC_SEQ_TO_SEQ_B(...) ((__VA_ARGS__)) FARM_PP_VARIADIC_SEQ_TO_SEQ_A
#    define FARM_PP_VARIADIC_SEQ_TO_SEQ_A0
#    define FARM_PP_VARIADIC_SEQ_TO_SEQ_B0
#
# endif
