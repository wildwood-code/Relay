# Relay
USB-HID Relay command line utility

This is a command line utility to control one or more USB HID relays.

It allows the user to set the state (open / closed) on one or more relays,
or to query the state of one or more relays, or to enumerate all of the USB-HID relays.

# Examples

List/enumerate all relay modules:
```
Relay.exe list
```

Query the state of a given relay relay module (addressed by its serial number):
```
Relay.exe query 6QMBS
```

Query the state of the first relay module (as obtained with enumerate):
```
Relay.exe query
```

Query the state of multiple relays:
```
Relay.exe query 6QMBS 5XARZ 6VXAT
```

Query the state of specific relays in modules:
```
Relay.exe query 6QMBS=1456 5XARZ=178
```

Turn all relays in the first module on:
```
Relay.exe set all=on
```

Turn on specific relays in a given module: (two equivalent methods)
```
Relay.exe set 6QMBS=011XX0
Relay.exe set 6QMBS 1=0 2=1 3=1 6=0
```

Turn on specific relays in multiple modules:
```
Relay.exe set 6QMBS=011XX0 5XARZ 1:1 5:1
```


Kerry S Martin, martin@wild-wood.net, wssm243@gmail.com
