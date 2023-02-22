OK="[\e[1;32mOK\e[0m]\n"
KO="[\e[1;33mInstalling\e[0m]\n"

function check_program_installed()
{
	package=$1
	if test -n $2
	then package=$2
	fi

	printf "%-40s" "Checking for $1"
	if command -v $1 >/dev/null 2>/dev/null
	then printf $OK
	else printf $KO
		 sudo apt-get install -y $package
	fi
}

check_program_installed pip python3-pip

printf "%-40s" "Checking for conan"
if command -v conan >/dev/null 2>/dev/null
then printf $OK
else printf $KO
	pip install --user 'conan<2.0'
fi

printf "%-40s" "Checking for required conan packages"
if test $(conan search --raw libft/2.9 | wc -c) -gt 1 \
		-a $(conan search --raw libunit/1.4 | wc -c) -gt 1
then printf $OK
else printf $KO
	TMP_DIR=$(mktemp -d)
	git clone https://github.com/agagniere/Libft.git $TMP_DIR
	conan export $TMP_DIR
	conan export $TMP_DIR/test/framework
	rm -rf $TMP_DIR
fi

printf "%-40s" "Checking for pkg-config files"
if test -f libft.pc -a -f test/libunit.pc
then printf $OK
else printf $KO
	conan install . --build=missing
	( cd test && conan install . --build=missing )
fi

if test "$1" = "--doc"
then
	check_program_installed doxygen doxygen-latex
	check_program_installed help2man
fi
