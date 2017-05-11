#!//bin/sh
# This script runs all tests until a non-zero return is encountered.
# Returns if all tests ran successfully, -1 otherwise.

TEST_DIR=$(dirname $(realpath $0))

for i in "$TEST_DIR"/*.test; do
	printf -- "%0.s-" {1..78}
	printf -- "\n$(basename $i)\n"
	"$i"
	if ! [ $? -eq 0 ]; then
		exit -1
	fi
done

printf -- "%0.s-" {1..78}
printf -- "\nAll tests passed!\n"

exit 0
