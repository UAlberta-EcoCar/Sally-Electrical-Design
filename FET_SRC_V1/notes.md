# TO-DO For Code 
- Set up state machine to receieve vehicle state from fuel cell controller
    - alternatively, we could have the state machine run on here since this is where the important data lies
    - set up gpio and test MOSFET drivers (need to test power supplies for this)
- Set up all data can id's and code to push onto the bus constantly
- Set up ADCs to record voltages and currents
- Handle incoming CAN messages (we could brute force and filter in code but we should abuse the filters were possible)


