## lush

A shell.  Written as a toy demo for my terminal line editor [`ledit`](https://github.com/Cubified/ledit.asm), using the reference C implementation.

### Demo

![Demo](https://github.com/Cubified/lush/blob/main/demo.gif)

### Features

- Syntax highlighting (a la `fish` or `oh-my-zsh`)
- Command and filename onion skin/ghosting (a la `fish` -- provides autofill suggestions based on programs in $PATH and files in current directory)
- Case-insensitive tab completion
  - Optionally, install [fzf](https://github.com/junegunn/fzf) for fuzzy file finding support.
- Functional line editor (arrow keys, Control+arrow keys for moving between words, home/end keys, backspace/delete)
- Aliases (see [`config.h`](https://github.com/Cubified/lush/blob/main/config.h))
- Persistent command history (saved to disk)
- ~8x faster than `fish`, and on par with `bash` and `zsh` speed-wise (see [benchmarks](https://github.com/Cubified/lush/blob/main/bench/results))
- 500 lines of pure `-pedantic` C

In its current state, `lush` works so long as you don't try to break it.  Not recommended for use as a daily driver, but still fun and satisfying to use.

My focus with this project has been less on creating the guts of a robust shell, and more on making the user interface as user-friendly and "bells-and-whistly" as possible.  As a result, features like builtins or aliases are somewhat hastily implemented and leave a good amount to be desired from a technical standpoint.  However, work to properly implement these features can be done without fear of causing the rest of the shell to implode, based on the way the properly-written parts of the codebase have been written.

### Compiling and Running

`lush` has no dependencies other than the C standard library, meaning it can be compiled and run with:

     $ make
     $ ./lush

### Configuration

Configuration of `lush` is done entirely at compile-time through the header file [`config.h`](https://github.com/Cubified/lush/blob/main/config.h).  Here are the options provided:

- Text colors
- PS1 (prompt)
- History file
- Aliases
- Builtins

### To-Do

- Environment variables (+ runtime PS1 customization)
- Fix escaping/quoted strings
- Fix onion skin not applying to builtins/aliases
- Multi-line editing
- Investigate and fix autocomplete occasionally not working
- Remove printfs where they aren't strictly necessary
