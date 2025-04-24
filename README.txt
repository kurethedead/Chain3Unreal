This is a plugin that provides a game subsystem to interface with the CHAIN 3 door / persistent shared data systems. Add this to your project's Plugins/ folder (see how to install an Unreal plugin for more details).

Persistent data is stored in <MyProject>/StreamingAssets/. The saved files are:

    - enter.door: Contains a string indicating which room you entered from (ex. M1)
    - exit.door: Contains a string indicating which room you are exiting to (ex. K1)
    - shareddata.data: Contains list of strings (separated by newlines)
        indicating flags that keep track of shared game state.
        The convention is to prefix flags by the game the flag originates from.
        (ex. K1_unlocked_green)

The CHAIN launcher will write the enter.door and shareddata.data files before your game starts,
and read exit.door and shareddata.data after your game ends.
This plugin provides an interface to interact with this system.

To use, look for Get Chain3DataSubsystem in blueprint search to access the subsystem. You can then call its functions (located in the "Chain Data" category) to modify/query the persistent state.