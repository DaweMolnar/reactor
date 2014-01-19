#!/bin/sh

nc -lp 9243 > out/a &
echo valami > out/b
sleep 0.1
( cat out/b; sleep 0.1 ) | out/testFuncs ClientTester 127.0.0.1 9243 2>/dev/null
diff -u3 out/a out/b
