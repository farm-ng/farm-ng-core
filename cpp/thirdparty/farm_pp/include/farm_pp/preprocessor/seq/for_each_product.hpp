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
# ifndef FARM_PP_PREPROCESSOR_SEQ_FOR_EACH_PRODUCT_HPP
# define FARM_PP_PREPROCESSOR_SEQ_FOR_EACH_PRODUCT_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/repetition/for.hpp>
# include <farm_pp/preprocessor/seq/seq.hpp>
# include <farm_pp/preprocessor/seq/size.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_SEQ_FOR_EACH_PRODUCT */
#
# define FARM_PP_SEQ_FOR_EACH_PRODUCT(macro, sets) FARM_PP_SEQ_FOR_EACH_PRODUCT_E(FARM_PP_FOR, macro, sets)
#
# /* FARM_PP_SEQ_FOR_EACH_PRODUCT_R */
#
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_R(r, macro, sets) FARM_PP_SEQ_FOR_EACH_PRODUCT_E(FARM_PP_FOR_ ## r, macro, sets)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_E(impl, macro, sets) impl((FARM_PP_SEQ_HEAD(sets)(nil), FARM_PP_SEQ_TAIL(sets)(nil), (nil), macro), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_0)
# else
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_E(impl, macro, sets) FARM_PP_SEQ_FOR_EACH_PRODUCT_E_I(impl, macro, sets)
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_E_I(impl, macro, sets) impl((FARM_PP_SEQ_HEAD(sets)(nil), FARM_PP_SEQ_TAIL(sets)(nil), (nil), macro), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_0)
# endif
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_P(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_P_I data
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_P_I(cset, rset, res, macro) FARM_PP_DEC(FARM_PP_SEQ_SIZE(cset))
# else
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_P(r, data) FARM_PP_DEC(FARM_PP_SEQ_SIZE(FARM_PP_TUPLE_ELEM(4, 0, data)))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_O(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_O_I data
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_O_I(cset, rset, res, macro) (FARM_PP_SEQ_TAIL(cset), rset, res, macro)
# else
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_O(r, data) (FARM_PP_SEQ_TAIL(FARM_PP_TUPLE_ELEM(4, 0, data)), FARM_PP_TUPLE_ELEM(4, 1, data), FARM_PP_TUPLE_ELEM(4, 2, data), FARM_PP_TUPLE_ELEM(4, 3, data))
# endif
#
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, i) FARM_PP_IF(FARM_PP_DEC(FARM_PP_SEQ_SIZE(FARM_PP_TUPLE_ELEM(4, 1, data))), FARM_PP_SEQ_FOR_EACH_PRODUCT_N_ ## i, FARM_PP_SEQ_FOR_EACH_PRODUCT_I)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_I(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_I_I(r, FARM_PP_TUPLE_ELEM(4, 0, data), FARM_PP_TUPLE_ELEM(4, 1, data), FARM_PP_TUPLE_ELEM(4, 2, data), FARM_PP_TUPLE_ELEM(4, 3, data))
# else
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_I(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_I_IM(r, FARM_PP_TUPLE_REM_4 data)
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_I_IM(r, im) FARM_PP_SEQ_FOR_EACH_PRODUCT_I_I(r, im)
# endif
#
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_I_I(r, cset, rset, res, macro) macro(r, FARM_PP_SEQ_TAIL(res (FARM_PP_SEQ_HEAD(cset))))
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data) FARM_PP_SEQ_FOR_EACH_PRODUCT_H_I data
# else
#    define FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data) FARM_PP_SEQ_FOR_EACH_PRODUCT_H_I(FARM_PP_TUPLE_ELEM(4, 0, data), FARM_PP_TUPLE_ELEM(4, 1, data), FARM_PP_TUPLE_ELEM(4, 2, data), FARM_PP_TUPLE_ELEM(4, 3, data))
# endif
#
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_H_I(cset, rset, res, macro) (FARM_PP_SEQ_HEAD(rset)(nil), FARM_PP_SEQ_TAIL(rset), res (FARM_PP_SEQ_HEAD(cset)), macro)
#
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_0(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 0)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_1(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 1)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_2(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 2)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_3(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 3)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_4(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 4)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_5(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 5)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_6(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 6)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_7(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 7)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_8(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 8)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_9(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 9)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_10(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 10)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_11(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 11)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_12(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 12)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_13(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 13)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_14(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 14)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_15(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 15)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_16(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 16)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_17(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 17)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_18(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 18)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_19(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 19)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_20(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 20)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_21(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 21)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_22(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 22)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_23(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 23)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_24(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 24)(r, data)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_M_25(r, data) FARM_PP_SEQ_FOR_EACH_PRODUCT_C(data, 25)(r, data)
#
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_0(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_1)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_1(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_2)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_2(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_3)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_3(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_4)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_4(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_5)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_5(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_6)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_6(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_7)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_7(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_8)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_8(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_9)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_9(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_10)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_10(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_11)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_11(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_12)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_12(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_13)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_13(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_14)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_14(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_15)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_15(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_16)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_16(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_17)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_17(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_18)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_18(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_19)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_19(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_20)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_20(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_21)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_21(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_22)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_22(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_23)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_23(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_24)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_24(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_25)
# define FARM_PP_SEQ_FOR_EACH_PRODUCT_N_25(r, data) FARM_PP_FOR_ ## r(FARM_PP_SEQ_FOR_EACH_PRODUCT_H(data), FARM_PP_SEQ_FOR_EACH_PRODUCT_P, FARM_PP_SEQ_FOR_EACH_PRODUCT_O, FARM_PP_SEQ_FOR_EACH_PRODUCT_M_26)
#
# endif
