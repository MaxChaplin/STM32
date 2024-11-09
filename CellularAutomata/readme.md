# Cellular Automata

## Description

This is a project for the STM32F407VET6 board which uses the ILI9341 LCD display and XPT2046 touchscreen controller. It is a visualization of [elementary cellular automatons](https://en.wikipedia.org/wiki/Elementary_cellular_automaton) - each row of pixels is generated from the previous one according to a local and deterministic rule. The rule specifies how each pixel in a new row is decided according to each possible permutation of its neighbors in the previous row. It is visualized on the top of the display, and its number appears in the lower center part.

The initial state is a single pixel in the middle of the row. Each subsequent row appears immediately below the previous one, except for the row that follows the bottom one, which wraps around back to the top.

## Libraries used

- [ili9341 driver by Andriy Honcharenko](https://github.com/taburyak/STM32-ILI9341-320x240-FSMC-Library): provides functions for initializing the ili9341 display, as well as a variety of graphics functions for displaying text and drawing geometric figures.

- XPT2046 driver: provides functions for telling whether the touch screen has been touched, and where.

# Controls

- +1: Increase rule by 1 (up to a maximum of 255).
- -1: Decrease rule by 1 (down to a minimum of 0).
- +10: Increase rule by 10.
- -10: Decrease rule by 10.
- ?: Show a short description of the principle.
- RESET: place the line at the top of the window and set the row to the initial state.
- PAUSE/UNPAUSE: pause or unpause the iteration

## Buttons and text fields

The project includes a library of structure definitions and functions that can be used to create touchscreen buttons, as well as text fields. The button structure isn't dependednt on any specific touchscreen library.

In order to use the interface library in a different project, copy the following files:

- touchscreen_button.h
- touchscreen_button.c
- text_field.c
- text_field.h