# Painter

<!-- ![Display](photo.jpg) -->

<img src="photo.jpg" width=30% height=30%>

## Description

This is a project for the STM32F407VET6 board which uses the ILI9341 LCD display and XPT2046 touchscreen controller. It's a simple drawing program with a circular brush.

## Libraries used

- [ili9341 driver by Andriy Honcharenko](https://github.com/taburyak/STM32-ILI9341-320x240-FSMC-Library): provides functions for initializing the ili9341 display, as well as a variety of graphics functions for displaying text and drawing geometric figures.

- XPT2046 driver: provides functions for telling whether the touch screen has been touched, and where.

# Interface

- Set the brush size with the "-" and "+" buttons.
- Set the brush color with the "<" and ">" buttons.
- Erase the display with the "CLR" button.
- The current brush is displayed in the bottom right.