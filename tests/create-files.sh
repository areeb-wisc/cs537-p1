#! /usr/bin/env bash

idx="$1"
touch "tests/${idx}.board"
touch "tests/${idx}.desc"
touch "tests/${idx}.err"
touch "tests/${idx}.in"
touch "tests/${idx}.pre"
touch "tests/${idx}.rc"
touch "tests/${idx}.run"
touch "tests/${idx}.out"

echo "make -C ../solution" > "tests/${idx}.pre"
echo "../solution/letter-boxed tests/${idx}.board ../dict.txt < tests/${idx}.in" > "tests/${idx}.run"

code "tests/${idx}.desc"
code "tests/${idx}.board"
code "tests/${idx}.in"
code "tests/${idx}.rc"
code "tests/${idx}.out"