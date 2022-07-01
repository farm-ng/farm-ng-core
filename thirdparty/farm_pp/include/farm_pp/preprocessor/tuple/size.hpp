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
# ifndef FARM_PP_PREPROCESSOR_TUPLE_SIZE_HPP
# define FARM_PP_PREPROCESSOR_TUPLE_SIZE_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/variadic/has_opt.hpp>
# include <farm_pp/preprocessor/variadic/size.hpp>
#
# if FARM_PP_VARIADIC_HAS_OPT()
#     if FARM_PP_VARIADICS_MSVC
#         define FARM_PP_TUPLE_SIZE(tuple) FARM_PP_TUPLE_SIZE_CHECK(FARM_PP_CAT(FARM_PP_VARIADIC_SIZE tuple,))
#     else
#         define FARM_PP_TUPLE_SIZE(tuple) FARM_PP_TUPLE_SIZE_CHECK(FARM_PP_VARIADIC_SIZE tuple)
#     endif
#     define FARM_PP_TUPLE_SIZE_CHECK(size) FARM_PP_IF(size,size,1)
# elif FARM_PP_VARIADICS_MSVC
#     define FARM_PP_TUPLE_SIZE(tuple) FARM_PP_CAT(FARM_PP_VARIADIC_SIZE tuple,)
# else
#     define FARM_PP_TUPLE_SIZE(tuple) FARM_PP_VARIADIC_SIZE tuple
# endif
#
# endif
