# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* Revised by Paul Mensonides (2011) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_LIST_TO_SEQ_HPP
# define FARM_PP_PREPROCESSOR_LIST_TO_SEQ_HPP
#
# include <farm_pp/preprocessor/list/for_each.hpp>
#
# /* FARM_PP_LIST_TO_SEQ */
#
# define FARM_PP_LIST_TO_SEQ(list) \
    FARM_PP_LIST_FOR_EACH(FARM_PP_LIST_TO_SEQ_MACRO, ~, list) \
    /**/
# define FARM_PP_LIST_TO_SEQ_MACRO(r, data, elem) (elem)
#
# /* FARM_PP_LIST_TO_SEQ_R */
#
# define FARM_PP_LIST_TO_SEQ_R(r, list) \
    FARM_PP_LIST_FOR_EACH_R(r, FARM_PP_LIST_TO_SEQ_MACRO, ~, list) \
    /**/
#
# endif /* FARM_PP_PREPROCESSOR_LIST_TO_SEQ_HPP */
