## Testing the printk log levels

Check the current log level

`cat /proc/sys/kernel/printk`

You can observe the output with four indicators as follows,representing current,default,min,max log levels.
`4    4     1     7`

Change the log level as 

`dmesg -n 6`

*or*

`echo "5" > /proc/sys/kernel/printk`

For each log level observe the behaviour when this module is loaded

You may not find any difference in `dmesg` output

In a pseudo terminal (under graphical view) you may not find any difference

When you try the loading of this module in a login console **(Eg:- Ctrl+Alt+F1 to Ctrl+Alt+F6)**, you can observe that printk 
output whose log level is less than the current log level will be immediately displayed to user without checking dmesg

Of course output of all log levels will be visible through `dmesg`.

Hit Ctrl+Alt+F7 to come back to default graphical view, it may be Ctrl+Alt+F8 or Ctrl+Alt+F1 in some machines
