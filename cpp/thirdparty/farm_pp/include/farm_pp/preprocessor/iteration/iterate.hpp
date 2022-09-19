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
# ifndef FARM_PP_PREPROCESSOR_ITERATION_ITERATE_HPP
# define FARM_PP_PREPROCESSOR_ITERATION_ITERATE_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/array/elem.hpp>
# include <farm_pp/preprocessor/array/size.hpp>
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/slot/slot.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_ITERATION_DEPTH */
#
# define FARM_PP_ITERATION_DEPTH() 0
#
# /* FARM_PP_ITERATION */
#
# define FARM_PP_ITERATION() FARM_PP_CAT(FARM_PP_ITERATION_, FARM_PP_ITERATION_DEPTH())
#
# /* FARM_PP_ITERATION_START && FARM_PP_ITERATION_FINISH */
#
# define FARM_PP_ITERATION_START() FARM_PP_CAT(FARM_PP_ITERATION_START_, FARM_PP_ITERATION_DEPTH())
# define FARM_PP_ITERATION_FINISH() FARM_PP_CAT(FARM_PP_ITERATION_FINISH_, FARM_PP_ITERATION_DEPTH())
#
# /* FARM_PP_ITERATION_FLAGS */
#
# define FARM_PP_ITERATION_FLAGS() (FARM_PP_CAT(FARM_PP_ITERATION_FLAGS_, FARM_PP_ITERATION_DEPTH())())
#
# /* FARM_PP_FRAME_ITERATION */
#
# define FARM_PP_FRAME_ITERATION(i) FARM_PP_CAT(FARM_PP_ITERATION_, i)
#
# /* FARM_PP_FRAME_START && FARM_PP_FRAME_FINISH */
#
# define FARM_PP_FRAME_START(i) FARM_PP_CAT(FARM_PP_ITERATION_START_, i)
# define FARM_PP_FRAME_FINISH(i) FARM_PP_CAT(FARM_PP_ITERATION_FINISH_, i)
#
# /* FARM_PP_FRAME_FLAGS */
#
# define FARM_PP_FRAME_FLAGS(i) (FARM_PP_CAT(FARM_PP_ITERATION_FLAGS_, i)())
#
# /* FARM_PP_RELATIVE_ITERATION */
#
# define FARM_PP_RELATIVE_ITERATION(i) FARM_PP_CAT(FARM_PP_RELATIVE_, i)(FARM_PP_ITERATION_)
#
# define FARM_PP_RELATIVE_0(m) FARM_PP_CAT(m, FARM_PP_ITERATION_DEPTH())
# define FARM_PP_RELATIVE_1(m) FARM_PP_CAT(m, FARM_PP_DEC(FARM_PP_ITERATION_DEPTH()))
# define FARM_PP_RELATIVE_2(m) FARM_PP_CAT(m, FARM_PP_DEC(FARM_PP_DEC(FARM_PP_ITERATION_DEPTH())))
# define FARM_PP_RELATIVE_3(m) FARM_PP_CAT(m, FARM_PP_DEC(FARM_PP_DEC(FARM_PP_DEC(FARM_PP_ITERATION_DEPTH()))))
# define FARM_PP_RELATIVE_4(m) FARM_PP_CAT(m, FARM_PP_DEC(FARM_PP_DEC(FARM_PP_DEC(FARM_PP_DEC(FARM_PP_ITERATION_DEPTH())))))
#
# /* FARM_PP_RELATIVE_START && FARM_PP_RELATIVE_FINISH */
#
# define FARM_PP_RELATIVE_START(i) FARM_PP_CAT(FARM_PP_RELATIVE_, i)(FARM_PP_ITERATION_START_)
# define FARM_PP_RELATIVE_FINISH(i) FARM_PP_CAT(FARM_PP_RELATIVE_, i)(FARM_PP_ITERATION_FINISH_)
#
# /* FARM_PP_RELATIVE_FLAGS */
#
# define FARM_PP_RELATIVE_FLAGS(i) (FARM_PP_CAT(FARM_PP_RELATIVE_, i)(FARM_PP_ITERATION_FLAGS_)())
#
# /* FARM_PP_ITERATE */
#
# define FARM_PP_ITERATE() FARM_PP_CAT(FARM_PP_ITERATE_, FARM_PP_INC(FARM_PP_ITERATION_DEPTH()))
#
# define FARM_PP_ITERATE_1 <farm_pp/preprocessor/iteration/detail/iter/forward1.hpp>
# define FARM_PP_ITERATE_2 <farm_pp/preprocessor/iteration/detail/iter/forward2.hpp>
# define FARM_PP_ITERATE_3 <farm_pp/preprocessor/iteration/detail/iter/forward3.hpp>
# define FARM_PP_ITERATE_4 <farm_pp/preprocessor/iteration/detail/iter/forward4.hpp>
# define FARM_PP_ITERATE_5 <farm_pp/preprocessor/iteration/detail/iter/forward5.hpp>
#
# endif
