# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_LIST_ENUM_HPP
# define FARM_PP_PREPROCESSOR_LIST_ENUM_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/list/for_each_i.hpp>
# include <farm_pp/preprocessor/punctuation/comma_if.hpp>
# include <farm_pp/preprocessor/variadic/has_opt.hpp>
#
# /* FARM_PP_LIST_ENUM */
#
# if FARM_PP_VARIADIC_HAS_OPT()
#    if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#        define FARM_PP_LIST_ENUM_NOT_EMPTY(list) FARM_PP_LIST_FOR_EACH_I(FARM_PP_LIST_ENUM_O, FARM_PP_NIL, list)
#    else
#        define FARM_PP_LIST_ENUM_NOT_EMPTY(list) FARM_PP_LIST_ENUM_I(list)
#        define FARM_PP_LIST_ENUM_I(list) FARM_PP_LIST_FOR_EACH_I(FARM_PP_LIST_ENUM_O, FARM_PP_NIL, list)
#    endif
#    define FARM_PP_LIST_ENUM_EMPTY(list)
#    define FARM_PP_LIST_ENUM(list) FARM_PP_IIF(FARM_PP_LIST_IS_NIL(list),FARM_PP_LIST_ENUM_EMPTY,FARM_PP_LIST_ENUM_NOT_EMPTY)(list)
# elif ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_ENUM(list) FARM_PP_LIST_FOR_EACH_I(FARM_PP_LIST_ENUM_O, FARM_PP_NIL, list)
# else
#    define FARM_PP_LIST_ENUM(list) FARM_PP_LIST_ENUM_I(list)
#    define FARM_PP_LIST_ENUM_I(list) FARM_PP_LIST_FOR_EACH_I(FARM_PP_LIST_ENUM_O, FARM_PP_NIL, list)
# endif
#
# define FARM_PP_LIST_ENUM_O(r, _, i, elem) FARM_PP_COMMA_IF(i) elem
#
# /* FARM_PP_LIST_ENUM_R */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_ENUM_R(r, list) FARM_PP_LIST_FOR_EACH_I_R(r, FARM_PP_LIST_ENUM_O, FARM_PP_NIL, list)
# else
#    define FARM_PP_LIST_ENUM_R(r, list) FARM_PP_LIST_ENUM_R_I(r, list)
#    define FARM_PP_LIST_ENUM_R_I(r, list) FARM_PP_LIST_FOR_EACH_I_R(r, FARM_PP_LIST_ENUM_O, FARM_PP_NIL, list)
# endif
#
# endif
