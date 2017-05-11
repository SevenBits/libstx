#!/usr/bin/env sh

TEST_DIR=$(dirname $(realpath $0))

# Run all tests until a non-zero return is encountered.
for i in "$TEST_DIR"/*.test; do
	printf -- "%0.s-" {1..79}
	printf "$i\n"
	"$i"
	if ! [[ $? -eq 0 ]]; then
		exit -1
	fi
done

exit 0
