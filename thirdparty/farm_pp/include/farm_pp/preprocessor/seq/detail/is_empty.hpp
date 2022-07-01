# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2015.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_SEQ_DETAIL_IS_EMPTY_HPP
# define FARM_PP_PREPROCESSOR_SEQ_DETAIL_IS_EMPTY_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/logical/bool.hpp>
# include <farm_pp/preprocessor/logical/compl.hpp>
# include <farm_pp/preprocessor/seq/size.hpp>
#
/* An empty seq is one that is just FARM_PP_SEQ_NIL */
#
# define FARM_PP_SEQ_DETAIL_IS_EMPTY(seq) \
    FARM_PP_COMPL \
        ( \
        FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq) \
        ) \
/**/
#
# define FARM_PP_SEQ_DETAIL_IS_EMPTY_SIZE(size) \
    FARM_PP_COMPL \
        ( \
        FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(size) \
        ) \
/**/
#
# define FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq) \
    FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(FARM_PP_SEQ_DETAIL_EMPTY_SIZE(seq)) \
/**/
#
# define FARM_PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(size) \
    FARM_PP_BOOL(size) \
/**/
#
# define FARM_PP_SEQ_DETAIL_EMPTY_SIZE(seq) \
    FARM_PP_DEC(FARM_PP_SEQ_SIZE(seq (nil))) \
/**/
#
# endif
