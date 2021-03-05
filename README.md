## lush

A shell.  Written as a toy demo for my terminal line editor [`ledit`](https://github.com/Cubified/ledit.asm), using the reference C implementation.

My focus with this project has been less on creating the guts of a robust shell, and more on making the user interface as user-friendly and "bells-and-whistly" as possible.  As a result, features like builtins or aliases are somewhat hastily implemented and leave a good amount to be desired from an implementation standpoint.  However, work to properly implement these features can be done without fear of causing the rest of the shell to implode, based on the way the properly-written parts of the codebase have been written.

### Demo

![Demo](https://github.com/Cubified/lush/blob/main/demo.gif)

### Features

- Syntax highlighting (a la `fish` or `oh-my-zsh`)
- Command and filename onion skin/ghosting (a la `fish`)
- Functional line editor (arrow keys, Control+arrow keys for moving between words, home/end keys, backspace/delete)
- Aliases (see [`config.h`](https://github.com/Cubified/lush/blob/main/config.h))
- Command history
- <500 lines of pure `-pedantic` C

In its current state, `lush` works so long as you don't try to break it.  Not recommended for use as a daily driver, but still fun and satisfying to use.

### Compiling and Running

`lush` has no dependencies other than the C standard library, meaning it can be compiled and run with:

     $ make
     $ ./lush

### To-Do

- Fix history occasionally jumping between entries
- Environment variables
- Tab completion
