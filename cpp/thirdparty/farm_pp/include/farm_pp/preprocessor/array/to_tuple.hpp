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
# ifndef FARM_PP_PREPROCESSOR_ARRAY_TO_TUPLE_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_TO_TUPLE_HPP
#
# include <farm_pp/preprocessor/array/data.hpp>
# include <farm_pp/preprocessor/array/size.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
#
# /* FARM_PP_ARRAY_TO_TUPLE */
#
#    define FARM_PP_ARRAY_TO_TUPLE(array) \
        FARM_PP_IF \
            ( \
            FARM_PP_ARRAY_SIZE(array), \
            FARM_PP_ARRAY_DATA, \
            FARM_PP_ARRAY_TO_TUPLE_EMPTY \
            ) \
        (array) \
/**/
#    define FARM_PP_ARRAY_TO_TUPLE_EMPTY(array)
#
# endif
