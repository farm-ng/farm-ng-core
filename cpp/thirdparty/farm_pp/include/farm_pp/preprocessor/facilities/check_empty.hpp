# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2019.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_FACILITIES_CHECK_EMPTY_HPP
# define FARM_PP_PREPROCESSOR_FACILITIES_CHECK_EMPTY_HPP
# include <farm_pp/preprocessor/variadic/has_opt.hpp>
# if FARM_PP_VARIADIC_HAS_OPT()
# include <farm_pp/preprocessor/facilities/is_empty_variadic.hpp>
# define FARM_PP_CHECK_EMPTY(...) FARM_PP_IS_EMPTY_OPT(__VA_ARGS__)
# endif /* FARM_PP_VARIADIC_HAS_OPT() */
# endif /* FARM_PP_PREPROCESSOR_FACILITIES_CHECK_EMPTY_HPP */
