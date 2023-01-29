OK="[\e[1;32mOK\e[0m]\n"
KO="[\e[1;33mInstalling\e[0m]\n"

printf "%-40s" "Checking for pip"
if command -v pip >/dev/null 2>/dev/null
then printf $OK
else printf $KO
	sudo apt install pip
fi

printf "%-40s" "Checking for conan"
if command -v conan >/dev/null 2>/dev/null
then printf $OK
else printf $KO
	pip install --user conan
fi

printf "%-40s" "Checking for required conan packages"
if test $(conan search --raw libft/2.6 | wc -c) -gt 1 \
		-a $(conan search --raw libunit/1.3 | wc -c) -gt 1
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
