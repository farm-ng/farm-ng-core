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
# ifndef FARM_PP_PREPROCESSOR_TUPLE_DETAIL_IS_SINGLE_RETURN_HPP
# define FARM_PP_PREPROCESSOR_TUPLE_DETAIL_IS_SINGLE_RETURN_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_TUPLE_IS_SINGLE_RETURN */
#
# if FARM_PP_VARIADICS_MSVC
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/facilities/is_1.hpp>
# include <farm_pp/preprocessor/tuple/size.hpp>
# define FARM_PP_TUPLE_IS_SINGLE_RETURN(sr,nsr,tuple)  \
    FARM_PP_IIF(FARM_PP_IS_1(FARM_PP_TUPLE_SIZE(tuple)),sr,nsr) \
    /**/
# endif /* FARM_PP_VARIADICS_MSVC */
#
# endif /* FARM_PP_PREPROCESSOR_TUPLE_DETAIL_IS_SINGLE_RETURN_HPP */
