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
# ifndef FARM_PP_PREPROCESSOR_LIST_TO_TUPLE_HPP
# define FARM_PP_PREPROCESSOR_LIST_TO_TUPLE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/list/enum.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
#
# /* FARM_PP_LIST_TO_TUPLE */
#
# define FARM_PP_LIST_TO_TUPLE(list) \
    FARM_PP_IIF \
        ( \
        FARM_PP_LIST_IS_NIL(list), \
        FARM_PP_LIST_TO_TUPLE_EMPTY, \
        FARM_PP_LIST_TO_TUPLE_DO \
        ) \
    (list) \
/**/
# define FARM_PP_LIST_TO_TUPLE_EMPTY(list)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_TO_TUPLE_DO(list) (FARM_PP_LIST_ENUM(list))
# else
#    define FARM_PP_LIST_TO_TUPLE_DO(list) FARM_PP_LIST_TO_TUPLE_I(list)
#    define FARM_PP_LIST_TO_TUPLE_I(list) (FARM_PP_LIST_ENUM(list))
# endif
#
# /* FARM_PP_LIST_TO_TUPLE_R */
#
# define FARM_PP_LIST_TO_TUPLE_R(r, list) \
    FARM_PP_IIF \
        ( \
        FARM_PP_LIST_IS_NIL(list), \
        FARM_PP_LIST_TO_TUPLE_R_EMPTY, \
        FARM_PP_LIST_TO_TUPLE_R_DO \
        ) \
    (r, list) \
/**/
# define FARM_PP_LIST_TO_TUPLE_R_EMPTY(r,list)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_TO_TUPLE_R_DO(r, list) (FARM_PP_LIST_ENUM_R(r, list))
# else
#    define FARM_PP_LIST_TO_TUPLE_R_DO(r, list) FARM_PP_LIST_TO_TUPLE_R_I(r, list)
#    define FARM_PP_LIST_TO_TUPLE_R_I(r, list) (FARM_PP_LIST_ENUM_R(r, list))
# endif
#
# endif
