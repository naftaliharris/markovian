# params.py
# tools for creating and editing the parameter file, params.h

from collections import OrderedDict
import re
import sys

class CConstant:
    """Object representing a constant in C"""
    def __init__(self, ctype=None, name=None, data=None, dims=None, string=None):
        self.ctype = ctype
        self.name = name
        self.data = data
        self.dims = dims if dims else []

        if string is not None:
            if not self.from_str(string):
                raise ValueError("Bad string given")

    def from_str(self, string):
        """Read the CConstant from a string and return True or False 
        on success or failure. Doesn't currently work for multiword 
        ctypes. (eg: long long)"""
        m = re.match(r"const (?P<ctype>\w+) (?P<name>\w+)(?P<dims>(\[\d+\])*) = (?P<data>[0-9.{}, -]+);", string)
        if not m:
            return False

        ctype = m.group('ctype')
        name = m.group('name')

        dims = [int(x) for x in m.group('dims').replace('[', ' ',).replace(']', ' ').strip().split()]
        if dims:
            data = self._read_array(m.group('data'))
        else:
            data = self._atox(m.group('data'))

        self.ctype = ctype
        self.name = name
        self.data = data
        self.dims = dims if dims else []
        return True

    def _atox(self, string):
        """Makes string into an int, float, or str, for use in from_str"""
        try: return int(string)
        except ValueError:
            try: return float(string)
            except ValueError: return string 

    def _read_array(self, string):
        """Reads a arbitrary-dimensional array in C into a list of lists, 
        for use in from_str"""
        m = re.match(r"{(?P<data>[0-9.{}, -]+)}", string)
        if m:
            return [self._read_array(elem) for elem in self._split_brackets(m.group('data'))]
        else:
            return self._atox(string)

    def _split_brackets(self, string):
        """Takes '{{1,2,3},{4,5,6}}' -> ['{1,2,3}','{4,5,6}'], for use in _read_array"""
        result = []
        bracketdepth = 0
        for i, c in enumerate(string):
            if c == '{':
                if bracketdepth == 0: 
                    start = i
                bracketdepth += 1
            elif c == '}':
                bracketdepth -= 1
                if bracketdepth == 0:
                    result.append(string[start: i+1])
        if not result:
            result = string.split(',')
        return result

    def __str__(self):
        """Returns the C-code version"""
        dimstr = "".join("[%d]" % dim for dim in self.dims)
        result = "const %s %s%s = %s;" % (self.ctype, self.name, dimstr, self._str_array(self.data))
        return result

    def _str_array(self, array):
        """Turns an arbitrary-dimensional array into a str, for use in __str__"""
        if isinstance(array, str): # special case since str's are iterable!
            return array
        try:
            return "{" + ", ".join(self._str_array(x) for x in array) + "}"
        except TypeError: # not iterable
            return str(array)

class ParamSet:
    """Object representing the set of parameters"""
    def __init__(self, filename=None):
        self.f = open(filename, "r") if filename else None
        self.sections = OrderedDict()
        if filename:
            self.readin()

    def readin(self):
        section = ""
        for line in self.f:
            # Check if the line introduces a new section
            m = re.match(r"/\* Section: (?P<section>[a-zA-Z0-9_ ]+) \*/", line)
            if m: 
                section = m.group('section')
                self.sections[section] = []
                continue

            # Try to parse the line as a CConstant, or else store it as text
            try:
                c = CConstant(string=line)
                self.sections[section].append(c)
            except ValueError:
                self.sections[section].append(line.strip())

    def __str__(self):
        """Prints back out the file"""
        result = ""
        for section in self.sections:
            result += "/* Section: %s */\n" % section
            result += "\n".join(str(x) for x in self.sections[section]) + "\n"

        return result

def main():
    paramset = ParamSet("../src/params.h")
    sys.stdout.write(str(paramset)) # no terminal newline

if __name__ == "__main__":
    main()
