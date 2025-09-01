# VegasCoop - Online Test Mod for Fallout: New Vegas


https://github.com/user-attachments/assets/4ddc8341-14fd-45b9-82ed-1efd39b49893


**Full Youtube Video: https://youtube.com/shorts/R6a3DWe0GQ0** 

# Description

This is a quick test mod, done in a weekend, whose goal was to replicate position and rotation of **two** Fallout New Vegas clients. It was made to get stated with modding and xNVSE.

It's based on the nvse_plugin_example and uses the [NECRO Socket Library](https://github.com/silvematt/NECRO-MMO/). It sets up a P2P TCPConnection to communicate, which shouldn't really be done as UDP is the correct choice for a project like this. Again, it's meant to be a quick test, as I had already written a TCP networking library and didn’t have much time.

The mod adds commands that call C++ code, but it uses an .esp to interface with the game, so there will be OBScript overhead.


I think sharing this is a good thing, it can serve as a starting point or reference for setting up a co-op mod.


# Some notes

I do believe you could get far into development with an OBScript based setup like this example, especially for a co-op 2 player mod. But it's probably one of those things that works until it doesnt, and there will probably be some instances where you would really like to avoid it. 

It would be a good idea to write it as a plugin mod completely in C++, avoiding OBScript, but that requires a lot of reverse-engineering and **a ton** of time to spend on.


It would be an incredibly good idea to spawn a separate thread to handle the connection, computations, packets handling, and related tasks in the background while exposing processed results for the main game thread to collect, on the kMessage_MainGameLoop handler. 

In the kMessage_MainGameLoop you'd access a "processedMessages" queue and apply the changes to the game.


# Usage

1) [Set the desired port for the host](https://github.com/silvematt/VegasCoop/blob/665c99974e314fc2e2ed27191a3675a35632f218/src/VegasCoopBlock.h#L35) (default is 61532)


2) [Set the host IP and outport](https://github.com/silvematt/VegasCoop/blob/665c99974e314fc2e2ed27191a3675a35632f218/src/VegasCoopBlock.h#L75)


3) Compile and install the .dll, install the PersonalNPC.esp as well and enable it.


4) Open the game on both machines (it can also be two instances on the same computer) and reach Goodsprings, where there will be an NPC standing there.


5) In game, on the host machine, open the console and run these cmds:
* VegasCoop_Init
* VegasCoop_SetupListener


6) In game, on the client machine, open the console and run these cmds:
* VegasCoop_Init
* VegasCoop_Connect


7) Again on the host machine, run:
* VegasCoop_AcceptConnection



And you should be set, positions and rotation in that worldspace will be synchronized. The other player will be represented by the NPC the .esp added.
