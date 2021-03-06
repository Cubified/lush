#!/bin/sh

SH=sh
BASH=bash
ZSH=zsh
FISH=fish
LUSH=../lush

check_shell() {
  if ! command -v $1 &> /dev/null; then
    eval "$2=NO"
  else
    printf "\x1b[35mDetected shell: $1\x1b[0m\n"
  fi
}

do_test() {
  if [ "$1" = "$2" ]; then
    printf "\x1b[36m  Now testing: $1\x1b[0m\n"
    hyperfine --warmup 15 "$3 | $1"
  fi
}

if ! command -v hyperfine &> /dev/null; then
  printf "Please install hyperfine to continue.\n"
  exit
fi

if ! [ -x $LUSH ]; then
  LUSH=./lush
  if ! [ -x $LUSH ]; then
    printf "Please compile lush first.\n"
    exit
  fi
fi

check_shell $SH SH
check_shell $BASH BASH
check_shell $ZSH ZSH
check_shell $FISH FISH

printf "\n\x1b[32mTest 1: Raw startup time\n"
do_test $SH   sh    "echo true"
do_test $BASH bash  "echo true"
do_test $ZSH  zsh   "echo true"
do_test $FISH fish  "echo true"
do_test $LUSH $LUSH "echo true"

printf "\n\x1b[32mTest 2: Simple command sequence\n"
do_test $SH   sh    "echo 'echo a && echo b && whoami && true'"
do_test $BASH bash  "echo 'echo a && echo b && whoami && true'"
do_test $ZSH  zsh   "echo 'echo a && echo b && whoami && true'"
do_test $FISH fish  "echo 'echo a && echo b && whoami && true'"
do_test $LUSH $LUSH "echo 'echo a && echo b && whoami && true'"

printf "\n\x1b[32mDone testing!\x1b[0m\n"
