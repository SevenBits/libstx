#!/usr/bin/env sh

while [ 1 ]; do
	for i in *.test; do
		./"$i"
		if [[ $? -eq 1 ]]; then
			break
		fi
	done
done
