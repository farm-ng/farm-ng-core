# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_STRINGIZE_HPP
# define FARM_PP_PREPROCESSOR_STRINGIZE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_STRINGIZE */
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_STRINGIZE(text) FARM_PP_STRINGIZE_A((text))
#    define FARM_PP_STRINGIZE_A(arg) FARM_PP_STRINGIZE_I arg
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_STRINGIZE(text) FARM_PP_STRINGIZE_OO((text))
#    define FARM_PP_STRINGIZE_OO(par) FARM_PP_STRINGIZE_I ## par
# else
#    define FARM_PP_STRINGIZE(text) FARM_PP_STRINGIZE_I(text)
# endif
#
# define FARM_PP_STRINGIZE_I(...) #__VA_ARGS__
#
# endif
