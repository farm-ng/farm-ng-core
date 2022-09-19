# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_FACILITIES_OVERLOAD_HPP
# define FARM_PP_PREPROCESSOR_FACILITIES_OVERLOAD_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/variadic/size.hpp>
#
# /* FARM_PP_OVERLOAD */
#
# define FARM_PP_OVERLOAD(prefix, ...) FARM_PP_CAT(prefix, FARM_PP_VARIADIC_SIZE(__VA_ARGS__))
#
# endif
