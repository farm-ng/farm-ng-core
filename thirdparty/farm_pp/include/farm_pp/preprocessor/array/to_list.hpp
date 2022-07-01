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
# ifndef FARM_PP_PREPROCESSOR_ARRAY_TO_LIST_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_TO_LIST_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/array/size.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/tuple/to_list.hpp>
#
# /* FARM_PP_ARRAY_TO_LIST */
#
#    define FARM_PP_ARRAY_TO_LIST(array) \
        FARM_PP_IF \
            ( \
            FARM_PP_ARRAY_SIZE(array), \
            FARM_PP_ARRAY_TO_LIST_DO, \
            FARM_PP_ARRAY_TO_LIST_EMPTY \
            ) \
        (array) \
/**/
#
#    define FARM_PP_ARRAY_TO_LIST_EMPTY(array) FARM_PP_NIL
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_ARRAY_TO_LIST_DO(array) FARM_PP_ARRAY_TO_LIST_I(FARM_PP_TUPLE_TO_LIST, array)
#    define FARM_PP_ARRAY_TO_LIST_I(m, args) FARM_PP_ARRAY_TO_LIST_II(m, args)
#    define FARM_PP_ARRAY_TO_LIST_II(m, args) FARM_PP_CAT(m ## args,)
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_ARRAY_TO_LIST_DO(array) FARM_PP_ARRAY_TO_LIST_I(array)
#    define FARM_PP_ARRAY_TO_LIST_I(array) FARM_PP_TUPLE_TO_LIST ## array
# else
#    define FARM_PP_ARRAY_TO_LIST_DO(array) FARM_PP_TUPLE_TO_LIST array
# endif
#
# endif
