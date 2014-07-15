/*******************************************************************************
 * File Name	: matrix.h
 * Project	: Costar Replacement Controllers
 * Date		: 2014/07/03
 * Version	: 1.0
 * Target MCU   : ATMEGA32U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

#ifndef	__matrix_h__
#define	__matrix_h__

//-------------------------------------------------------------------------------

// Convert physical keyboard layout to matrix array.

/*
,---.   ,---------------. ,---------------. ,---------------. ,-----------.
|Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|
`---'   `---------------' `---------------' `---------------' `-----------'
,-----------------------------------------------------------. ,-----------. ,---------------.
| ~ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | - | = | Backsp| |Ins|Hom|PgU| |NuL| / | * | - |
|-----------------------------------------------------------| |-----------| |---------------|
| Tab | Q | W | E | R | T | Y | U | I | O | P | [ | ] |  \  | |Del|End|PgD| | 7 | 8 | 9 | + |
|-----------------------------------------------------------| `-----------' |-----------|   |
|CapsL | A | S | D | F | G | H | J | K | L | ; | ' | Return |               | 5 | 6 | 6 |   |
|-----------------------------------------------------------|     ,---.     |---------------|
|Shift   | Z | X | C | V | B | N | M | , | . | / |    Shift |     |Up |     | 1 | 2 | 3 | E |
|-----------------------------------------------------------| ,-----------. |-----------| n |
|Ctl|Gui|Alt|          Space                |Alt|Gui|App|Ctl| |Lef|Dow|Rig| | 0     | . | t |
`-----------------------------------------------------------' `-----------' `---------------'
*/

#define KEYMAP_ANSI( \
    KJ6,      KI4, KH4, KH2, KH6, KA7, KE6, KD2, KD4, KB4, KB7, KB6, KB0,   KC7, KC5, KA5,                       \
    KJ4, KJ7, KI7, KH7, KG7, KG4, KF4, KF7, KE7, KD7, KR7, KR4, KE4, KB2,   KL4, KO4, KQ4,   KK1, KL1, KQ1, KQ0, \
    KJ2, KJ5, KI5, KH5, KG5, KG2, KF2, KF5, KE5, KD5, KR5, KR2, KE2, KB3,   KK4, KO7, KQ7,   KK5, KL5, KQ5, KO5, \
    KI2, KJ3, KI3, KH3, KG3, KG6, KF6, KF3, KE3, KD3, KR3, KR6,      KB1,                    KK2, KL2, KQ2,      \
    KN2,      KJ1, KI1, KH1, KG1, KG0, KF0, KF1, KE1, KD1, KR0,      KN3,        KO6,        KK3, KL3, KQ3, KO3, \
    KA4, KP2, KC6,                KK6,                KC0, KM3, KD0, KA1,   KO0, KK0, KL0,   KL6,      KQ6     ) \
{										\
/*      0        1        2        3        4        5        6        7   */	\
/* A */ U_None,  U_##KA1, U_None,  U_None,  U_##KA4, U_##KA5, U_None,  U_##KA7,	\
/* B */ U_##KB0, U_##KB1, U_##KB2, U_##KB3, U_##KB4, U_None,  U_##KB6, U_##KB7,	\
/* C */ U_##KC0, U_None,  U_None,  U_None,  U_None,  U_##KC5, U_##KC6, U_##KC7,	\
/* D */ U_##KD0, U_##KD1, U_##KD2, U_##KD3, U_##KD4, U_##KD5, U_None,  U_##KD7,	\
/* E */ U_None,  U_##KE1, U_##KE2, U_##KE3, U_##KE4, U_##KE5, U_##KE6, U_##KE7,	\
/* F */ U_##KF0, U_##KF1, U_##KF2, U_##KF3, U_##KF4, U_##KF5, U_##KF6, U_##KF7,	\
/* G */ U_##KG0, U_##KG1, U_##KG2, U_##KG3, U_##KG4, U_##KG5, U_##KG6, U_##KG7,	\
/* H */ U_None,  U_##KH1, U_##KH2, U_##KH3, U_##KH4, U_##KH5, U_##KH6, U_##KH7,	\
/* I */ U_None,  U_##KI1, U_##KI2, U_##KI3, U_##KI4, U_##KI5, U_None,  U_##KI7,	\
/* J */ U_None,  U_##KJ1, U_##KJ2, U_##KJ3, U_##KJ4, U_##KJ5, U_##KJ6, U_##KJ7,	\
/* K */ U_##KK0, U_##KK1, U_##KK2, U_##KK3, U_##KK4, U_##KK5, U_##KK6, U_None,	\
/* L */ U_##KL0, U_##KL1, U_##KL2, U_##KL3, U_##KL4, U_##KL5, U_##KL6, U_None,	\
/* M */ U_None,  U_None,  U_None,  U_##KM3, U_None,  U_None,  U_None,  U_None,	\
/* N */ U_None,  U_None,  U_##KN2, U_##KN3, U_None,  U_None,  U_None,  U_None,	\
/* O */ U_##KO0, U_None,  U_None,  U_##KO3, U_##KO4, U_##KO5, U_##KO6, U_##KO7,	\
/* P */ U_None,  U_None,  U_##KP2, U_None,  U_None,  U_None,  U_None,  U_None,	\
/* Q */ U_##KQ0, U_##KQ1, U_##KQ2, U_##KQ3, U_##KQ4, U_##KQ5, U_##KQ6, U_##KQ7,	\
/* R */ U_##KR0, U_None,  U_##KR2, U_##KR3, U_##KR4, U_##KR5, U_##KR6, U_##KR7 	\
}

/*
,---.   ,---------------. ,---------------. ,---------------. ,-----------.
|Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|
`---'   `---------------' `---------------' `---------------' `-----------'
,-----------------------------------------------------------. ,-----------.
| ~ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | - | = | Backsp| |Ins|Hom|PgU|
|-----------------------------------------------------------| |-----------|
| Tab | Q | W | E | R | T | Y | U | I | O | P | [ | ] |  \  | |Del|End|PgD|
|-----------------------------------------------------------| `-----------'
|CapsL | A | S | D | F | G | H | J | K | L | ; | ' | Return |
|-----------------------------------------------------------|     ,---.
|Shift   | Z | X | C | V | B | N | M | , | . | / |    Shift |     |Up |
|-----------------------------------------------------------| ,-----------.
|Ctl|Gui|Alt|          Space                |Alt|Gui|App|Ctl| |Lef|Dow|Rig|
`-----------------------------------------------------------' `-----------'
*/

#define KEYMAP_ANSI_TKL( \
    KJ6,      KI4, KH4, KH2, KH6, KA7, KE6, KD2, KD4, KB4, KB7, KB6, KB0,   KC7, KC5, KA5,	\
    KJ4, KJ7, KI7, KH7, KG7, KG4, KF4, KF7, KE7, KD7, KR7, KR4, KE4, KB2,   KL4, KO4, KQ4,	\
    KJ2, KJ5, KI5, KH5, KG5, KG2, KF2, KF5, KE5, KD5, KR5, KR2, KE2, KB3,   KK4, KO7, KQ7,	\
    KI2, KJ3, KI3, KH3, KG3, KG6, KF6, KF3, KE3, KD3, KR3, KR6,      KB1,			\
    KN2,      KJ1, KI1, KH1, KG1, KG0, KF0, KF1, KE1, KD1, KR0,      KN3,        KO6,		\
    KA4, KP2, KC6,                KK6,                KC0, KM3, KD0, KA1,   KO0, KK0, KL0 )	\
{										\
/*      0        1        2        3        4        5        6        7   */	\
/* A */ U_None,  U_##KA1, U_None,  U_None,  U_##KA4, U_##KA5, U_None,  U_##KA7,	\
/* B */ U_##KB0, U_##KB1, U_##KB2, U_##KB3, U_##KB4, U_None,  U_##KB6, U_##KB7,	\
/* C */ U_##KC0, U_None,  U_None,  U_None,  U_None,  U_##KC5, U_##KC6, U_##KC7,	\
/* D */ U_##KD0, U_##KD1, U_##KD2, U_##KD3, U_##KD4, U_##KD5, U_None,  U_##KD7,	\
/* E */ U_None,  U_##KE1, U_##KE2, U_##KE3, U_##KE4, U_##KE5, U_##KE6, U_##KE7,	\
/* F */ U_##KF0, U_##KF1, U_##KF2, U_##KF3, U_##KF4, U_##KF5, U_##KF6, U_##KF7,	\
/* G */ U_##KG0, U_##KG1, U_##KG2, U_##KG3, U_##KG4, U_##KG5, U_##KG6, U_##KG7,	\
/* H */ U_None,  U_##KH1, U_##KH2, U_##KH3, U_##KH4, U_##KH5, U_##KH6, U_##KH7,	\
/* I */ U_None,  U_##KI1, U_##KI2, U_##KI3, U_##KI4, U_##KI5, U_None,  U_##KI7,	\
/* J */ U_None,  U_##KJ1, U_##KJ2, U_##KJ3, U_##KJ4, U_##KJ5, U_##KJ6, U_##KJ7,	\
/* K */ U_##KK0, U_None,  U_None,  U_None,  U_##KK4, U_None,  U_##KK6, U_None,	\
/* L */ U_##KL0, U_None,  U_None,  U_None,  U_##KL4, U_None,  U_None,  U_None,	\
/* M */ U_None,  U_None,  U_None,  U_##KM3, U_None,  U_None,  U_None,  U_None,	\
/* N */ U_None,  U_None,  U_##KN2, U_##KN3, U_None,  U_None,  U_None,  U_None,	\
/* O */ U_##KO0, U_None,  U_None,  U_None,  U_##KO4, U_None,  U_##KO6, U_##KO7,	\
/* P */ U_None,  U_None,  U_##KP2, U_None,  U_None,  U_None,  U_None,  U_None,	\
/* Q */ U_None,  U_None,  U_None,  U_None,  U_##KQ4, U_None,  U_None,  U_##KQ7,	\
/* R */ U_##KR0, U_None,  U_##KR2, U_##KR3, U_##KR4, U_##KR5, U_##KR6, U_##KR7 	\
}

/*
,---.   ,---------------. ,---------------. ,---------------. ,-----------.
|Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|
`---'   `---------------' `---------------' `---------------' `-----------'
,-----------------------------------------------------------. ,-----------. ,---------------.
| ~ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | - | = | Backsp| |Ins|Hom|PgU| |NuL| / | * | - |
|-----------------------------------------------------------| |-----------| |---------------|
| Tab | Q | W | E | R | T | Y | U | I | O | P | [ | ] |     | |Del|End|PgD| | 7 | 8 | 9 | + |
|------------------------------------------------------.    | `-----------' |-----------|   |
|CapsL | A | S | D | F | G | H | J | K | L | ; | ' | \ | Ret|               | 5 | 6 | 6 |   |
|-----------------------------------------------------------|     ,---.     |---------------|
|Shft|ISO| Z | X | C | V | B | N | M | , | . | / |    Shift |     |Up |     | 1 | 2 | 3 | E |
|-----------------------------------------------------------| ,-----------. |-----------| n |
|Ctl|Gui|Alt|          Space                |Alt|Gui|App|Ctl| |Lef|Dow|Rig| | 0     | . | t |
`-----------------------------------------------------------' `-----------' `---------------'
*/

#define KEYMAP_ISO( \
    KJ6,      KI4, KH4, KH2, KH6, KA7, KE6, KD2, KD4, KB4, KB7, KB6, KB0,   KC7, KC5, KA5,                       \
    KJ4, KJ7, KI7, KH7, KG7, KG4, KF4, KF7, KE7, KD7, KR7, KR4, KE4, KB2,   KL4, KO4, KQ4,   KK1, KL1, KQ1, KQ0, \
    KJ2, KJ5, KI5, KH5, KG5, KG2, KF2, KF5, KE5, KD5, KR5, KR2, KE2,        KK4, KO7, KQ7,   KK5, KL5, KQ5, KO5, \
    KI2, KJ3, KI3, KH3, KG3, KG6, KF6, KF3, KE3, KD3, KR3, KR6, KB3, KB1,                    KK2, KL2, KQ2,      \
    KN2, KI6, KJ1, KI1, KH1, KG1, KG0, KF0, KF1, KE1, KD1, KR0,      KN3,        KO6,        KK3, KL3, KQ3, KO3, \
    KA4, KP2, KC6,                KK6,                KC0, KM3, KD0, KA1,   KO0, KK0, KL0,   KL6,      KQ6     ) \
{										\
/*      0        1        2        3        4        5        6        7   */	\
/* A */ U_None,  U_##KA1, U_None,  U_None,  U_##KA4, U_##KA5, U_None,  U_##KA7,	\
/* B */ U_##KB0, U_##KB1, U_##KB2, U_##KB3, U_##KB4, U_None,  U_##KB6, U_##KB7,	\
/* C */ U_##KC0, U_None,  U_None,  U_None,  U_None,  U_##KC5, U_##KC6, U_##KC7,	\
/* D */ U_##KD0, U_##KD1, U_##KD2, U_##KD3, U_##KD4, U_##KD5, U_None,  U_##KD7,	\
/* E */ U_None,  U_##KE1, U_##KE2, U_##KE3, U_##KE4, U_##KE5, U_##KE6, U_##KE7,	\
/* F */ U_##KF0, U_##KF1, U_##KF2, U_##KF3, U_##KF4, U_##KF5, U_##KF6, U_##KF7,	\
/* G */ U_##KG0, U_##KG1, U_##KG2, U_##KG3, U_##KG4, U_##KG5, U_##KG6, U_##KG7,	\
/* H */ U_None,  U_##KH1, U_##KH2, U_##KH3, U_##KH4, U_##KH5, U_##KH6, U_##KH7,	\
/* I */ U_None,  U_##KI1, U_##KI2, U_##KI3, U_##KI4, U_##KI5, U_##KI6, U_##KI7,	\
/* J */ U_None,  U_##KJ1, U_##KJ2, U_##KJ3, U_##KJ4, U_##KJ5, U_##KJ6, U_##KJ7,	\
/* K */ U_##KK0, U_##KK1, U_##KK2, U_##KK3, U_##KK4, U_##KK5, U_##KK6, U_None,	\
/* L */ U_##KL0, U_##KL1, U_##KL2, U_##KL3, U_##KL4, U_##KL5, U_##KL6, U_None,	\
/* M */ U_None,  U_None,  U_None,  U_##KM3, U_None,  U_None,  U_None,  U_None,	\
/* N */ U_None,  U_None,  U_##KN2, U_##KN3, U_None,  U_None,  U_None,  U_None,	\
/* O */ U_##KO0, U_None,  U_None,  U_##KO3, U_##KO4, U_##KO5, U_##KO6, U_##KO7,	\
/* P */ U_None,  U_None,  U_##KP2, U_None,  U_None,  U_None,  U_None,  U_None,	\
/* Q */ U_##KQ0, U_##KQ1, U_##KQ2, U_##KQ3, U_##KQ4, U_##KQ5, U_##KQ6, U_##KQ7,	\
/* R */ U_##KR0, U_None,  U_##KR2, U_##KR3, U_##KR4, U_##KR5, U_##KR6, U_##KR7 	\
}

/*
,---.   ,---------------. ,---------------. ,---------------. ,-----------.
|Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|
`---'   `---------------' `---------------' `---------------' `-----------'
,-----------------------------------------------------------. ,-----------.
| ~ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | - | = | Backsp| |Ins|Hom|PgU|
|-----------------------------------------------------------| |-----------|
| Tab | Q | W | E | R | T | Y | U | I | O | P | [ | ] |     | |Del|End|PgD|
|------------------------------------------------------.    | `-----------'
|CapsL | A | S | D | F | G | H | J | K | L | ; | ' | \ | Ret|
|-----------------------------------------------------------|     ,---.
|Shft|ISO| Z | X | C | V | B | N | M | , | . | / |    Shift |     |Up |
|-----------------------------------------------------------| ,-----------.
|Ctl|Gui|Alt|          Space                |Alt|Gui|App|Ctl| |Lef|Dow|Rig|
`-----------------------------------------------------------' `-----------'
*/

#define KEYMAP_ISO_TKL( \
    KJ6,      KI4, KH4, KH2, KH6, KA7, KE6, KD2, KD4, KB4, KB7, KB6, KB0,   KC7, KC5, KA5,	\
    KJ4, KJ7, KI7, KH7, KG7, KG4, KF4, KF7, KE7, KD7, KR7, KR4, KE4, KB2,   KL4, KO4, KQ4,	\
    KJ2, KJ5, KI5, KH5, KG5, KG2, KF2, KF5, KE5, KD5, KR5, KR2, KE2,        KK4, KO7, KQ7,	\
    KI2, KJ3, KI3, KH3, KG3, KG6, KF6, KF3, KE3, KD3, KR3, KR6, KB3, KB1,			\
    KN2, KI6, KJ1, KI1, KH1, KG1, KG0, KF0, KF1, KE1, KD1, KR0,      KN3,        KO6,		\
    KA4, KP2, KC6,                KK6,                KC0, KM3, KD0, KA1,   KO0, KK0, KL0 )	\
{										\
/*      0        1        2        3        4        5        6        7   */	\
/* A */ U_None,  U_##KA1, U_None,  U_None,  U_##KA4, U_##KA5, U_None,  U_##KA7,	\
/* B */ U_##KB0, U_##KB1, U_##KB2, U_##KB3, U_##KB4, U_None,  U_##KB6, U_##KB7,	\
/* C */ U_##KC0, U_None,  U_None,  U_None,  U_None,  U_##KC5, U_##KC6, U_##KC7,	\
/* D */ U_##KD0, U_##KD1, U_##KD2, U_##KD3, U_##KD4, U_##KD5, U_None,  U_##KD7,	\
/* E */ U_None,  U_##KE1, U_##KE2, U_##KE3, U_##KE4, U_##KE5, U_##KE6, U_##KE7,	\
/* F */ U_##KF0, U_##KF1, U_##KF2, U_##KF3, U_##KF4, U_##KF5, U_##KF6, U_##KF7,	\
/* G */ U_##KG0, U_##KG1, U_##KG2, U_##KG3, U_##KG4, U_##KG5, U_##KG6, U_##KG7,	\
/* H */ U_None,  U_##KH1, U_##KH2, U_##KH3, U_##KH4, U_##KH5, U_##KH6, U_##KH7,	\
/* I */ U_None,  U_##KI1, U_##KI2, U_##KI3, U_##KI4, U_##KI5, U_##KI6, U_##KI7,	\
/* J */ U_None,  U_##KJ1, U_##KJ2, U_##KJ3, U_##KJ4, U_##KJ5, U_##KJ6, U_##KJ7,	\
/* K */ U_##KK0, U_None,  U_None,  U_None,  U_##KK4, U_None,  U_##KK6, U_None,	\
/* L */ U_##KL0, U_None,  U_None,  U_None,  U_##KL4, U_None,  U_None,  U_None,	\
/* M */ U_None,  U_None,  U_None,  U_##KM3, U_None,  U_None,  U_None,  U_None,	\
/* N */ U_None,  U_None,  U_##KN2, U_##KN3, U_None,  U_None,  U_None,  U_None,	\
/* O */ U_##KO0, U_None,  U_None,  U_None,  U_##KO4, U_None,  U_##KO6, U_##KO7,	\
/* P */ U_None,  U_None,  U_##KP2, U_None,  U_None,  U_None,  U_None,  U_None,	\
/* Q */ U_None,  U_None,  U_None,  U_None,  U_##KQ4, U_None,  U_None,  U_##KQ7,	\
/* R */ U_##KR0, U_None,  U_##KR2, U_##KR3, U_##KR4, U_##KR5, U_##KR6, U_##KR7 	\
}

//-------------------------------------------------------------------------------

#endif	// __matrix_h__
