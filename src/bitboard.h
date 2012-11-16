// bitboard.h
// defs for the bitboard

// if unsigned char index is the position of a piece on liboard, then
// index & FILEMASK is the file of the piece, and 
// index >> ROWSHIFT is the row of the piece
#define FILEMASK	0x07
#define ROWSHIFT	3

const uint64_t board[8][8] =
{{0x0000000000000001,0x0000000000000002,0x0000000000000004,0x0000000000000008,0x0000000000000010,0x0000000000000020,0x0000000000000040,0x0000000000000080},
 {0x0000000000000100,0x0000000000000200,0x0000000000000400,0x0000000000000800,0x0000000000001000,0x0000000000002000,0x0000000000004000,0x0000000000008000},
 {0x0000000000010000,0x0000000000020000,0x0000000000040000,0x0000000000080000,0x0000000000100000,0x0000000000200000,0x0000000000400000,0x0000000000800000},
 {0x0000000001000000,0x0000000002000000,0x0000000004000000,0x0000000008000000,0x0000000010000000,0x0000000020000000,0x0000000040000000,0x0000000080000000},
 {0x0000000100000000,0x0000000200000000,0x0000000400000000,0x0000000800000000,0x0000001000000000,0x0000002000000000,0x0000004000000000,0x0000008000000000},
 {0x0000010000000000,0x0000020000000000,0x0000040000000000,0x0000080000000000,0x0000100000000000,0x0000200000000000,0x0000400000000000,0x0000800000000000},
 {0x0001000000000000,0x0002000000000000,0x0004000000000000,0x0008000000000000,0x0010000000000000,0x0020000000000000,0x0040000000000000,0x0080000000000000},
 {0x0100000000000000,0x0200000000000000,0x0400000000000000,0x0800000000000000,0x1000000000000000,0x2000000000000000,0x4000000000000000,0x8000000000000000}};

const uint64_t linboard[64] =
{0x0000000000000001,0x0000000000000002,0x0000000000000004,0x0000000000000008,0x0000000000000010,0x0000000000000020,0x0000000000000040,0x0000000000000080,
 0x0000000000000100,0x0000000000000200,0x0000000000000400,0x0000000000000800,0x0000000000001000,0x0000000000002000,0x0000000000004000,0x0000000000008000,
 0x0000000000010000,0x0000000000020000,0x0000000000040000,0x0000000000080000,0x0000000000100000,0x0000000000200000,0x0000000000400000,0x0000000000800000,
 0x0000000001000000,0x0000000002000000,0x0000000004000000,0x0000000008000000,0x0000000010000000,0x0000000020000000,0x0000000040000000,0x0000000080000000,
 0x0000000100000000,0x0000000200000000,0x0000000400000000,0x0000000800000000,0x0000001000000000,0x0000002000000000,0x0000004000000000,0x0000008000000000,
 0x0000010000000000,0x0000020000000000,0x0000040000000000,0x0000080000000000,0x0000100000000000,0x0000200000000000,0x0000400000000000,0x0000800000000000,
 0x0001000000000000,0x0002000000000000,0x0004000000000000,0x0008000000000000,0x0010000000000000,0x0020000000000000,0x0040000000000000,0x0080000000000000,
 0x0100000000000000,0x0200000000000000,0x0400000000000000,0x0800000000000000,0x1000000000000000,0x2000000000000000,0x4000000000000000,0x8000000000000000};

const uint64_t notlinboard[64] = {
0xfffffffffffffffe,
0xfffffffffffffffd,
0xfffffffffffffffb,
0xfffffffffffffff7,
0xffffffffffffffef,
0xffffffffffffffdf,
0xffffffffffffffbf,
0xffffffffffffff7f,
0xfffffffffffffeff,
0xfffffffffffffdff,
0xfffffffffffffbff,
0xfffffffffffff7ff,
0xffffffffffffefff,
0xffffffffffffdfff,
0xffffffffffffbfff,
0xffffffffffff7fff,
0xfffffffffffeffff,
0xfffffffffffdffff,
0xfffffffffffbffff,
0xfffffffffff7ffff,
0xffffffffffefffff,
0xffffffffffdfffff,
0xffffffffffbfffff,
0xffffffffff7fffff,
0xfffffffffeffffff,
0xfffffffffdffffff,
0xfffffffffbffffff,
0xfffffffff7ffffff,
0xffffffffefffffff,
0xffffffffdfffffff,
0xffffffffbfffffff,
0xffffffff7fffffff,
0xfffffffeffffffff,
0xfffffffdffffffff,
0xfffffffbffffffff,
0xfffffff7ffffffff,
0xffffffefffffffff,
0xffffffdfffffffff,
0xffffffbfffffffff,
0xffffff7fffffffff,
0xfffffeffffffffff,
0xfffffdffffffffff,
0xfffffbffffffffff,
0xfffff7ffffffffff,
0xffffefffffffffff,
0xffffdfffffffffff,
0xffffbfffffffffff,
0xffff7fffffffffff,
0xfffeffffffffffff,
0xfffdffffffffffff,
0xfffbffffffffffff,
0xfff7ffffffffffff,
0xffefffffffffffff,
0xffdfffffffffffff,
0xffbfffffffffffff,
0xff7fffffffffffff,
0xfeffffffffffffff,
0xfdffffffffffffff,
0xfbffffffffffffff,
0xf7ffffffffffffff,
0xefffffffffffffff,
0xdfffffffffffffff,
0xbfffffffffffffff,
0x7fffffffffffffff,
};