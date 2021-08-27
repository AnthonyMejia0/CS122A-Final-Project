# CS122A-Final-Project
Final project for CS122A at UCR<br />
Intermediate Embedded and Real-Time Systems

## Introduction
Everybody requires their privacy. Whether it’s online or anywhere in the world people need a sense of security. This is especially important in the case of a home or someplace that needs to be kept private. My project is a lock system that requires two forms of ID in the form of an access card and pin code.

The system runs on two Atmega1284’s and a Raspberry Pi Zero W. The Pi drives an MC522 RFID Scanner while one Atmega controls an LCD Display, keypad and rgb LED, while the other runs an SG90 Servo motor to represent the lock.

Upon scanning a valid rfid tag the Raspberry Pi sends a signal to the Atmega1284 which then requests a pin code to be entered. If the code is correct the led will light up green and a signal will be sent to the other microcontroller to activate the servo and then return to the lock state after 3 seconds. If the code is incorrect the led will light up red and the system will revert to waiting for a tag.


## Hardware
### Parts List
The hardware that was used in this design is listed below. The equipment that was not taught in this course has been bolded. 

* ATMega1284 x 2
* Raspberry Pi Zero W
* 16x2 LCD Display
* 16 Key Membrane Switch Keypad
* **RFID Scanner**
* **Servo Motor**
* **RGB Led**


## Components
### Completed components
I was able to complete almost all of my components. 
I set up the rfid scanner to connect via spi to the raspberry pi and I connected that to the atmega using UART. 

The LCD and keypad were straightforward as I have used them in multiple projects so far and the servo was a challenge to setup but I got it done and enabled it using a pin change interrupt on the second atmega which receives a toggle from the first. I also included an unlock signal from an android smartphone using the RaspController app from the Google Play Store.

### Incomplete components
While I was able to complete most of my planned components, there were improvements I had to leave out such as an actual lock and full smartphone functionality. I initially planned on having the options to manage cards and passwords from a smartphone, I did not have the time to implement it and instead opted for a simple unlock signal.


## Links
### YouTube
[Demo Video](https://youtu.be/6YsKKifXEaM)
### Google Docs
For more information including code breakdown, diagrams, pinouts and testing, please checkout the full [report.](https://docs.google.com/document/d/1qlkAMn7JDkERGlkQ6wXC27hr7q1LPF6JiLnGWcGw1mo/edit?usp=sharing)
