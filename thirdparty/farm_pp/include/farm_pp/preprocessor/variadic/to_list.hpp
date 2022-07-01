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
# ifndef FARM_PP_PREPROCESSOR_VARIADIC_TO_LIST_HPP
# define FARM_PP_PREPROCESSOR_VARIADIC_TO_LIST_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/tuple/to_list.hpp>
# include <farm_pp/preprocessor/variadic/has_opt.hpp>
# include <farm_pp/preprocessor/variadic/size.hpp>
#
# /* FARM_PP_VARIADIC_TO_LIST */
#
# if FARM_PP_VARIADIC_HAS_OPT()
#     define FARM_PP_VARIADIC_TO_LIST_NOT_EMPTY(...) FARM_PP_TUPLE_TO_LIST((__VA_ARGS__))
#     define FARM_PP_VARIADIC_TO_LIST_EMPTY(...) FARM_PP_NIL
#     define FARM_PP_VARIADIC_TO_LIST(...) FARM_PP_IF(FARM_PP_VARIADIC_SIZE(__VA_ARGS__),FARM_PP_VARIADIC_TO_LIST_NOT_EMPTY,FARM_PP_VARIADIC_TO_LIST_EMPTY)(__VA_ARGS__)
# else
#     define FARM_PP_VARIADIC_TO_LIST(...) FARM_PP_TUPLE_TO_LIST((__VA_ARGS__))
# endif
#
# endif
