# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2020.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_ARITHMETIC_DETAIL_IS_MAXIMUM_NUMBER_HPP
# define FARM_PP_PREPROCESSOR_ARITHMETIC_DETAIL_IS_MAXIMUM_NUMBER_HPP
#
# /* FARM_PP_DETAIL_IS_MAXIMUM_NUMBER */
#
# include <farm_pp/preprocessor/comparison/equal.hpp>
# include <farm_pp/preprocessor/arithmetic/detail/maximum_number.hpp>
#
# define FARM_PP_DETAIL_IS_MAXIMUM_NUMBER(x) FARM_PP_EQUAL(x,FARM_PP_DETAIL_MAXIMUM_NUMBER)
#
# endif
