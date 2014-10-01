/*******************************************************************************
 * File Name	: Keys105.h
 * Project	: Costar Replacement Controllers
 * Date		: 2014/05/24
 * Version	: 1.0
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 *
 * $Id$
 ******************************************************************************/

#ifndef __Keys105_h__
#define __Keys105_h__

//------------------------------------------------------------------------------
// Key Matrix Layers - ISO 105

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

#define U_L1 KEYMAP_ISO( \
  Esc,       F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10, F11, F12, PrSc,SLck,Paus,\
  Grv,   1,   2,   3,   4,   5,   6,   7,   8,   9,   0,Mins, Eql,  BS,  Ins,Home,PgUp, NLck,NDiv,NMul,NMns, \
  Tab,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,LBrc,RBrc,       Del, End,PgDn, Num7,Num8,Num9,NPls, \
 CLck,   A,   S,   D,   F,   G,   H,   J,   K,   L,SCln,Quot,BkSl, Ent,                 Num4,Num5,Num6,      \
 LSft,NBSl,   Z,   X,   C,   V,   B,   N,   M, Com, Dot,Slsh,     RSft,        Up,      Num1,Num2,Num3,NEnt, \
 LCtl,LGui,LAlt,              Space,               RAlt,RGui,  FN,RCtl, Left,Down,Rght, Num0,     NDot )

#define U_L2 KEYMAP_ISO( \
 TOff,       F1,  F2,  F3,  F4, CPP,CStp,CBwd,CFwd,WLck,CMte,CVDn,CVUp, PrSc,SLck, CPP,\
  Grv, 1mn,   2,30mn,   4, 5mn,60mn,   7,   8,   9,10mn,Mins, Eql,  BS, CMte,CBwd,CVUp, LNLk,NDiv,NMul,NMns, \
  Tab,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,LBrc,RBrc,      CStp,CFwd,CVDn, Num7,Num8,Num9,NPls, \
 CLck,CCfg,   S,   D,   F,   G,   H,   J,   K,   L,SCln,Quot,BkSl, Ent,                 Num4, 5mn,60mn,      \
 LSft,NBSl,   Z,   X,CCal,   V,CAHm,   N,CEml, Com, Dot,Slsh,     RSft,      PgUp,       1mn,Num2,30mn,NEnt, \
 LCtl,WLck,LAlt,              Space,               RAlt,WLck,  FN,  FN, Home,PgDn, End, 10mn,     NDot )

#define U_L3 KEYMAP_ISO( \
  Esc,       M1,  M2,  M3,  M4,  M5,  M6,None,None,SPDn,SSlp,None,Lock, BtLd,SLck, CPP,\
 None,None,None,None,None,None,None,Num7,Num8,Num9,Num0,NMns, Eql,  BS, CBwd,CFwd,CVUp, LNLk,Slsh,NMul,Mins, \
  Tab,None,None,None,None,None,None,Num4,Num5,Num6,None,LBrc,RBrc,      Demo,CMte,CVDn,    7,   8,   9,NPls, \
 CLck,None,None,None,None,None,None,Num1,Num2,Num3,NPls,NMul,BkSl,NEnt,                    4,   5,   6,      \
 LSft,NBSl,None,None,None,None,None,None,Num0, Com,NDot,NDiv,     RSft,      LBUp,         1,   2,   3, Ent, \
 LCtl,LGui,LAlt,              Space,               RAlt,RGui,  FN,  FN, CBwd,LBDn,CFwd,    0,      Dot )

#define U_L4 KEYMAP_ISO( \
  Esc,       F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10, F11, F12, PrSc,SLck,Paus,\
  Grv,   1,   2,   3,   4,   5,   6,   7,   8,   9,   0,Mins, Eql,  BS,  Ins,Home,PgUp, LNLk,NDiv,NMul,NMns, \
  Tab,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,LBrc,RBrc,       Del, End,PgDn,    7,   8,   9,NPls, \
 CLck,   A,   S,   D,   F,   G,   H,   J,   K,   L,SCln,Quot,BkSl, Ent,                    4,   5,   6,      \
 LSft,NBSl,   Z,   X,   C,   V,   B,   N,   M, Com, Dot,Slsh,     RSft,        Up,         1,   2,   3,NEnt, \
 LCtl,LGui,LAlt,              Space,               RAlt,RGui,  FN,RCtl, Left,Down,Rght,    0,      Dot )

//------------------------------------------------------------------------------

#endif	// __Keys105_h__
