#!/usr/bin/env sh

loop=1

while [ 1 ]; do
	for i in *.test; do
		./"$i"
		if ! [[ $? -eq 0 ]]; then
			loop=0
		fi
	done
done
