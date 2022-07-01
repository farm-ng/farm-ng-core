# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  *
#  * See http://www.boost.org for most recent version.
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# ifndef FARM_PP_PREPROCESSOR_LIST_ADT_HPP
# define FARM_PP_PREPROCESSOR_LIST_ADT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/detail/is_binary.hpp>
# include <farm_pp/preprocessor/logical/compl.hpp>
# include <farm_pp/preprocessor/tuple/eat.hpp>
#
# /* FARM_PP_LIST_CONS */
#
# define FARM_PP_LIST_CONS(head, tail) (head, tail)
#
# /* FARM_PP_LIST_NIL */
#
# define FARM_PP_LIST_NIL FARM_PP_NIL
#
# /* FARM_PP_LIST_FIRST */
#
# define FARM_PP_LIST_FIRST(list) FARM_PP_LIST_FIRST_D(list)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_LIST_FIRST_D(list) FARM_PP_LIST_FIRST_I list
# else
#    define FARM_PP_LIST_FIRST_D(list) FARM_PP_LIST_FIRST_I ## list
# endif
#
# define FARM_PP_LIST_FIRST_I(head, tail) head
#
# /* FARM_PP_LIST_REST */
#
# define FARM_PP_LIST_REST(list) FARM_PP_LIST_REST_D(list)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_LIST_REST_D(list) FARM_PP_LIST_REST_I list
# else
#    define FARM_PP_LIST_REST_D(list) FARM_PP_LIST_REST_I ## list
# endif
#
# define FARM_PP_LIST_REST_I(head, tail) tail
#
# /* FARM_PP_LIST_IS_CONS */
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_BCC()
#    define FARM_PP_LIST_IS_CONS(list) FARM_PP_LIST_IS_CONS_D(list)
#    define FARM_PP_LIST_IS_CONS_D(list) FARM_PP_LIST_IS_CONS_ ## list
#    define FARM_PP_LIST_IS_CONS_(head, tail) 1
#    define FARM_PP_LIST_IS_CONS_FARM_PP_NIL 0
# else
#    define FARM_PP_LIST_IS_CONS(list) FARM_PP_IS_BINARY(list)
# endif
#
# /* FARM_PP_LIST_IS_NIL */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_BCC()
#    define FARM_PP_LIST_IS_NIL(list) FARM_PP_COMPL(FARM_PP_IS_BINARY(list))
# else
#    define FARM_PP_LIST_IS_NIL(list) FARM_PP_COMPL(FARM_PP_LIST_IS_CONS(list))
# endif
#
# endif
