#!/usr/bin/python
# runtests.py
# runs all the tests on Markovian

import unittest
import sh

class TestMarkovian(unittest.TestCase):

    def test_units(self):
        """Run the C unittests"""
        sh.make("unittest")
        run = sh.Command("./unittest")
        run() # will throw an exception upon nonzero exit code

    def test_search(self):
        """Make sure aspiration search is the same as ordinary search
        Uses random fens as values, so not guaranteed to produce the same
        output when run multiple times"""
        lines = str(sh.rl("test/data/fenio.fens", "--count=10")).rstrip('\n')

        sh.make("aspire_search")
        run = sh.Command("./aspire_search")
        aspire_output = str(run(sh.echo(lines)))

        sh.make("no_aspire_search")
        run = sh.Command("./no_aspire_search")
        no_aspire_output = str(run(sh.echo(lines)))

        for fen_orig, fen1, fen2 in zip(lines.split('\n'), 
                                        aspire_output.split('\n'), 
                                        no_aspire_output.split('\n')):
            self.assertEquals(fen1, fen2, "Original fen: '%s'" % fen_orig)

    def test_compilation(self):
        """Make sure all binaries compile"""
        for target in ["xboard", "debug", "profile", "cache-hits"]:
            sh.make(target)
            sh.make("clean")

    def tearDown(self):
        sh.make("clean")

if __name__ == "__main__":
    unittest.main()
