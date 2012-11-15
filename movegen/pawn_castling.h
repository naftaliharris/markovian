// pawn_castling.h
// constants for where pawns (and kings) may attack castling squares
// In the following, Pawn_Castling[0] = WKING, Pawn_Castling[1] = WQUEEN, Pawn_Castling[2] = BKING, ...[3] = BQUEEN

#define PC_WKING 0
#define PC_WQUEEN 1
#define PC_BKING 2
#define PC_BQUEEN 3

const uint64_t Pawn_Castling[4] = {
0xf800,
0x3e00,
0xf8000000000000,
0x3e000000000000,
};
