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
# ifndef FARM_PP_PREPROCESSOR_LIST_AT_HPP
# define FARM_PP_PREPROCESSOR_LIST_AT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/list/rest_n.hpp>
#
# /* FARM_PP_LIST_AT */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_AT(list, index) FARM_PP_LIST_FIRST(FARM_PP_LIST_REST_N(index, list))
# else
#    define FARM_PP_LIST_AT(list, index) FARM_PP_LIST_AT_I(list, index)
#    define FARM_PP_LIST_AT_I(list, index) FARM_PP_LIST_FIRST(FARM_PP_LIST_REST_N(index, list))
# endif
#
# /* FARM_PP_LIST_AT_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LIST_AT_D(d, list, index) FARM_PP_LIST_FIRST(FARM_PP_LIST_REST_N_D(d, index, list))
# else
#    define FARM_PP_LIST_AT_D(d, list, index) FARM_PP_LIST_AT_D_I(d, list, index)
#    define FARM_PP_LIST_AT_D_I(d, list, index) FARM_PP_LIST_FIRST(FARM_PP_LIST_REST_N_D(d, index, list))
# endif
#
# endif
