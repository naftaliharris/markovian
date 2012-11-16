// ep_squares.h
// squares on which an en passant capture may occur
// ep_squares[COLOR][file] is the square to which a pawn of notCOLOR may move and kill a pawn of COLOR.
// file = 8 means no captures are possible, so ep_squares[COLOR][8] = 0

const uint64_t ep_squares[2][9] = {
{
0x10000,
0x20000,
0x40000,
0x80000,
0x100000,
0x200000,
0x400000,
0x800000,
0x0,
},
{
0x10000000000,
0x20000000000,
0x40000000000,
0x80000000000,
0x100000000000,
0x200000000000,
0x400000000000,
0x800000000000,
0x0,
},
};
