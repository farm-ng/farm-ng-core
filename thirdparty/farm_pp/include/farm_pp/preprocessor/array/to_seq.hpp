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
# ifndef FARM_PP_PREPROCESSOR_ARRAY_TO_SEQ_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_TO_SEQ_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/array/size.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/tuple/to_seq.hpp>
#
# /* FARM_PP_ARRAY_TO_SEQ */
#
#    define FARM_PP_ARRAY_TO_SEQ(array) \
        FARM_PP_IF \
            ( \
            FARM_PP_ARRAY_SIZE(array), \
            FARM_PP_ARRAY_TO_SEQ_DO, \
            FARM_PP_ARRAY_TO_SEQ_EMPTY \
            ) \
        (array) \
/**/
#    define FARM_PP_ARRAY_TO_SEQ_EMPTY(array)
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_ARRAY_TO_SEQ_DO(array) FARM_PP_ARRAY_TO_SEQ_I(FARM_PP_TUPLE_TO_SEQ, array)
#    define FARM_PP_ARRAY_TO_SEQ_I(m, args) FARM_PP_ARRAY_TO_SEQ_II(m, args)
#    define FARM_PP_ARRAY_TO_SEQ_II(m, args) FARM_PP_CAT(m ## args,)
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_ARRAY_TO_SEQ_DO(array) FARM_PP_ARRAY_TO_SEQ_I(array)
#    define FARM_PP_ARRAY_TO_SEQ_I(array) FARM_PP_TUPLE_TO_SEQ ## array
# else
#    define FARM_PP_ARRAY_TO_SEQ_DO(array) FARM_PP_TUPLE_TO_SEQ array
# endif
#
# endif
