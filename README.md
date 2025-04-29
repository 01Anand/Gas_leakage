# Gas_leakage


Testing the Bluetooth Communication (Sending the "CALL" Command):

# Pair Your Bluetooth Module:

Make sure your Bluetooth module (e.g., HC-05) is properly connected to the Arduino (power, ground, RX to TX, TX to RX).
Use your smartphone's Bluetooth settings to scan for and pair with your Bluetooth module. The module will likely have a default name (like HC-05) and a default PIN (often 1234 or 0000).

# Use a Bluetooth Serial Terminal App:

Download a Bluetooth serial terminal application on your smartphone (search for "Bluetooth Serial Terminal" on the app store).   
Connect to your paired Bluetooth module from within the terminal app.
Repeat the gas leak simulation (or threshold adjustment) as in step 2 of the Arduino testing.
Observe the Bluetooth serial terminal app on your phone. When a gas leak is detected, you should see the messages being sent from the Arduino, including:
"WARNING: Gas Leakage Detected!"
"CALL_EMERGENCY" (or whatever command you defined).
