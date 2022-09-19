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
# ifndef FARM_PP_PREPROCESSOR_TUPLE_INSERT_HPP
# define FARM_PP_PREPROCESSOR_TUPLE_INSERT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/array/insert.hpp>
# include <farm_pp/preprocessor/array/to_tuple.hpp>
# include <farm_pp/preprocessor/tuple/to_array.hpp>
#
# /* FARM_PP_TUPLE_INSERT */
#
# define FARM_PP_TUPLE_INSERT(tuple, i, elem) \
    FARM_PP_ARRAY_TO_TUPLE(FARM_PP_ARRAY_INSERT(FARM_PP_TUPLE_TO_ARRAY(tuple), i, elem)) \
/**/
#
# /* FARM_PP_TUPLE_INSERT_D */
#
# define FARM_PP_TUPLE_INSERT_D(d, tuple, i, elem) \
    FARM_PP_ARRAY_TO_TUPLE(FARM_PP_ARRAY_INSERT_D(d, FARM_PP_TUPLE_TO_ARRAY(tuple), i, elem)) \
/**/
#
# endif // FARM_PP_PREPROCESSOR_TUPLE_INSERT_HPP
