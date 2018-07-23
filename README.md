==Proc Grab==

A basic tool to create snap shots of linux process (externally, not as a snapshot library) from the user space and restore the process later. 

This tool works in two parts -

 1. grab - Grabs (create a snap shot) of an already existing process into a dump file. 
 2. release - Spawns a new process from a dump file

Support for features like fd (some class of fds) restore are part of the initial plan. 
Some features like sockets etc depend on the environment and are not in scope of the first version. 

