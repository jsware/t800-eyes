# t800-eyes
Flickering &amp; Fading Terminator Eyes.

Wire up red LEDs to pins 5 and 6 and servo signals to pins 9 and 10. Connect Led ground to GND pin. Wire eye movement servos to +5v and GND.

Drive the Arduino from a 5v power source.

Each set of eyes on pins 5 and 6 will come on, flicker for a while and then fade off slowly as per T800s "dieing" in the movies. If connected to a servo, they move while flickering and stop when fading off.

Pin 5 goes with servo signal 9, pin 6 with servo pin 10.

## Scan Mode

Alternatively, you can fit this into the head of the T800.

Wire the eyes to pin 13 and the servo signal to pin 11.

The T800 eyes will scan slowly left and right with the eyes permanently on.
