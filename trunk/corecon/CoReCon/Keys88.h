/*******************************************************************************
 * File Name	: Keys88.h
 * Project	: Costar Replacement Controllers
 * Date		: 2014/05/24
 * Version	: 1.0
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 *
 * $Id$
 ******************************************************************************/

#ifndef __Keys88_h__
#define __Keys88_h__

//------------------------------------------------------------------------------
// Key Matrix Layers - ISO 88

/*
,---.   ,---------------. ,---------------. ,---------------. ,-----------.
|Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|
`---'   `---------------' `---------------' `---------------' `-----------'
,-----------------------------------------------------------. ,-----------.
| ~ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | - | = | Backsp| |Ins|Hom|PgU|
|-----------------------------------------------------------| |-----------|
| Tab | Q | W | E | R | T | Y | U | I | O | P | [ | ] |     | |Del|End|PgD|
|------------------------------------------------------.    | `-----------'
|CapsL | A | S | D | F | G | H | J | K | L | ; | ' | \ |Ret |
|-----------------------------------------------------------|     ,---.
|Shft|ISO| Z | X | C | V | B | N | M | , | . | / |    Shift |     |Up |
|-----------------------------------------------------------| ,-----------.
|Ctl|Gui|Alt|          Space                |Alt|Gui|App|Ctl| |Lef|Dow|Rig|
`-----------------------------------------------------------' `-----------'
*/

#define U_L1 KEYMAP_ISO_TKL( \
  Esc,       F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10, F11, F12, PrSc,LNLk,Paus,	\
  Grv,   1,   2,   3,   4,   5,   6,   7,   8,   9,   0,Mins, Eql,  BS,  Ins,Home,PgUp,	\
  Tab,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,LBrc,RBrc,       Del, End,PgDn,	\
 CLck,   A,   S,   D,   F,   G,   H,   J,   K,   L,SCln,Quot,BkSl, Ent,			\
 LSft,NBSl,   Z,   X,   C,   V,   B,   N,   M, Com, Dot,Slsh,     RSft,        Up,	\
 LCtl,LGui,LAlt,              Space,               RAlt,RGui,  FN,RCtl, Left,Down,Rght )

#define U_L2 KEYMAP_ISO_TKL( \
 TOff,       F1,  F2,  F3,  F4, CPP,CStp,CBwd,CFwd,WLck,CMte,CVDn,CVUp, PrSc,NLck, CPP,	\
  Grv, 1mn,   2,30mn,   4, 5mn,60mn,   7,   8,   9,10mn,Mins, Eql,  BS, CMte,CBwd,CVUp,	\
  Tab,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,LBrc,RBrc,      CStp,CFwd,CVDn,	\
 CLck,CCfg,   S,   D,   F,   G,   H,   J,   K,   L,SCln,Quot,BkSl, Ent,			\
 LSft,NBSl,   Z,   X,CCal,   V,CAHm,   N,CEml, Com, Dot,Slsh,     RSft,      PgUp,	\
 LCtl,WLck,LAlt,              Space,               RAlt,WLck,  FN,  FN, Home,PgDn, End )

#define U_L3 KEYMAP_ISO_TKL( \
  Esc,       M1,  M2,  M3,  M4,  M5,  M6,None,None,SPDn,SSlp,None,Lock, BtLd,SLck, CPP,	\
 None,None,None,None,None,None,None,Num7,Num8,Num9,Num0,NMns, Eql,  BS, CBwd,CFwd,CVUp,	\
  Tab,None,None,None,None,None,None,Num4,Num5,Num6,None,LBrc,RBrc,      Demo,CMte,CVDn,	\
 NLck,None,None,None,None,None,None,Num1,Num2,Num3,NPls,NMul,BkSl,NEnt,			\
 LSft,NBSl,None,None,None,None,None,None,Num0, Com,NDot,NDiv,     RSft,      LBUp,	\
 LCtl,LGui,LAlt,              Space,               RAlt,RGui,  FN,  FN, CBwd,LBDn,CFwd )

#define U_L4 KEYMAP_ISO_TKL( \
  Esc,       F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10, F11, F12, PrSc,LNLk,Paus,	\
  Grv,   1,   2,   3,   4,   5,   6,Num7,Num8,Num9,   0,NMns, Eql,  BS,  Ins,Home,PgUp,	\
  Tab,   Q,   W,   E,   R,   T,   Y,Num4,Num5,Num6,   P,LBrc,RBrc,       Del, End,PgDn,	\
 NLck,   A,   S,   D,   F,   G,   H,Num1,Num2,Num3,NPls,NMul,BkSl,NEnt,			\
 LSft,NBSl,   Z,   X,   C,   V,   B,   N,Num0, Com,NDot,NDiv,     RSft,        Up,	\
 LCtl,LGui,LAlt,              Space,               RAlt,RGui,  FN,RCtl, Left,Down,Rght )

//------------------------------------------------------------------------------

#endif	// __Keys88_h__
