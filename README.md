# SandVendor

## Maze

### Motor control
  - Physical Feeder position feedback
  - I2C protocol – master slave
    + Need to find a way to amplify signal over longer wires with this protocol.
    + P82B715 Ics should be great (1 at Master, 2 Feeder Slaves, 1 Door Slaves)
  - L298P have high voltage drop (2-3V)
    + very inefficient to drive the low voltage high current motors
    + overheating -> need heatsinks!
    + 3.6V with
  - 7V-6.2V-3.6V 1.7A (TOO MUCH!)
  - 5V-4.5V-2.4V 1.1A (bit too low)
  - Maybe step down current via PWM
    + maybe with current sensing feedback

### Power supply
  - 7V recommended as external input for Arduino
  - I2C needs common ground without extender ICs


### Feeder design -  several changes needed: 
  - Not sand proof! (Wear, stuff getting stuck)
    + The sand falls between the edge, clogs the ball bearings
    + Sand might get stuck between feeder disk and maze floor
  - Bottom of the tray with the cups would be damaged very soon (it is too thin to properly bear the weight). 
  - Removable cups!  
     + Scooping sand into all the sandwells at once is inefficient 
     + Hard to place into motor coupling again, might wear them down
     + Exposes underside to sand falling off
     + Much easier to clean
  - The gratings are quite large, as a result the sand will mix with the food crumbs below, when you tip the sand out the food crumbs will follow as well and will then need to be refilled again. (also right now the gratings can only be opened with a screw driver – when you have to replace the crumbs below that often then you need an easier way to open it). (or are we just putting entire 1 gram balls in there?). 
  - The motors are having trouble moving the weight, using a set up that provides some leverage could make the motors more efficient (otherwise we need to think about ordering different motors or motor shields). 
  - The current design is very heavy – could (should) make this lighter. 
  - Need to ensure that cups align exactly with hole in floorboard of maze. 
  - It would be best to incorporate into the design that the cup tray can only be placed onto the motor in such a way that the first cup aligns with the start position of the motor, you need some sort of reference for where the start position/position ‘0’ is. Also, considering multiple people (summer students, interns, marijn and I) will be carrying out the experiment this is safer and ensures consistency. 
  - Still need a way to attach the feeder to the maze (or floor)

### Feedback
  - Motor feedback
    + Absolute tray position
    + zero point calibration (simple button at end)
  - Digging detection
  - Animal position triggering

## Software
  - Write code for opening and closing doors, incorporate in Matlab code for trial. 
  - Incorporate command for stepper motors to turn in main Matlab code that runs the trial. 

## Doors
  - Basically complete, but need to test (are they strong enough, will the rats push them open/disrespect them?)
  - Control door Arduinos by Mastre

## Camera
  - Have to get Point Grey SDK to work (problem with 32bit sdk on 64bit system?) 
  - What exactly is it we want to achieve? (have the camera send pulses to the data acquisition system so that we have exact timing of frame to data). 
  - Install camera in room and get connection to computer there to work
  - Fallback LED - blinks at medium time scale random intervals, recorded on Ephys setup

## Computer
  - Currently isn’t working at all – need to figure out what’s wrong with it, or get another one. 
  - Have to get a working screen – or at least a small one for testing. 
  - Install Matlab and other necessary software

## Room
  - Need curtains around maze – find a way to hang these. 
