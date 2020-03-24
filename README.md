# Visualizer-LED-s
Personal coding project to make lights that react to music

This project uses Python to read audio values from the sound card on your computer,
then creates a graph of the values that updates in realtime. This graph takes the form of 
an audio spectrum analyzer of sorts. I have implemented a small gui that allows the user to 
switch between 3 modes. The first one is a basic mode that rotates the LED color around the 
whole color spectrum. The next is the mode that reacts to music. The third mode allows the user
to pick the color that they want the lights to be. The pyton file sends all the necessary data
to an Arduino Uno, which controls the LED's

***This project is still a work in progress. I want to improve the function of the LED's,
the GUI, and attempt to make my python file an executable file.
