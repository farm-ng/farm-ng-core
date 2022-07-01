# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_VARIADIC_DETAIL_IS_SINGLE_RETURN_HPP
# define FARM_PP_PREPROCESSOR_VARIADIC_DETAIL_IS_SINGLE_RETURN_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_VARIADIC_IS_SINGLE_RETURN */
#
# if FARM_PP_VARIADICS_MSVC
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/facilities/is_1.hpp>
# include <farm_pp/preprocessor/variadic/size.hpp>
# define FARM_PP_VARIADIC_IS_SINGLE_RETURN(sr,nsr,...) \
    FARM_PP_IIF(FARM_PP_IS_1(FARM_PP_VARIADIC_SIZE(__VA_ARGS__)),sr,nsr) \
    /**/
# endif /* FARM_PP_VARIADICS_MSVC */
#
# endif /* FARM_PP_PREPROCESSOR_VARIADIC_DETAIL_IS_SINGLE_RETURN_HPP */
