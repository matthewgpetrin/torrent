# Torrent GNURadio Program:
## Overview:

This directory contains the GNURadio code and flowgraph for transmitting and receiving coordinates.

Make sure you set your own directories for the transmitted and received message (originalMessage.txt and demodMessage.txt)

Note that the Header/Payload Demux block causes some overflow issues in the current version. This problem has not yet been properly diagnosed.
  
## Files:

**FSK_SDR_Packet_TxRx.py**

Python file that is generated when the flowgraph is processed by GNU Radio Companion. Forgot to upload, but I will do it soon.

**originalMessage.txt**  

Contains the message you intend to transmit. For the packetized flowgraph, make sure to leave space after the message at least equal to the number of characters of the message + 1. In the provided example, we input the coordinates 3 times.   

**demodMessage.txt**

Contains the received message. In our test, we were able to receive the coordinates properly, but the formatting is still slightly off. The filk sink that this was linked with is set to overwrite, so don't worry about deleting what's already in this file.
