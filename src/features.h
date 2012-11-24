// features.h
// for selectively turning on and off features

// Definitions

//#define _PRINT_DEFS

//#define _DEBUG
//#define _XBOARD
#define _USE_SCORE_HASH
#define _USE_QUIET_HASH
#define _USE_QUIET
//#define _CACHE_HITS
//#define _NODE_COUNT
//#define _IT_DEP
#define _USE_CONTROL_EVAL
#define _USE_PAWN_EVAL
//#define _USE_COUNT_EVAL
//#define _USE_ASPIRATION_SEARCH
#define _USE_HISTORY

// Dependencies

#ifdef _CACHE_HITS
	#ifndef _NODE_COUNT
	#define _NODE_COUNT
	#endif
	#ifndef _USE_HASH
	#define _USE_HASH
	#endif
#endif



// Printouts

#ifdef _PRINT_DEFS

#ifdef _DEBUG
#warning "Debugging mode:			ON"
#else
#warning "Debugging mode:			OFF"
#endif

#ifdef _XBOARD
#warning "Xboard mode:			ON"
#else
#warning "Xboard mode:			OFF"
#endif

#ifdef _USE_QUIET
#warning "Use quiescience searching:		ON"
#else
#warning "Use quiescience searching:		OFF"
#endif

#ifdef _CACHE_HITS
#warning "Collect cache hit data:		ON"
#else
#warning "Collect cache hit data:		OFF"
#endif

#ifdef _USE_SCORE_HASH
#warning "Use transposition hashing in score:	ON"
#else
#warning "Use transposition hashing in score:	OFF"
#endif

#ifdef _USE_QUIET_HASH
#warning "Use transposition hashing in quiet:	ON"
#else
#warning "Use transposition hashing in quiet:	OFF"
#endif

#ifdef _NODE_COUNT
#warning "Collect node visits data:		ON"
#else
#warning "Collect node visits data:		OFF"
#endif

#ifdef _IT_DEP
#warning "Use iterative deepening:		ON"
#else
#warning "Use iterative deepening:		OFF"
#endif

#ifdef _USE_CONTROL_EVAL
#warning "Use control evaluation:		ON"
#else
#warning "Use control evaluation:		OFF"
#endif

#ifdef _USE_PAWN_EVAL
#warning "Use pawn structure evaluation:	ON"
#else
#warning "Use pawn structure evaluation:	OFF"
#endif

#ifdef _USE_COUNT_EVAL
#warning "Use move count evaluation:		ON"
#else
#warning "Use move count evaluation:		OFF"
#endif

#ifdef _USE_ASPIRATION_SEARCH
#warning "Use aspiration search:		ON"
#else
#warning "Use aspiration search:		OFF"
#endif

#ifdef _USE_HISTORY
#warning "Use history to find draws:		ON"
#else
#warning "Use history to find draws:		OFF"
#endif

#endif

