# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_SEQ_TO_LIST_HPP
# define FARM_PP_PREPROCESSOR_SEQ_TO_LIST_HPP
#
# include <farm_pp/preprocessor/punctuation/comma.hpp>
# include <farm_pp/preprocessor/punctuation/paren.hpp>
# include <farm_pp/preprocessor/seq/detail/binary_transform.hpp>
#
# /* FARM_PP_SEQ_TO_LIST */
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
# include <farm_pp/preprocessor/seq/size.hpp>
# include <farm_pp/preprocessor/seq/detail/to_list_msvc.hpp>
# define FARM_PP_SEQ_TO_LIST(seq) \
    FARM_PP_SEQ_DETAIL_TO_LIST_MSVC \
        ( \
        FARM_PP_SEQ_TO_LIST_I(FARM_PP_SEQ_BINARY_TRANSFORM(seq)), \
        FARM_PP_SEQ_SIZE(seq) \
        ) \
/**/
# else
# define FARM_PP_SEQ_TO_LIST(seq) FARM_PP_SEQ_TO_LIST_I(FARM_PP_SEQ_BINARY_TRANSFORM(seq))
# endif
# define FARM_PP_SEQ_TO_LIST_I(bseq) FARM_PP_SEQ_TO_LIST_A bseq FARM_PP_NIL FARM_PP_SEQ_TO_LIST_B bseq
# define FARM_PP_SEQ_TO_LIST_A(m, e) m(FARM_PP_LPAREN() e FARM_PP_COMMA() FARM_PP_SEQ_TO_LIST_A_ID)
# define FARM_PP_SEQ_TO_LIST_A_ID() FARM_PP_SEQ_TO_LIST_A
# define FARM_PP_SEQ_TO_LIST_B(m, e) m(FARM_PP_RPAREN() FARM_PP_SEQ_TO_LIST_B_ID)
# define FARM_PP_SEQ_TO_LIST_B_ID() FARM_PP_SEQ_TO_LIST_B
#
# endif
