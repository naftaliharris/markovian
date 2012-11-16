// knight_castling.h
// constants for where knights may attack castling squares
// In the following, Knight_Castling[0] = WKING, Knight_Castling[1] = WQUEEN, Knight_Castling[2] = BKING, ...[3] = BQUEEN

#define KC_WKING 0
#define KC_WQUEEN 1
#define KC_BKING 2
#define KC_BQUEEN 3

const uint64_t Knight_Castling[4] = {
0xf8dc00,
0x3e7700,
0xdcf80000000000,
0x773e0000000000,
};
