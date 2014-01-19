#!/bin/sh

ok=yes
for i in $@; do
	echo "  TEST  ${i}"
	if ! tests/func/script/${i}.sh; then
		ok=no
	fi
done
test "${ok}" = yes
