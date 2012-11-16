#!/bin/bash

find . | grep "\.c$" | xargs indent -linux
find . | grep "\.c~$" | xargs rm
