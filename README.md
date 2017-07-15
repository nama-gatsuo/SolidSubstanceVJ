# SolidSubstanceVJ
A VJ set using motifs like mass solid substances. It is made by openFrameworks and Max. The Max part is to process sound input and send OSC to oF app. An oF part is generating CG.

## oF CG outputs
...

## Max Controller
![max controller](./bin/maxcontroller.png)
### Audio processor
Attack detection: a user can get attacks of sound in specific frequency by using band pass filter. And then it sends OSC to another app of a specified port.
### Parameter sender
Simple controller: it sends parameters in OSC with simple address. A user can also use hardware MIDI controllers.

# Author
Ayumu Nagamatsu
