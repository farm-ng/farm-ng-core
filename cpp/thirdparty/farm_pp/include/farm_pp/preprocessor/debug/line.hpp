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
# ifndef FARM_PP_PREPROCESSOR_DEBUG_LINE_HPP
# define FARM_PP_PREPROCESSOR_DEBUG_LINE_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/iteration/iterate.hpp>
# include <farm_pp/preprocessor/stringize.hpp>
#
# /* FARM_PP_LINE */
#
# if FARM_PP_CONFIG_EXTENDED_LINE_INFO
#    define FARM_PP_LINE(line, file) line FARM_PP_CAT(FARM_PP_LINE_, FARM_PP_IS_ITERATING)(file)
#    define FARM_PP_LINE_FARM_PP_IS_ITERATING(file) #file
#    define FARM_PP_LINE_1(file) FARM_PP_STRINGIZE(file FARM_PP_CAT(FARM_PP_LINE_I_, FARM_PP_ITERATION_DEPTH())())
#    define FARM_PP_LINE_I_1() [FARM_PP_FRAME_ITERATION(1)]
#    define FARM_PP_LINE_I_2() FARM_PP_LINE_I_1()[FARM_PP_FRAME_ITERATION(2)]
#    define FARM_PP_LINE_I_3() FARM_PP_LINE_I_2()[FARM_PP_FRAME_ITERATION(3)]
#    define FARM_PP_LINE_I_4() FARM_PP_LINE_I_3()[FARM_PP_FRAME_ITERATION(4)]
#    define FARM_PP_LINE_I_5() FARM_PP_LINE_I_4()[FARM_PP_FRAME_ITERATION(5)]
# else
#    define FARM_PP_LINE(line, file) line __FILE__
# endif
#
# endif
