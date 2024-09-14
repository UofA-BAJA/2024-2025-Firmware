The biggest flaw with the way I'm doing things right now is the fact that subsystems can be accessed by multiple
procedures at the same time. In theory this is nice, because if two procedures wanted to read the RPM they could,
but if there is a subsystem where you can set values, and multiple procedures are both trying to set values at once,
this could be a problem.

Maybe there could be a flag so that if the user wants, a subsystem can only be accessible by one procedure at a time
(the dash for example)