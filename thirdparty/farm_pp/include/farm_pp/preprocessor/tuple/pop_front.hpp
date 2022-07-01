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
# ifndef FARM_PP_PREPROCESSOR_TUPLE_POP_FRONT_HPP
# define FARM_PP_PREPROCESSOR_TUPLE_POP_FRONT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/array/pop_front.hpp>
# include <farm_pp/preprocessor/array/to_tuple.hpp>
# include <farm_pp/preprocessor/comparison/greater.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/tuple/size.hpp>
# include <farm_pp/preprocessor/tuple/to_array.hpp>
#
#
# /* FARM_PP_TUPLE_POP_FRONT */
#
# define FARM_PP_TUPLE_POP_FRONT(tuple) \
    FARM_PP_IIF \
        ( \
        FARM_PP_GREATER(FARM_PP_TUPLE_SIZE(tuple),1), \
        FARM_PP_TUPLE_POP_FRONT_EXEC, \
        FARM_PP_TUPLE_POP_FRONT_RETURN \
        ) \
    (tuple) \
/**/
#
# define FARM_PP_TUPLE_POP_FRONT_EXEC(tuple) \
    FARM_PP_ARRAY_TO_TUPLE(FARM_PP_ARRAY_POP_FRONT(FARM_PP_TUPLE_TO_ARRAY(tuple))) \
/**/
#
# define FARM_PP_TUPLE_POP_FRONT_RETURN(tuple) tuple
#
# /* FARM_PP_TUPLE_POP_FRONT_Z */
#
# define FARM_PP_TUPLE_POP_FRONT_Z(z, tuple) \
    FARM_PP_IIF \
        ( \
        FARM_PP_GREATER(FARM_PP_TUPLE_SIZE(tuple),1), \
        FARM_PP_TUPLE_POP_FRONT_Z_EXEC, \
        FARM_PP_TUPLE_POP_FRONT_Z_RETURN \
        ) \
    (z, tuple) \
/**/
#
# define FARM_PP_TUPLE_POP_FRONT_Z_EXEC(z, tuple) \
    FARM_PP_ARRAY_TO_TUPLE(FARM_PP_ARRAY_POP_FRONT_Z(z, FARM_PP_TUPLE_TO_ARRAY(tuple))) \
/**/
#
# define FARM_PP_TUPLE_POP_FRONT_Z_RETURN(z, tuple) tuple
#
# endif // FARM_PP_PREPROCESSOR_TUPLE_POP_FRONT_HPP
