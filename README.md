# edison-oled-msg 

This small C++ program allows you to display stdout on your Intel Edison's Sparkfun OLED display.

## Why

I wanted an easy way to blast things to the Edison's screen. I'm using the Sparkfun boards with the 
tiny OLED display. 

I often can't get the Edison's address easily if it's bouncing around between
wifi access points. This helps.

## What it do

This program reads stdin and spits it out to the OLED display using the default font.

Use like: 

```
$ date | omsg
```

It will wait briefly for input on the D-pad or buttons, and if input/press is detected, it will output
to stdout a one-character code for which was pressed, so you can use this to drive other programs/logic.

## How to install

I'm using it with a script in `/etc/init.d`. I've included the script here (`initd-omsg-status`) and you
can read more about [how to setup `init.d` here](http://stephaniemoyerman.com/?p=41).