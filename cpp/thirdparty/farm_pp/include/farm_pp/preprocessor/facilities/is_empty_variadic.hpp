# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014,2019.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP
# define FARM_PP_PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/punctuation/is_begin_parens.hpp>
# include <farm_pp/preprocessor/facilities/detail/is_empty.hpp>
#
#if FARM_PP_VARIADICS_MSVC && _MSC_VER <= 1400
#
#define FARM_PP_IS_EMPTY(param) \
    FARM_PP_DETAIL_IS_EMPTY_IIF \
      ( \
      FARM_PP_IS_BEGIN_PARENS \
        ( \
        param \
        ) \
      ) \
      ( \
      FARM_PP_IS_EMPTY_ZERO, \
      FARM_PP_DETAIL_IS_EMPTY_PROCESS \
      ) \
    (param) \
/**/
#define FARM_PP_IS_EMPTY_ZERO(param) 0
# else
# if defined(__cplusplus) && __cplusplus > 201703L
# include <farm_pp/preprocessor/variadic/has_opt.hpp>
#define FARM_PP_IS_EMPTY(...) \
    FARM_PP_DETAIL_IS_EMPTY_IIF \
      ( \
      FARM_PP_VARIADIC_HAS_OPT() \
      ) \
      ( \
      FARM_PP_IS_EMPTY_OPT, \
      FARM_PP_IS_EMPTY_NO_OPT \
      ) \
    (__VA_ARGS__) \
/**/
#define FARM_PP_IS_EMPTY_FUNCTION2(...) \
    __VA_OPT__(0,) 1 \
/**/
#define FARM_PP_IS_EMPTY_FUNCTION(...) \
    FARM_PP_IS_EMPTY_FUNCTION2(__VA_ARGS__) \
/**/
#define FARM_PP_IS_EMPTY_OPT(...) \
    FARM_PP_VARIADIC_HAS_OPT_ELEM0(FARM_PP_IS_EMPTY_FUNCTION(__VA_ARGS__),) \
/**/
# else
#define FARM_PP_IS_EMPTY(...) \
    FARM_PP_IS_EMPTY_NO_OPT(__VA_ARGS__) \
/**/
# endif /* defined(__cplusplus) && __cplusplus > 201703L */
#define FARM_PP_IS_EMPTY_NO_OPT(...) \
    FARM_PP_DETAIL_IS_EMPTY_IIF \
      ( \
      FARM_PP_IS_BEGIN_PARENS \
        ( \
        __VA_ARGS__ \
        ) \
      ) \
      ( \
      FARM_PP_IS_EMPTY_ZERO, \
      FARM_PP_DETAIL_IS_EMPTY_PROCESS \
      ) \
    (__VA_ARGS__) \
/**/
#define FARM_PP_IS_EMPTY_ZERO(...) 0
# endif /* FARM_PP_VARIADICS_MSVC && _MSC_VER <= 1400 */
# endif /* FARM_PP_PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP */
