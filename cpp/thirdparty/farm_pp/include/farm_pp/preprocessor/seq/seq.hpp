# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_SEQ_SEQ_HPP
# define FARM_PP_PREPROCESSOR_SEQ_SEQ_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/seq/elem.hpp>
#
# /* FARM_PP_SEQ_HEAD */
#
# define FARM_PP_SEQ_HEAD(seq) FARM_PP_SEQ_ELEM(0, seq)
#
# /* FARM_PP_SEQ_TAIL */
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_SEQ_TAIL(seq) FARM_PP_SEQ_TAIL_1((seq))
#    define FARM_PP_SEQ_TAIL_1(par) FARM_PP_SEQ_TAIL_2 ## par
#    define FARM_PP_SEQ_TAIL_2(seq) FARM_PP_SEQ_TAIL_I ## seq
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_SEQ_TAIL(seq) FARM_PP_SEQ_TAIL_ID(FARM_PP_SEQ_TAIL_I seq)
#    define FARM_PP_SEQ_TAIL_ID(id) id
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_TAIL(seq) FARM_PP_SEQ_TAIL_D(seq)
#    define FARM_PP_SEQ_TAIL_D(seq) FARM_PP_SEQ_TAIL_I seq
# else
#    define FARM_PP_SEQ_TAIL(seq) FARM_PP_SEQ_TAIL_I seq
# endif
#
# define FARM_PP_SEQ_TAIL_I(x)
#
# /* FARM_PP_SEQ_NIL */
#
# define FARM_PP_SEQ_NIL(x) (x)
#
# endif
