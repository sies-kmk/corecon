/*******************************************************************************
 * File Name	: Keys87.h
 * Project	: Costar Replacement Controllers
 * Date		: 2014/05/24
 * Version	: 1.0
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 *
 * $Id$
 ******************************************************************************/

#ifndef __Keys87_h__
#define __Keys87_h__

//------------------------------------------------------------------------------
// Key Matrix Layers - ANSI 87

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
|Shft    | Z | X | C | V | B | N | M | , | . | / |    Shift |     |Up |
|-----------------------------------------------------------| ,-----------.
|Ctl|Gui|Alt|          Space                |Alt|Gui|App|Ctl| |Lef|Dow|Rig|
`-----------------------------------------------------------' `-----------'
*/

#define U_L1 KEYMAP_ANSI_TKL( \
  Esc,       F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10, F11, F12, PrSc,LNLk,Paus,	\
  Grv,   1,   2,   3,   4,   5,   6,   7,   8,   9,   0,Mins, Eql,  BS,  Ins,Home,PgUp,	\
  Tab,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,LBrc,RBrc,BkSl,  Del, End,PgDn,	\
 LCtl,   A,   S,   D,   F,   G,   H,   J,   K,   L,SCln,Quot,      Ent,			\
 LSft,        Z,   X,   C,   V,   B,   N,   M, Com, Dot,Slsh,     RSft,        Up,	\
 LCtl,  FN,LAlt,              Space,               RAlt,RGui, App,RCtl, Left,Down,Rght )

#define U_L2 KEYMAP_ANSI_TKL( \
 TOff,       F1,  F2,  F3,  F4, CPP,CStp,CBwd,CFwd,WLck,CMte,CVDn,CVUp, PrSc,NLck, CPP,	\
  Grv, 1mn,   2,30mn,   4, 5mn,60mn,   7,   8,   9,10mn,Mins, Eql,  BS, CMte,CBwd,CVUp,	\
  Tab,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,LBrc,RBrc,BkSl, CStp,CFwd,CVDn,	\
 LCtl,   A,   S,   D,   F,   G,   H,   J,   K,   L,SCln,Quot,      Ent,			\
 LSft,        Z,   X,CCal,  M6,CAHm,   N,CEml, Com, Dot,Slsh,     CLck,      PgUp,	\
 LCtl,  FN,  FN,              Space,               RAlt,WLck, App,RCtl, Home,PgDn, End )

#define U_L3 KEYMAP_ANSI_TKL( \
  Esc,       M1,  M2,  M3,  M4,  M5,  M6,None,None,SPDn,SSlp,None,Lock, BtLd,SLck, CPP,	\
 None,None,None,None,None,None,None,Num7,Num8,Num9,Num0,NMns, Eql,  BS, CBwd,CFwd,CVUp,	\
  Tab,None,None,None,None,None,None,Num4,Num5,Num6,None,LBrc,RBrc,BkSl, Demo,CMte,CVDn,	\
 NLck,None,None,None,None,None,None,Num1,Num2,Num3,NPls,NMul,     NEnt,			\
 LSft,     None,None,None,None,None,None,Num0, Com,NDot,NDiv,     RSft,      LBUp,	\
 LCtl,  FN,  FN,              Space,               RAlt,WLck, App,RCtl, CBwd,LBDn,CFwd )

#define U_L4 KEYMAP_ANSI_TKL( \
  Esc,       F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10, F11, F12, PrSc,LNLk,Paus,	\
  Grv,   1,   2,   3,   4,   5,   6,Num7,Num8,Num9,   0,NMns, Eql,  BS,  Ins,Home,PgUp,	\
  Tab,   Q,   W,   E,   R,   T,   Y,Num4,Num5,Num6,   P,LBrc,RBrc,BkSl,  Del, End,PgDn,	\
 NLck,   A,   S,   D,   F,   G,   H,Num1,Num2,Num3,NPls,NMul,     NEnt,			\
 LSft,        Z,   X,   C,   V,   B,   N,Num0, Com,NDot,NDiv,     RSft,        Up,	\
 LCtl,  FN,LAlt,              Space,               RAlt,RGui, App,RCtl, Left,Down,Rght)

//------------------------------------------------------------------------------

#endif	// __Keys87_h__
