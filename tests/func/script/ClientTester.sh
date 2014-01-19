#!/bin/sh

if nc -V | grep 'GNU Netcat' >/dev/null 2>&1; then
	NCOPTS=-lp
else
	NCOPTS=-l
fi

nc "${NCOPTS}" 9243 > out/a &
echo valami > out/b
sleep 0.1
( cat out/b; sleep 0.1 ) | out/testFuncs ClientTester 127.0.0.1 9243 2>/dev/null
diff -u3 out/a out/b
