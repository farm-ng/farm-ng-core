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
# ifndef FARM_PP_PREPROCESSOR_TUPLE_TO_ARRAY_HPP
# define FARM_PP_PREPROCESSOR_TUPLE_TO_ARRAY_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/facilities/overload.hpp>
# include <farm_pp/preprocessor/tuple/size.hpp>
# include <farm_pp/preprocessor/variadic/size.hpp>
# include <farm_pp/preprocessor/variadic/has_opt.hpp>
#
# /* FARM_PP_TUPLE_TO_ARRAY */
#
# if FARM_PP_VARIADICS_MSVC
#     define FARM_PP_TUPLE_TO_ARRAY(...) FARM_PP_TUPLE_TO_ARRAY_I(FARM_PP_OVERLOAD(FARM_PP_TUPLE_TO_ARRAY_, __VA_ARGS__), (__VA_ARGS__))
#     define FARM_PP_TUPLE_TO_ARRAY_I(m, args) FARM_PP_TUPLE_TO_ARRAY_II(m, args)
#     define FARM_PP_TUPLE_TO_ARRAY_II(m, args) FARM_PP_CAT(m ## args,)
#     define FARM_PP_TUPLE_TO_ARRAY_1(tuple) (FARM_PP_TUPLE_SIZE(tuple), tuple)
# else
#     define FARM_PP_TUPLE_TO_ARRAY(...) FARM_PP_OVERLOAD(FARM_PP_TUPLE_TO_ARRAY_, __VA_ARGS__)(__VA_ARGS__)
#     if FARM_PP_VARIADIC_HAS_OPT()
#         define FARM_PP_TUPLE_TO_ARRAY_1(tuple) FARM_PP_TUPLE_TO_ARRAY_1_SIZE(FARM_PP_VARIADIC_SIZE tuple, tuple)
#         define FARM_PP_TUPLE_TO_ARRAY_1_SIZE(size,tuple) (FARM_PP_IF(size,size,1), tuple)
#     else
#         define FARM_PP_TUPLE_TO_ARRAY_1(tuple) (FARM_PP_VARIADIC_SIZE tuple, tuple)
#     endif
# endif
# define FARM_PP_TUPLE_TO_ARRAY_2(size, tuple) (size, tuple)
#
# endif
