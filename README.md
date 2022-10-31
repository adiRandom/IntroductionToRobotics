# Introduction to Robotics (2021 - 2022)

This repository will contain documentation, code and assets for homeworks in the Robotics Introduction Course taken in the 3rd year at the Faculty of Mathematics and Informatics of Bucharest 

# Table of contents
- [Homework 1](#homework-1)
- [Homework 2](#homework-2)


# Homework 1

## Requirement

Controll an RGB LED using 3 potentiometers (one for each color)
The values of the potentiometers should be read by the arduino uno board, then converted into output values for the RGB LED

## Files

tema_1/tema_1.ino

## Photo

![image](https://user-images.githubusercontent.com/38132701/197337422-8c798450-c0f8-4650-82fb-f66a8862aeaa.png)

## Video
https://photos.app.goo.gl/pk8tyzWqX2WVh13Y8

# Homework 1

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

![image](https://user-images.githubusercontent.com/38132701/199050479-5b611ed3-848c-47f9-ac10-5e45e4c251f1.png)

## Video
https://photos.app.goo.gl/Exs7KoChwKhdcjER8
