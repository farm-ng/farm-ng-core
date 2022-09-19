# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2016.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_SEQ_DETAIL_TO_LIST_MSVC_HPP
# define FARM_PP_PREPROCESSOR_SEQ_DETAIL_TO_LIST_MSVC_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# define FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_STATE_RESULT(state) \
    FARM_PP_TUPLE_ELEM(2, 0, state) \
/**/
# define FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_STATE_SIZE(state) \
    FARM_PP_TUPLE_ELEM(2, 1, state) \
/**/
# define FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_PRED(d,state) \
    FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_STATE_SIZE(state) \
/**/
# define FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_OP(d,state) \
    ( \
    FARM_PP_CAT(FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_STATE_RESULT(state),), \
    FARM_PP_DEC(FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_STATE_SIZE(state)) \
    ) \
/**/
#
# /* FARM_PP_SEQ_DETAIL_TO_LIST_MSVC */
#
# define FARM_PP_SEQ_DETAIL_TO_LIST_MSVC(result,seqsize) \
    FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_STATE_RESULT \
        ( \
        FARM_PP_WHILE \
            ( \
            FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_PRED, \
            FARM_PP_SEQ_DETAIL_TO_LIST_MSVC_OP, \
            (result,seqsize) \
            ) \
        ) \
/**/
# endif // FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#
# endif // FARM_PP_PREPROCESSOR_SEQ_DETAIL_TO_LIST_MSVC_HPP
