# stlink2

```
$ STLINK2_LOGLEVEL=trace STLINK2_LOGFILE=stdout stlink2 probe
[INFO]  (stlink2_probe) src/stlink2.c:473 : programmer found: 503f7206506752553329033f
Found 1 stlink2 programmer

  serial: 503f7206506752553329033f
      name: st-link/v2
[TRACE] (stlink2_usb_send_recv) src/usb.c:232 : USB > f1 80 00 00 00 00 00 00 00 00 00 00 00 00 00 00
[TRACE] (stlink2_usb_send_recv) src/usb.c:260 : USB < 26 c0 83 04 48 37
   version: V2J27S0
```
