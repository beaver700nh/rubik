.PHONY: all clean build run finddws
.SILENT: all

PROJECT := rubik

ACTIONS := a - Quit\nb - Run

all:
	$(MAKE) build
	
	read -p $$'\n\e[1m$(ACTIONS)\e[0m\n\n% ';                \
	case $$REPLY in                                          \
	"a") echo "Quit." ;;                                     \
	"b") $(MAKE) run ;;                                      \
	esac

clean:
	rm -r build

build:
	(mkdir -p build; cd build; cmake ..; make -j4)

run:
	./build/bin/$(PROJECT)

finddws:
	grep -En '\s+$$' src/*

