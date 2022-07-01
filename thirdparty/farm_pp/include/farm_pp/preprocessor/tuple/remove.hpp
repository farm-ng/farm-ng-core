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
# ifndef FARM_PP_PREPROCESSOR_TUPLE_REMOVE_HPP
# define FARM_PP_PREPROCESSOR_TUPLE_REMOVE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/array/remove.hpp>
# include <farm_pp/preprocessor/array/to_tuple.hpp>
# include <farm_pp/preprocessor/comparison/greater.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/tuple/size.hpp>
# include <farm_pp/preprocessor/tuple/to_array.hpp>
#
# /* FARM_PP_TUPLE_REMOVE */
#
# define FARM_PP_TUPLE_REMOVE(tuple, i) \
    FARM_PP_IIF \
        ( \
        FARM_PP_GREATER(FARM_PP_TUPLE_SIZE(tuple),1), \
        FARM_PP_TUPLE_REMOVE_EXEC, \
        FARM_PP_TUPLE_REMOVE_RETURN \
        ) \
    (tuple, i) \
/**/
#
# define FARM_PP_TUPLE_REMOVE_EXEC(tuple, i) \
    FARM_PP_ARRAY_TO_TUPLE(FARM_PP_ARRAY_REMOVE(FARM_PP_TUPLE_TO_ARRAY(tuple), i)) \
/**/
#
# define FARM_PP_TUPLE_REMOVE_RETURN(tuple, i) tuple
#
# /* FARM_PP_TUPLE_REMOVE_D */
#
# define FARM_PP_TUPLE_REMOVE_D(d, tuple, i) \
    FARM_PP_IIF \
        ( \
        FARM_PP_GREATER_D(d, FARM_PP_TUPLE_SIZE(tuple), 1), \
        FARM_PP_TUPLE_REMOVE_D_EXEC, \
        FARM_PP_TUPLE_REMOVE_D_RETURN \
        ) \
    (d, tuple, i) \
/**/
#
# define FARM_PP_TUPLE_REMOVE_D_EXEC(d, tuple, i) \
    FARM_PP_ARRAY_TO_TUPLE(FARM_PP_ARRAY_REMOVE_D(d, FARM_PP_TUPLE_TO_ARRAY(tuple), i)) \
/**/
#
# define FARM_PP_TUPLE_REMOVE_D_RETURN(d, tuple, i) tuple
#
# endif // FARM_PP_PREPROCESSOR_TUPLE_REMOVE_HPP
