# FIR Sensor Monitoring
> Some software controlling the ESP-01/8266 board getting input from a FIR sensor and sending data to a google sheet. 

## ESP-01/8266
The software programmed on the ESP board reads the FIR sensor's output on the GPIO2 pin of the board and calls a Google Spreadsheet Script URL to add a new line. 
The ESP is programmed through a CH340 board. 

## FIR Sensor
The sensor works in a range of 3 meters approximatively. It will emit a 1 if a movement is detected for 6-8 seconds and is then ready to detect a new movement. 

## Google Spreadsheet Script
Developped as web app, callable through its URL, followed by `?tag=MovementDetected&value=FIR`, will update the sheet it is pointing towards y adding a new line to it.  

# Reference
Everything is based on [this tutorial](http://embedded-lab.com/blog/post-data-google-sheets-using-esp8266/).
