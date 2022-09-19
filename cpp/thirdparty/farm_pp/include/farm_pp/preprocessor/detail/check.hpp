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
# ifndef FARM_PP_PREPROCESSOR_DETAIL_CHECK_HPP
# define FARM_PP_PREPROCESSOR_DETAIL_CHECK_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_CHECK */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_CHECK(x, type) FARM_PP_CHECK_D(x, type)
# else
#    define FARM_PP_CHECK(x, type) FARM_PP_CHECK_OO((x, type))
#    define FARM_PP_CHECK_OO(par) FARM_PP_CHECK_D ## par
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC() && ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_DMC()
#    define FARM_PP_CHECK_D(x, type) FARM_PP_CHECK_1(FARM_PP_CAT(FARM_PP_CHECK_RESULT_, type x))
#    define FARM_PP_CHECK_1(chk) FARM_PP_CHECK_2(chk)
#    define FARM_PP_CHECK_2(res, _) res
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_CHECK_D(x, type) FARM_PP_CHECK_1(type x)
#    define FARM_PP_CHECK_1(chk) FARM_PP_CHECK_2(chk)
#    define FARM_PP_CHECK_2(chk) FARM_PP_CHECK_3((FARM_PP_CHECK_RESULT_ ## chk))
#    define FARM_PP_CHECK_3(im) FARM_PP_CHECK_5(FARM_PP_CHECK_4 im)
#    define FARM_PP_CHECK_4(res, _) res
#    define FARM_PP_CHECK_5(res) res
# else /* DMC */
#    define FARM_PP_CHECK_D(x, type) FARM_PP_CHECK_OO((type x))
#    define FARM_PP_CHECK_OO(par) FARM_PP_CHECK_0 ## par
#    define FARM_PP_CHECK_0(chk) FARM_PP_CHECK_1(FARM_PP_CAT(FARM_PP_CHECK_RESULT_, chk))
#    define FARM_PP_CHECK_1(chk) FARM_PP_CHECK_2(chk)
#    define FARM_PP_CHECK_2(res, _) res
# endif
#
# define FARM_PP_CHECK_RESULT_1 1, FARM_PP_NIL
#
# endif
