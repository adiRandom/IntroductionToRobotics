# Introduction to Robotics (2021 - 2022)

This repository will contain documentation, code and assets for homeworks in the Robotics Introduction Course taken in the 3rd year at the Faculty of Mathematics and Informatics of Bucharest 

# Table of contents
- [Homework 1](#homework-1)
- [Homework 2](#homework-2)
- [Homework 3](#homework-3)
- [Homework 4](#homework-4)
- [Homework 5](#homework-5)


# Homework 1

## Requirement

Controll an RGB LED using 3 potentiometers (one for each color)
The values of the potentiometers should be read by the arduino uno board, then converted into output values for the RGB LED

## Files

tema_1/tema_1.ino

## Photo

<img src="https://user-images.githubusercontent.com/38132701/197337422-8c798450-c0f8-4650-82fb-f66a8862aeaa.png" width="350"/>

## Video
https://photos.app.goo.gl/pk8tyzWqX2WVh13Y8

# Homework 2

## Requirement

Using leds, a buzzer and a button, create a crosswalk. 

The system has 3 LEDs for the car traffic lights and 2 for the crosswalk itself
The system has 4 states it can be in
- Green for cars and red for pedestrians
- Yellow for cars and red for pedestrians
- Red for cars and green for pedestrians
- Red for cars and blinking green for bedestrians

Pressing the button starts the transition from the first state to the rest

When the pedestrian light is green, the buzzer should intermittently buzz. When the light is blinking, the buzzer should buzz more frequenlty.
The button also needs to implement a debounced input.

## Files

tema_2/tema_2.ino

## Photo

<img src="https://user-images.githubusercontent.com/38132701/199050479-5b611ed3-848c-47f9-ac10-5e45e4c251f1.png" width="350"/>

## Video

https://photos.app.goo.gl/Exs7KoChwKhdcjER8


# Homework 3

## Requirement

Using a joystick and a seven segments display, create a small canvas that you can draw on.
Initially, all but one segments on the display are off, with one blinking. 

The blinking segment can be moved around using the joystick
It can only be moved around to the neighbour segments (so no diagonal moves, segment skips, etc.)
When cliking the joystick, we select the blinking segment and we can decide, moving the joystick left or right, wether that segment is active or inactive. 
Pressing the joystick again unselects the segment and lets us move a blinking segment around the display, keeping the previously selected one in the state we chose

Long pressing the joystick resets the display


## Files 

tema_3/tema_3.ino

## Photos

<img src="https://user-images.githubusercontent.com/38132701/200429301-48a4817b-8d8f-4bcb-b214-a208d29919e0.png" width="350"/>

## Video

https://photos.app.goo.gl/jEzMFZLG985VzwXg9


# Homework 4

## Requirement

Using 4 7-segment displays and a joystick, display a 4 digit number. 
Using the joysticks one axis, select one of the 4 displays, select it pressing the button and then control the digit on the display with the other axis 
Each dispay can show a hexadecimal number
A shift register needs to be used to control the 4 displays

## Files

tema_4/tema_4.ino

## Photos

<img src="https://user-images.githubusercontent.com/38132701/201729786-cd30629c-c7b9-4cec-b21e-9063b8cb7db5.png" width="350"/>

## Video

https://photos.app.goo.gl/EA1Xu4bEBw2BjH46A


# Homework 5

## Requirement

Create the menu for the led matrix game using an LCD display
The menu should have:

- An intro screen
- An option to start the game
- A settings page with
  - Brightness
  - Difficulty
  - Player name
  - Reset highscore
  - Volume
- An about screen
- A help screen
- A highscore screen
- Option menues for all these screens
- A simple demo game

The settings should be saved to the EEPROM
A buzzer should be used to play music

The LCD should display game relates text and icons when the game starts

## What's done

- An intro screen
- An option to start the game
- An about screen
- A help screen
- Option menues for all these screens (partially - missing highscore option)
- A simple demo


## Files

tema_5/*


## Photos

<img src="https://user-images.githubusercontent.com/38132701/206277397-09d9e595-326f-428f-86a7-5251fa8ff0a8.png" width="350"/>

## Video

https://photos.app.goo.gl/GXCuzTCjfRW5twjEA
