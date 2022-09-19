# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002-2011) */
# /* Revised by Edward Diener (2011,2014,2020) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_TUPLE_ELEM_HPP
# define FARM_PP_PREPROCESSOR_TUPLE_ELEM_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/facilities/expand.hpp>
# include <farm_pp/preprocessor/facilities/overload.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
# include <farm_pp/preprocessor/variadic/elem.hpp>
# include <farm_pp/preprocessor/tuple/detail/is_single_return.hpp>
#
# if FARM_PP_VARIADICS_MSVC
#     define FARM_PP_TUPLE_ELEM(...) FARM_PP_TUPLE_ELEM_I(FARM_PP_OVERLOAD(FARM_PP_TUPLE_ELEM_O_, __VA_ARGS__), (__VA_ARGS__))
#     define FARM_PP_TUPLE_ELEM_I(m, args) FARM_PP_TUPLE_ELEM_II(m, args)
#     define FARM_PP_TUPLE_ELEM_II(m, args) FARM_PP_CAT(m ## args,)
/*
  Use FARM_PP_REM_CAT if it is a single element tuple ( which might be empty )
  else use FARM_PP_REM. This fixes a VC++ problem with an empty tuple and FARM_PP_TUPLE_ELEM
  functionality. See tuple_elem_bug_test.cxx.
*/
#     define FARM_PP_TUPLE_ELEM_O_2(n, tuple) \
         FARM_PP_VARIADIC_ELEM(n, FARM_PP_EXPAND(FARM_PP_TUPLE_IS_SINGLE_RETURN(FARM_PP_REM_CAT,FARM_PP_REM,tuple) tuple)) \
         /**/
# else
#     define FARM_PP_TUPLE_ELEM(...) FARM_PP_OVERLOAD(FARM_PP_TUPLE_ELEM_O_, __VA_ARGS__)(__VA_ARGS__)
#     define FARM_PP_TUPLE_ELEM_O_2(n, tuple) FARM_PP_VARIADIC_ELEM(n, FARM_PP_REM tuple)
# endif
# define FARM_PP_TUPLE_ELEM_O_3(size, n, tuple) FARM_PP_TUPLE_ELEM_O_2(n, tuple)
#
# /* directly used elsewhere in Boost... */
#
# define FARM_PP_TUPLE_ELEM_1_0(a) a
#
# define FARM_PP_TUPLE_ELEM_2_0(a, b) a
# define FARM_PP_TUPLE_ELEM_2_1(a, b) b
#
# define FARM_PP_TUPLE_ELEM_3_0(a, b, c) a
# define FARM_PP_TUPLE_ELEM_3_1(a, b, c) b
# define FARM_PP_TUPLE_ELEM_3_2(a, b, c) c
#
# endif
