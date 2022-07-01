# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2013.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_TUPLE_PUSH_BACK_HPP
# define FARM_PP_PREPROCESSOR_TUPLE_PUSH_BACK_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/array/push_back.hpp>
# include <farm_pp/preprocessor/array/to_tuple.hpp>
# include <farm_pp/preprocessor/tuple/to_array.hpp>
#
# /* FARM_PP_TUPLE_PUSH_BACK */
#
# define FARM_PP_TUPLE_PUSH_BACK(tuple, elem) \
    FARM_PP_ARRAY_TO_TUPLE(FARM_PP_ARRAY_PUSH_BACK(FARM_PP_TUPLE_TO_ARRAY(tuple), elem)) \
/**/
#
# endif // FARM_PP_PREPROCESSOR_TUPLE_PUSH_BACK_HPP
