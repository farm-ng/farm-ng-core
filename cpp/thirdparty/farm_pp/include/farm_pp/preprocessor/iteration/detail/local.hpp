# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* Revised by Edward Diener (2020) */
#
# /* See http://www.boost.org for most recent version. */
#
# if !defined(FARM_PP_LOCAL_LIMITS)
#    error FARM_PP_ERROR:  local iteration boundaries are not defined
# elif !defined(FARM_PP_LOCAL_MACRO)
#    error FARM_PP_ERROR:  local iteration target macro is not defined
# else
#    if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#        define FARM_PP_LOCAL_S FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_LOCAL_LIMITS)
#        define FARM_PP_LOCAL_F FARM_PP_TUPLE_ELEM(2, 1, FARM_PP_LOCAL_LIMITS)
#    else
#        define FARM_PP_VALUE FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_LOCAL_LIMITS)
#        include <farm_pp/preprocessor/iteration/detail/start.hpp>
#        define FARM_PP_VALUE FARM_PP_TUPLE_ELEM(2, 1, FARM_PP_LOCAL_LIMITS)
#        include <farm_pp/preprocessor/iteration/detail/finish.hpp>
#        define FARM_PP_LOCAL_S FARM_PP_LOCAL_SE()
#        define FARM_PP_LOCAL_F FARM_PP_LOCAL_FE()
#    endif
# endif
#
# if (FARM_PP_LOCAL_S) > (FARM_PP_LOCAL_F)
#    include <farm_pp/preprocessor/iteration/detail/rlocal.hpp>
# else
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
#    if FARM_PP_LOCAL_C(0)
        FARM_PP_LOCAL_MACRO(0)
#    endif
#    if FARM_PP_LOCAL_C(1)
        FARM_PP_LOCAL_MACRO(1)
#    endif
#    if FARM_PP_LOCAL_C(2)
        FARM_PP_LOCAL_MACRO(2)
#    endif
#    if FARM_PP_LOCAL_C(3)
        FARM_PP_LOCAL_MACRO(3)
#    endif
#    if FARM_PP_LOCAL_C(4)
        FARM_PP_LOCAL_MACRO(4)
#    endif
#    if FARM_PP_LOCAL_C(5)
        FARM_PP_LOCAL_MACRO(5)
#    endif
#    if FARM_PP_LOCAL_C(6)
        FARM_PP_LOCAL_MACRO(6)
#    endif
#    if FARM_PP_LOCAL_C(7)
        FARM_PP_LOCAL_MACRO(7)
#    endif
#    if FARM_PP_LOCAL_C(8)
        FARM_PP_LOCAL_MACRO(8)
#    endif
#    if FARM_PP_LOCAL_C(9)
        FARM_PP_LOCAL_MACRO(9)
#    endif
#    if FARM_PP_LOCAL_C(10)
        FARM_PP_LOCAL_MACRO(10)
#    endif
#    if FARM_PP_LOCAL_C(11)
        FARM_PP_LOCAL_MACRO(11)
#    endif
#    if FARM_PP_LOCAL_C(12)
        FARM_PP_LOCAL_MACRO(12)
#    endif
#    if FARM_PP_LOCAL_C(13)
        FARM_PP_LOCAL_MACRO(13)
#    endif
#    if FARM_PP_LOCAL_C(14)
        FARM_PP_LOCAL_MACRO(14)
#    endif
#    if FARM_PP_LOCAL_C(15)
        FARM_PP_LOCAL_MACRO(15)
#    endif
#    if FARM_PP_LOCAL_C(16)
        FARM_PP_LOCAL_MACRO(16)
#    endif
#    if FARM_PP_LOCAL_C(17)
        FARM_PP_LOCAL_MACRO(17)
#    endif
#    if FARM_PP_LOCAL_C(18)
        FARM_PP_LOCAL_MACRO(18)
#    endif
#    if FARM_PP_LOCAL_C(19)
        FARM_PP_LOCAL_MACRO(19)
#    endif
#    if FARM_PP_LOCAL_C(20)
        FARM_PP_LOCAL_MACRO(20)
#    endif
#    if FARM_PP_LOCAL_C(21)
        FARM_PP_LOCAL_MACRO(21)
#    endif
#    if FARM_PP_LOCAL_C(22)
        FARM_PP_LOCAL_MACRO(22)
#    endif
#    if FARM_PP_LOCAL_C(23)
        FARM_PP_LOCAL_MACRO(23)
#    endif
#    if FARM_PP_LOCAL_C(24)
        FARM_PP_LOCAL_MACRO(24)
#    endif
#    if FARM_PP_LOCAL_C(25)
        FARM_PP_LOCAL_MACRO(25)
#    endif
#    if FARM_PP_LOCAL_C(26)
        FARM_PP_LOCAL_MACRO(26)
#    endif
#    if FARM_PP_LOCAL_C(27)
        FARM_PP_LOCAL_MACRO(27)
#    endif
#    if FARM_PP_LOCAL_C(28)
        FARM_PP_LOCAL_MACRO(28)
#    endif
#    if FARM_PP_LOCAL_C(29)
        FARM_PP_LOCAL_MACRO(29)
#    endif
#    if FARM_PP_LOCAL_C(30)
        FARM_PP_LOCAL_MACRO(30)
#    endif
#    if FARM_PP_LOCAL_C(31)
        FARM_PP_LOCAL_MACRO(31)
#    endif
#    if FARM_PP_LOCAL_C(32)
        FARM_PP_LOCAL_MACRO(32)
#    endif
#    if FARM_PP_LOCAL_C(33)
        FARM_PP_LOCAL_MACRO(33)
#    endif
#    if FARM_PP_LOCAL_C(34)
        FARM_PP_LOCAL_MACRO(34)
#    endif
#    if FARM_PP_LOCAL_C(35)
        FARM_PP_LOCAL_MACRO(35)
#    endif
#    if FARM_PP_LOCAL_C(36)
        FARM_PP_LOCAL_MACRO(36)
#    endif
#    if FARM_PP_LOCAL_C(37)
        FARM_PP_LOCAL_MACRO(37)
#    endif
#    if FARM_PP_LOCAL_C(38)
        FARM_PP_LOCAL_MACRO(38)
#    endif
#    if FARM_PP_LOCAL_C(39)
        FARM_PP_LOCAL_MACRO(39)
#    endif
#    if FARM_PP_LOCAL_C(40)
        FARM_PP_LOCAL_MACRO(40)
#    endif
#    if FARM_PP_LOCAL_C(41)
        FARM_PP_LOCAL_MACRO(41)
#    endif
#    if FARM_PP_LOCAL_C(42)
        FARM_PP_LOCAL_MACRO(42)
#    endif
#    if FARM_PP_LOCAL_C(43)
        FARM_PP_LOCAL_MACRO(43)
#    endif
#    if FARM_PP_LOCAL_C(44)
        FARM_PP_LOCAL_MACRO(44)
#    endif
#    if FARM_PP_LOCAL_C(45)
        FARM_PP_LOCAL_MACRO(45)
#    endif
#    if FARM_PP_LOCAL_C(46)
        FARM_PP_LOCAL_MACRO(46)
#    endif
#    if FARM_PP_LOCAL_C(47)
        FARM_PP_LOCAL_MACRO(47)
#    endif
#    if FARM_PP_LOCAL_C(48)
        FARM_PP_LOCAL_MACRO(48)
#    endif
#    if FARM_PP_LOCAL_C(49)
        FARM_PP_LOCAL_MACRO(49)
#    endif
#    if FARM_PP_LOCAL_C(50)
        FARM_PP_LOCAL_MACRO(50)
#    endif
#    if FARM_PP_LOCAL_C(51)
        FARM_PP_LOCAL_MACRO(51)
#    endif
#    if FARM_PP_LOCAL_C(52)
        FARM_PP_LOCAL_MACRO(52)
#    endif
#    if FARM_PP_LOCAL_C(53)
        FARM_PP_LOCAL_MACRO(53)
#    endif
#    if FARM_PP_LOCAL_C(54)
        FARM_PP_LOCAL_MACRO(54)
#    endif
#    if FARM_PP_LOCAL_C(55)
        FARM_PP_LOCAL_MACRO(55)
#    endif
#    if FARM_PP_LOCAL_C(56)
        FARM_PP_LOCAL_MACRO(56)
#    endif
#    if FARM_PP_LOCAL_C(57)
        FARM_PP_LOCAL_MACRO(57)
#    endif
#    if FARM_PP_LOCAL_C(58)
        FARM_PP_LOCAL_MACRO(58)
#    endif
#    if FARM_PP_LOCAL_C(59)
        FARM_PP_LOCAL_MACRO(59)
#    endif
#    if FARM_PP_LOCAL_C(60)
        FARM_PP_LOCAL_MACRO(60)
#    endif
#    if FARM_PP_LOCAL_C(61)
        FARM_PP_LOCAL_MACRO(61)
#    endif
#    if FARM_PP_LOCAL_C(62)
        FARM_PP_LOCAL_MACRO(62)
#    endif
#    if FARM_PP_LOCAL_C(63)
        FARM_PP_LOCAL_MACRO(63)
#    endif
#    if FARM_PP_LOCAL_C(64)
        FARM_PP_LOCAL_MACRO(64)
#    endif
#    if FARM_PP_LOCAL_C(65)
        FARM_PP_LOCAL_MACRO(65)
#    endif
#    if FARM_PP_LOCAL_C(66)
        FARM_PP_LOCAL_MACRO(66)
#    endif
#    if FARM_PP_LOCAL_C(67)
        FARM_PP_LOCAL_MACRO(67)
#    endif
#    if FARM_PP_LOCAL_C(68)
        FARM_PP_LOCAL_MACRO(68)
#    endif
#    if FARM_PP_LOCAL_C(69)
        FARM_PP_LOCAL_MACRO(69)
#    endif
#    if FARM_PP_LOCAL_C(70)
        FARM_PP_LOCAL_MACRO(70)
#    endif
#    if FARM_PP_LOCAL_C(71)
        FARM_PP_LOCAL_MACRO(71)
#    endif
#    if FARM_PP_LOCAL_C(72)
        FARM_PP_LOCAL_MACRO(72)
#    endif
#    if FARM_PP_LOCAL_C(73)
        FARM_PP_LOCAL_MACRO(73)
#    endif
#    if FARM_PP_LOCAL_C(74)
        FARM_PP_LOCAL_MACRO(74)
#    endif
#    if FARM_PP_LOCAL_C(75)
        FARM_PP_LOCAL_MACRO(75)
#    endif
#    if FARM_PP_LOCAL_C(76)
        FARM_PP_LOCAL_MACRO(76)
#    endif
#    if FARM_PP_LOCAL_C(77)
        FARM_PP_LOCAL_MACRO(77)
#    endif
#    if FARM_PP_LOCAL_C(78)
        FARM_PP_LOCAL_MACRO(78)
#    endif
#    if FARM_PP_LOCAL_C(79)
        FARM_PP_LOCAL_MACRO(79)
#    endif
#    if FARM_PP_LOCAL_C(80)
        FARM_PP_LOCAL_MACRO(80)
#    endif
#    if FARM_PP_LOCAL_C(81)
        FARM_PP_LOCAL_MACRO(81)
#    endif
#    if FARM_PP_LOCAL_C(82)
        FARM_PP_LOCAL_MACRO(82)
#    endif
#    if FARM_PP_LOCAL_C(83)
        FARM_PP_LOCAL_MACRO(83)
#    endif
#    if FARM_PP_LOCAL_C(84)
        FARM_PP_LOCAL_MACRO(84)
#    endif
#    if FARM_PP_LOCAL_C(85)
        FARM_PP_LOCAL_MACRO(85)
#    endif
#    if FARM_PP_LOCAL_C(86)
        FARM_PP_LOCAL_MACRO(86)
#    endif
#    if FARM_PP_LOCAL_C(87)
        FARM_PP_LOCAL_MACRO(87)
#    endif
#    if FARM_PP_LOCAL_C(88)
        FARM_PP_LOCAL_MACRO(88)
#    endif
#    if FARM_PP_LOCAL_C(89)
        FARM_PP_LOCAL_MACRO(89)
#    endif
#    if FARM_PP_LOCAL_C(90)
        FARM_PP_LOCAL_MACRO(90)
#    endif
#    if FARM_PP_LOCAL_C(91)
        FARM_PP_LOCAL_MACRO(91)
#    endif
#    if FARM_PP_LOCAL_C(92)
        FARM_PP_LOCAL_MACRO(92)
#    endif
#    if FARM_PP_LOCAL_C(93)
        FARM_PP_LOCAL_MACRO(93)
#    endif
#    if FARM_PP_LOCAL_C(94)
        FARM_PP_LOCAL_MACRO(94)
#    endif
#    if FARM_PP_LOCAL_C(95)
        FARM_PP_LOCAL_MACRO(95)
#    endif
#    if FARM_PP_LOCAL_C(96)
        FARM_PP_LOCAL_MACRO(96)
#    endif
#    if FARM_PP_LOCAL_C(97)
        FARM_PP_LOCAL_MACRO(97)
#    endif
#    if FARM_PP_LOCAL_C(98)
        FARM_PP_LOCAL_MACRO(98)
#    endif
#    if FARM_PP_LOCAL_C(99)
        FARM_PP_LOCAL_MACRO(99)
#    endif
#    if FARM_PP_LOCAL_C(100)
        FARM_PP_LOCAL_MACRO(100)
#    endif
#    if FARM_PP_LOCAL_C(101)
        FARM_PP_LOCAL_MACRO(101)
#    endif
#    if FARM_PP_LOCAL_C(102)
        FARM_PP_LOCAL_MACRO(102)
#    endif
#    if FARM_PP_LOCAL_C(103)
        FARM_PP_LOCAL_MACRO(103)
#    endif
#    if FARM_PP_LOCAL_C(104)
        FARM_PP_LOCAL_MACRO(104)
#    endif
#    if FARM_PP_LOCAL_C(105)
        FARM_PP_LOCAL_MACRO(105)
#    endif
#    if FARM_PP_LOCAL_C(106)
        FARM_PP_LOCAL_MACRO(106)
#    endif
#    if FARM_PP_LOCAL_C(107)
        FARM_PP_LOCAL_MACRO(107)
#    endif
#    if FARM_PP_LOCAL_C(108)
        FARM_PP_LOCAL_MACRO(108)
#    endif
#    if FARM_PP_LOCAL_C(109)
        FARM_PP_LOCAL_MACRO(109)
#    endif
#    if FARM_PP_LOCAL_C(110)
        FARM_PP_LOCAL_MACRO(110)
#    endif
#    if FARM_PP_LOCAL_C(111)
        FARM_PP_LOCAL_MACRO(111)
#    endif
#    if FARM_PP_LOCAL_C(112)
        FARM_PP_LOCAL_MACRO(112)
#    endif
#    if FARM_PP_LOCAL_C(113)
        FARM_PP_LOCAL_MACRO(113)
#    endif
#    if FARM_PP_LOCAL_C(114)
        FARM_PP_LOCAL_MACRO(114)
#    endif
#    if FARM_PP_LOCAL_C(115)
        FARM_PP_LOCAL_MACRO(115)
#    endif
#    if FARM_PP_LOCAL_C(116)
        FARM_PP_LOCAL_MACRO(116)
#    endif
#    if FARM_PP_LOCAL_C(117)
        FARM_PP_LOCAL_MACRO(117)
#    endif
#    if FARM_PP_LOCAL_C(118)
        FARM_PP_LOCAL_MACRO(118)
#    endif
#    if FARM_PP_LOCAL_C(119)
        FARM_PP_LOCAL_MACRO(119)
#    endif
#    if FARM_PP_LOCAL_C(120)
        FARM_PP_LOCAL_MACRO(120)
#    endif
#    if FARM_PP_LOCAL_C(121)
        FARM_PP_LOCAL_MACRO(121)
#    endif
#    if FARM_PP_LOCAL_C(122)
        FARM_PP_LOCAL_MACRO(122)
#    endif
#    if FARM_PP_LOCAL_C(123)
        FARM_PP_LOCAL_MACRO(123)
#    endif
#    if FARM_PP_LOCAL_C(124)
        FARM_PP_LOCAL_MACRO(124)
#    endif
#    if FARM_PP_LOCAL_C(125)
        FARM_PP_LOCAL_MACRO(125)
#    endif
#    if FARM_PP_LOCAL_C(126)
        FARM_PP_LOCAL_MACRO(126)
#    endif
#    if FARM_PP_LOCAL_C(127)
        FARM_PP_LOCAL_MACRO(127)
#    endif
#    if FARM_PP_LOCAL_C(128)
        FARM_PP_LOCAL_MACRO(128)
#    endif
#    if FARM_PP_LOCAL_C(129)
        FARM_PP_LOCAL_MACRO(129)
#    endif
#    if FARM_PP_LOCAL_C(130)
        FARM_PP_LOCAL_MACRO(130)
#    endif
#    if FARM_PP_LOCAL_C(131)
        FARM_PP_LOCAL_MACRO(131)
#    endif
#    if FARM_PP_LOCAL_C(132)
        FARM_PP_LOCAL_MACRO(132)
#    endif
#    if FARM_PP_LOCAL_C(133)
        FARM_PP_LOCAL_MACRO(133)
#    endif
#    if FARM_PP_LOCAL_C(134)
        FARM_PP_LOCAL_MACRO(134)
#    endif
#    if FARM_PP_LOCAL_C(135)
        FARM_PP_LOCAL_MACRO(135)
#    endif
#    if FARM_PP_LOCAL_C(136)
        FARM_PP_LOCAL_MACRO(136)
#    endif
#    if FARM_PP_LOCAL_C(137)
        FARM_PP_LOCAL_MACRO(137)
#    endif
#    if FARM_PP_LOCAL_C(138)
        FARM_PP_LOCAL_MACRO(138)
#    endif
#    if FARM_PP_LOCAL_C(139)
        FARM_PP_LOCAL_MACRO(139)
#    endif
#    if FARM_PP_LOCAL_C(140)
        FARM_PP_LOCAL_MACRO(140)
#    endif
#    if FARM_PP_LOCAL_C(141)
        FARM_PP_LOCAL_MACRO(141)
#    endif
#    if FARM_PP_LOCAL_C(142)
        FARM_PP_LOCAL_MACRO(142)
#    endif
#    if FARM_PP_LOCAL_C(143)
        FARM_PP_LOCAL_MACRO(143)
#    endif
#    if FARM_PP_LOCAL_C(144)
        FARM_PP_LOCAL_MACRO(144)
#    endif
#    if FARM_PP_LOCAL_C(145)
        FARM_PP_LOCAL_MACRO(145)
#    endif
#    if FARM_PP_LOCAL_C(146)
        FARM_PP_LOCAL_MACRO(146)
#    endif
#    if FARM_PP_LOCAL_C(147)
        FARM_PP_LOCAL_MACRO(147)
#    endif
#    if FARM_PP_LOCAL_C(148)
        FARM_PP_LOCAL_MACRO(148)
#    endif
#    if FARM_PP_LOCAL_C(149)
        FARM_PP_LOCAL_MACRO(149)
#    endif
#    if FARM_PP_LOCAL_C(150)
        FARM_PP_LOCAL_MACRO(150)
#    endif
#    if FARM_PP_LOCAL_C(151)
        FARM_PP_LOCAL_MACRO(151)
#    endif
#    if FARM_PP_LOCAL_C(152)
        FARM_PP_LOCAL_MACRO(152)
#    endif
#    if FARM_PP_LOCAL_C(153)
        FARM_PP_LOCAL_MACRO(153)
#    endif
#    if FARM_PP_LOCAL_C(154)
        FARM_PP_LOCAL_MACRO(154)
#    endif
#    if FARM_PP_LOCAL_C(155)
        FARM_PP_LOCAL_MACRO(155)
#    endif
#    if FARM_PP_LOCAL_C(156)
        FARM_PP_LOCAL_MACRO(156)
#    endif
#    if FARM_PP_LOCAL_C(157)
        FARM_PP_LOCAL_MACRO(157)
#    endif
#    if FARM_PP_LOCAL_C(158)
        FARM_PP_LOCAL_MACRO(158)
#    endif
#    if FARM_PP_LOCAL_C(159)
        FARM_PP_LOCAL_MACRO(159)
#    endif
#    if FARM_PP_LOCAL_C(160)
        FARM_PP_LOCAL_MACRO(160)
#    endif
#    if FARM_PP_LOCAL_C(161)
        FARM_PP_LOCAL_MACRO(161)
#    endif
#    if FARM_PP_LOCAL_C(162)
        FARM_PP_LOCAL_MACRO(162)
#    endif
#    if FARM_PP_LOCAL_C(163)
        FARM_PP_LOCAL_MACRO(163)
#    endif
#    if FARM_PP_LOCAL_C(164)
        FARM_PP_LOCAL_MACRO(164)
#    endif
#    if FARM_PP_LOCAL_C(165)
        FARM_PP_LOCAL_MACRO(165)
#    endif
#    if FARM_PP_LOCAL_C(166)
        FARM_PP_LOCAL_MACRO(166)
#    endif
#    if FARM_PP_LOCAL_C(167)
        FARM_PP_LOCAL_MACRO(167)
#    endif
#    if FARM_PP_LOCAL_C(168)
        FARM_PP_LOCAL_MACRO(168)
#    endif
#    if FARM_PP_LOCAL_C(169)
        FARM_PP_LOCAL_MACRO(169)
#    endif
#    if FARM_PP_LOCAL_C(170)
        FARM_PP_LOCAL_MACRO(170)
#    endif
#    if FARM_PP_LOCAL_C(171)
        FARM_PP_LOCAL_MACRO(171)
#    endif
#    if FARM_PP_LOCAL_C(172)
        FARM_PP_LOCAL_MACRO(172)
#    endif
#    if FARM_PP_LOCAL_C(173)
        FARM_PP_LOCAL_MACRO(173)
#    endif
#    if FARM_PP_LOCAL_C(174)
        FARM_PP_LOCAL_MACRO(174)
#    endif
#    if FARM_PP_LOCAL_C(175)
        FARM_PP_LOCAL_MACRO(175)
#    endif
#    if FARM_PP_LOCAL_C(176)
        FARM_PP_LOCAL_MACRO(176)
#    endif
#    if FARM_PP_LOCAL_C(177)
        FARM_PP_LOCAL_MACRO(177)
#    endif
#    if FARM_PP_LOCAL_C(178)
        FARM_PP_LOCAL_MACRO(178)
#    endif
#    if FARM_PP_LOCAL_C(179)
        FARM_PP_LOCAL_MACRO(179)
#    endif
#    if FARM_PP_LOCAL_C(180)
        FARM_PP_LOCAL_MACRO(180)
#    endif
#    if FARM_PP_LOCAL_C(181)
        FARM_PP_LOCAL_MACRO(181)
#    endif
#    if FARM_PP_LOCAL_C(182)
        FARM_PP_LOCAL_MACRO(182)
#    endif
#    if FARM_PP_LOCAL_C(183)
        FARM_PP_LOCAL_MACRO(183)
#    endif
#    if FARM_PP_LOCAL_C(184)
        FARM_PP_LOCAL_MACRO(184)
#    endif
#    if FARM_PP_LOCAL_C(185)
        FARM_PP_LOCAL_MACRO(185)
#    endif
#    if FARM_PP_LOCAL_C(186)
        FARM_PP_LOCAL_MACRO(186)
#    endif
#    if FARM_PP_LOCAL_C(187)
        FARM_PP_LOCAL_MACRO(187)
#    endif
#    if FARM_PP_LOCAL_C(188)
        FARM_PP_LOCAL_MACRO(188)
#    endif
#    if FARM_PP_LOCAL_C(189)
        FARM_PP_LOCAL_MACRO(189)
#    endif
#    if FARM_PP_LOCAL_C(190)
        FARM_PP_LOCAL_MACRO(190)
#    endif
#    if FARM_PP_LOCAL_C(191)
        FARM_PP_LOCAL_MACRO(191)
#    endif
#    if FARM_PP_LOCAL_C(192)
        FARM_PP_LOCAL_MACRO(192)
#    endif
#    if FARM_PP_LOCAL_C(193)
        FARM_PP_LOCAL_MACRO(193)
#    endif
#    if FARM_PP_LOCAL_C(194)
        FARM_PP_LOCAL_MACRO(194)
#    endif
#    if FARM_PP_LOCAL_C(195)
        FARM_PP_LOCAL_MACRO(195)
#    endif
#    if FARM_PP_LOCAL_C(196)
        FARM_PP_LOCAL_MACRO(196)
#    endif
#    if FARM_PP_LOCAL_C(197)
        FARM_PP_LOCAL_MACRO(197)
#    endif
#    if FARM_PP_LOCAL_C(198)
        FARM_PP_LOCAL_MACRO(198)
#    endif
#    if FARM_PP_LOCAL_C(199)
        FARM_PP_LOCAL_MACRO(199)
#    endif
#    if FARM_PP_LOCAL_C(200)
        FARM_PP_LOCAL_MACRO(200)
#    endif
#    if FARM_PP_LOCAL_C(201)
        FARM_PP_LOCAL_MACRO(201)
#    endif
#    if FARM_PP_LOCAL_C(202)
        FARM_PP_LOCAL_MACRO(202)
#    endif
#    if FARM_PP_LOCAL_C(203)
        FARM_PP_LOCAL_MACRO(203)
#    endif
#    if FARM_PP_LOCAL_C(204)
        FARM_PP_LOCAL_MACRO(204)
#    endif
#    if FARM_PP_LOCAL_C(205)
        FARM_PP_LOCAL_MACRO(205)
#    endif
#    if FARM_PP_LOCAL_C(206)
        FARM_PP_LOCAL_MACRO(206)
#    endif
#    if FARM_PP_LOCAL_C(207)
        FARM_PP_LOCAL_MACRO(207)
#    endif
#    if FARM_PP_LOCAL_C(208)
        FARM_PP_LOCAL_MACRO(208)
#    endif
#    if FARM_PP_LOCAL_C(209)
        FARM_PP_LOCAL_MACRO(209)
#    endif
#    if FARM_PP_LOCAL_C(210)
        FARM_PP_LOCAL_MACRO(210)
#    endif
#    if FARM_PP_LOCAL_C(211)
        FARM_PP_LOCAL_MACRO(211)
#    endif
#    if FARM_PP_LOCAL_C(212)
        FARM_PP_LOCAL_MACRO(212)
#    endif
#    if FARM_PP_LOCAL_C(213)
        FARM_PP_LOCAL_MACRO(213)
#    endif
#    if FARM_PP_LOCAL_C(214)
        FARM_PP_LOCAL_MACRO(214)
#    endif
#    if FARM_PP_LOCAL_C(215)
        FARM_PP_LOCAL_MACRO(215)
#    endif
#    if FARM_PP_LOCAL_C(216)
        FARM_PP_LOCAL_MACRO(216)
#    endif
#    if FARM_PP_LOCAL_C(217)
        FARM_PP_LOCAL_MACRO(217)
#    endif
#    if FARM_PP_LOCAL_C(218)
        FARM_PP_LOCAL_MACRO(218)
#    endif
#    if FARM_PP_LOCAL_C(219)
        FARM_PP_LOCAL_MACRO(219)
#    endif
#    if FARM_PP_LOCAL_C(220)
        FARM_PP_LOCAL_MACRO(220)
#    endif
#    if FARM_PP_LOCAL_C(221)
        FARM_PP_LOCAL_MACRO(221)
#    endif
#    if FARM_PP_LOCAL_C(222)
        FARM_PP_LOCAL_MACRO(222)
#    endif
#    if FARM_PP_LOCAL_C(223)
        FARM_PP_LOCAL_MACRO(223)
#    endif
#    if FARM_PP_LOCAL_C(224)
        FARM_PP_LOCAL_MACRO(224)
#    endif
#    if FARM_PP_LOCAL_C(225)
        FARM_PP_LOCAL_MACRO(225)
#    endif
#    if FARM_PP_LOCAL_C(226)
        FARM_PP_LOCAL_MACRO(226)
#    endif
#    if FARM_PP_LOCAL_C(227)
        FARM_PP_LOCAL_MACRO(227)
#    endif
#    if FARM_PP_LOCAL_C(228)
        FARM_PP_LOCAL_MACRO(228)
#    endif
#    if FARM_PP_LOCAL_C(229)
        FARM_PP_LOCAL_MACRO(229)
#    endif
#    if FARM_PP_LOCAL_C(230)
        FARM_PP_LOCAL_MACRO(230)
#    endif
#    if FARM_PP_LOCAL_C(231)
        FARM_PP_LOCAL_MACRO(231)
#    endif
#    if FARM_PP_LOCAL_C(232)
        FARM_PP_LOCAL_MACRO(232)
#    endif
#    if FARM_PP_LOCAL_C(233)
        FARM_PP_LOCAL_MACRO(233)
#    endif
#    if FARM_PP_LOCAL_C(234)
        FARM_PP_LOCAL_MACRO(234)
#    endif
#    if FARM_PP_LOCAL_C(235)
        FARM_PP_LOCAL_MACRO(235)
#    endif
#    if FARM_PP_LOCAL_C(236)
        FARM_PP_LOCAL_MACRO(236)
#    endif

#    if FARM_PP_LOCAL_C(237)
        FARM_PP_LOCAL_MACRO(237)
#    endif
#    if FARM_PP_LOCAL_C(238)
        FARM_PP_LOCAL_MACRO(238)
#    endif
#    if FARM_PP_LOCAL_C(239)
        FARM_PP_LOCAL_MACRO(239)
#    endif
#    if FARM_PP_LOCAL_C(240)
        FARM_PP_LOCAL_MACRO(240)
#    endif
#    if FARM_PP_LOCAL_C(241)
        FARM_PP_LOCAL_MACRO(241)
#    endif
#    if FARM_PP_LOCAL_C(242)
        FARM_PP_LOCAL_MACRO(242)
#    endif
#    if FARM_PP_LOCAL_C(243)
        FARM_PP_LOCAL_MACRO(243)
#    endif
#    if FARM_PP_LOCAL_C(244)
        FARM_PP_LOCAL_MACRO(244)
#    endif
#    if FARM_PP_LOCAL_C(245)
        FARM_PP_LOCAL_MACRO(245)
#    endif
#    if FARM_PP_LOCAL_C(246)
        FARM_PP_LOCAL_MACRO(246)
#    endif
#    if FARM_PP_LOCAL_C(247)
        FARM_PP_LOCAL_MACRO(247)
#    endif
#    if FARM_PP_LOCAL_C(248)
        FARM_PP_LOCAL_MACRO(248)
#    endif
#    if FARM_PP_LOCAL_C(249)
        FARM_PP_LOCAL_MACRO(249)
#    endif
#    if FARM_PP_LOCAL_C(250)
        FARM_PP_LOCAL_MACRO(250)
#    endif
#    if FARM_PP_LOCAL_C(251)
        FARM_PP_LOCAL_MACRO(251)
#    endif
#    if FARM_PP_LOCAL_C(252)
        FARM_PP_LOCAL_MACRO(252)
#    endif
#    if FARM_PP_LOCAL_C(253)
        FARM_PP_LOCAL_MACRO(253)
#    endif
#    if FARM_PP_LOCAL_C(254)
        FARM_PP_LOCAL_MACRO(254)
#    endif
#    if FARM_PP_LOCAL_C(255)
        FARM_PP_LOCAL_MACRO(255)
#    endif
#    if FARM_PP_LOCAL_C(256)
        FARM_PP_LOCAL_MACRO(256)
#    endif
#
# else
#
#    include <farm_pp/preprocessor/config/limits.hpp>
#
#    if FARM_PP_LIMIT_ITERATION == 256
#    include <farm_pp/preprocessor/iteration/detail/limits/local_256.hpp>
#    elif FARM_PP_LIMIT_ITERATION == 512
#    include <farm_pp/preprocessor/iteration/detail/limits/local_256.hpp>
#    include <farm_pp/preprocessor/iteration/detail/limits/local_512.hpp>
#    elif FARM_PP_LIMIT_ITERATION == 1024
#    include <farm_pp/preprocessor/iteration/detail/limits/local_256.hpp>
#    include <farm_pp/preprocessor/iteration/detail/limits/local_512.hpp>
#    include <farm_pp/preprocessor/iteration/detail/limits/local_1024.hpp>
#    else
#    error Incorrect value for the FARM_PP_LIMIT_ITERATION limit
#    endif
#
# endif
#
# endif
#
# undef FARM_PP_LOCAL_LIMITS
#
# undef FARM_PP_LOCAL_S
# undef FARM_PP_LOCAL_F
#
# undef FARM_PP_LOCAL_MACRO
