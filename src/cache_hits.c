// cache_hits.c
// variables and definitions for keeping track of cache hits, (ie, those we cache in the transposition tables), and printing them.

// global variables for keeping track of cache hits:
uint64_t score_ops;
uint64_t quiet_ops;
uint64_t score_writes;
uint64_t quiet_writes;
uint64_t score_exact_hits;
uint64_t quiet_exact_hits;
uint64_t score_lb_cutoff_hits;
uint64_t quiet_lb_cutoff_hits;
uint64_t score_lb_nocutoff_hits;
uint64_t quiet_lb_nocutoff_hits;
uint64_t score_move_hits;
uint64_t quiet_move_hits;

void print_cache_hits(void)
{
    fprintf(stdout,
        "Writes: %d\nSOps\tSWrites\tSExact\tSLBCut\tSLBNCut\tSMove\n%d\t%d\t%d\t%d\t%d\t%d\n\t%d\t%d\t%d\t%d\t%d\nQOps\tQWrites\tQExact\tQLBCut\tQLBNCut\tQMove\n%d\t%d\t%d\t%d\t%d\t%d\n\t%d\t%d\t%d\t%d\t%d\n\n",
        cache_writes, score_ops, score_writes, score_exact_hits,
        score_lb_cutoff_hits, score_lb_nocutoff_hits, score_move_hits,
        (score_writes * 1000) / (score_ops + 1),
        (score_exact_hits * 1000) / (score_ops + 1),
        (score_lb_cutoff_hits * 1000) / (score_ops + 1),
        (score_lb_nocutoff_hits * 1000) / (score_ops + 1),
        (score_move_hits * 1000) / (score_ops + 1), quiet_ops,
        quiet_writes, quiet_exact_hits, quiet_lb_cutoff_hits,
        quiet_lb_nocutoff_hits, quiet_move_hits,
        (quiet_writes * 1000) / (quiet_ops + 1),
        (quiet_exact_hits * 1000) / (quiet_ops + 1),
        (quiet_lb_cutoff_hits * 1000) / (quiet_ops + 1),
        (quiet_lb_nocutoff_hits * 1000) / (quiet_ops + 1),
        (quiet_move_hits * 1000) / (quiet_ops + 1)
        );

    return;
}

void zero_cache_counts(void)
{
    cache_writes = 0;
    score_ops = 0;
    quiet_ops = 0;
    score_exact_hits = 0;
    quiet_exact_hits = 0;
    score_lb_cutoff_hits = 0;
    quiet_lb_cutoff_hits = 0;
    score_lb_nocutoff_hits = 0;
    quiet_lb_nocutoff_hits = 0;
    score_move_hits = 0;
    quiet_move_hits = 0;

    return;
}
