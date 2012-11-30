# learn.py
# scripts for learning parameters in params.h

import sh
import sys
import random
from copy import deepcopy
from math import log, ceil
from params import CConstant, ParamSet

def speedoptimize(iterations, purturb_count, fen_count, sigma, pgn_file):
    """Optimizes the search parameters for speed only.
    Will overwrite params.h--that's why we have version control!
    """

    random_fens = init_fen_file(pgn_file, iterations * fen_count)

    paramset = ParamSet("../src/params.h")
    for n in xrange(iterations):
        with open(".temp_fens", "w") as f:
            for line in random_fens[n * fen_count : (n+1) * fen_count]:
                f.write(line + '\n')
        paramset = speed_iteration(paramset, purturb_count, sigma)

    cleanup()

def init_fen_file(pgn_file, fen_count):
    """Returns fen_count random fens from pgn_file"""
    run = sh.Command("./randomfens.sh")
    res = run(pgn_file, str(fen_count))
    return str(res).rstrip('\n').split('\n')

def cleanup():
    sh.rm(".temp_fens")
    sh.rm(".temp_plies")

def speed_iteration(pset_orig, purturb_count, sigma):
    """Finds a better set of parameters near paramset, and returns it"""

    speeds = [init_fens(pset_orig)]
    purturbed = purturb(pset_orig, purturb_count, sigma)
    for pset in purturbed:
        speeds.append(speedtest(pset))

    candidates = [pset_orig] + purturbed

    # This is wrongish; should do some kind of modeling or something
    return candidates[speeds.index(max(speeds))]

def init_fens(pset_orig):
    """Initializes the plies and returns the speed of pset_orig on them"""
    recompile(pset_orig)
    run = sh.Command("speedbenchmark/a.out")
    speed = float(str(run("--new-plies", "5.0", "--fen-file", ".temp_fens", "--ply-file", ".temp_plies", "--average-only")))

    return speed

def speedtest(pset):
    """Returns the speed of pset on the current fens"""
    recompile(pset)
    run = sh.Command("speedbenchmark/a.out")
    speed = float(str(run("--fen-file", ".temp_fens", "--ply-file", ".temp_plies", "--average-only")))

    return speed

def purturb(pset_orig, purturb_count, sigma):
    """Returns a list of psets purturbed from pset_orig"""
    result = []
    for n in xrange(purturb_count):
        pset = deepcopy(pset_orig)
        for item in pset.sections["Search Parameters"]:
            if isinstance(item, CConstant):
                purturb_CConstant(item, sigma)
        result.append(pset)

    return result

def purturb_CConstant(c, sigma):
    """Purturbs a CConstant"""
    gtzero = True if c.name in ["ASPIRATION_WINDOW"] else False

    if c.dims:
        c.data = purturb_array(c.data, gtzero, sigma)
    else:
        c.data = purturb_x(c.data, gtzero, sigma)

def purturb_array(array, gtzero, sigma):
    """Purturbs an arbitrary dimensional array"""
    try:
        for i, x in enumerate(array):
            array[i] = purturb_array(x, gtzero, sigma)
        return array
    except TypeError: # not iterable
         return purturb_x(array, gtzero, sigma)

def purturb_x(x, gtzero, sigma):
    """Purturbs x, noting whether it must be > 0"""

    if gtzero:
        sig = sigma
        res = x * random.lognormvariate(-sig*sig/2, sig)
        return int(ceil(res)) if isinstance(x, int) else res
    else:
        sig = abs(x) * sigma
        res = random.gauss(x, sig)
        return int(res) if isinstance(x, int) else res

def recompile(pset):
    """Overwrite params.h and recompiles the speedbenchmark code"""
    with open("../src/params.h", "w") as f:
        f.write(str(pset))
    sh.cd("speedbenchmark")
    sh.make("clean")
    sh.make()
    sh.make("clean")
    sh.cd("../")

def main():
    speedoptimize(3, 3, 1, 0.1, sys.argv[1])

if __name__ == "__main__":
    main()
