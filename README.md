# OveRGBkill

![Render](Renders/OveRGBkill.png)

*OveRGBkill* is an expandable analog RGB video switcher using common components.

## Features

* 5, 10, or 15 RGB inputs
* Standard Megadrive (MD2) style Mini-DIN 9 connectors
* Sense and automatically switch to active input
* Uses composite for sync
* Routes R, G, B, composite/sync, stereo audio L, R, ground, and +5V. Other pins are not connected.

Pairs well with a [RGB-Component transcoder](https://github.com/partlyhuman/rgb-yuv-transcoder) if you have a TV with component input.

## Integrations

Some notes on how to wire your consoles for RGB output, from my experience. A good starting point is [RetroRGB](https://www.retrorgb.com/systems.html).

### RGB-capable systems

For systems that are already RGB capable but don't have an MD2 connector, such as MegaDrive/Genesis, Saturn, SNES, N64, Playstation 1/2, arcade superguns, etc., I'll make an MD2 connector out of a SCART connector:

* Purchase an RGB-capable SCART connector
* Splice off the SCART head
* Disassemble the SCART head and use a SCART pinout to note which colour wire has which purpose. Many I've bought from AliExpress have this pinout *but you must verify for yourself*
	* Yellow — Composite
	* Orange — Red
	* Green — Green
	* Blue — Blue
	* Brown — +5V
	* Purple — GND
	* Black — Audio GND
	* White — Audio Left
	* Red — Audio Right
* Use a Mini DIN-9 breakout PCB with a female Mini DIN-9 socket
	* For simple applications, something like [MobiusStripTech's breakout](https://oshpark.com/shared_projects/amKj4zix) is great
	* To include AC coupling capacitors, voltage dividers and/or mono expansion, use the [breakout PCB](pcb/md2-rgb/) in this repository!
* A 3d printable shell finishes the cable. On one end you have a console-specific connector, on the other end you have a female MD2 connector, and use a straight Mini DIN-9 cable to wire this to your switcher.

### SNES/SFC

Needs 220μF coupling capacitors on RGBC, use the  [breakout PCB](pcb/md2-rgb/) in this repository

### PS1/PS2

Needs 220μF coupling capacitors on RGBC, use the  [breakout PCB](pcb/md2-rgb/) in this repository

### Sega Genesis/Megadrive

Seems to use a much higher voltage, incorporate a voltage divider of around 1:2 on RGBC, using the breakout PCB or the customizable daughterboard.

### Arcade superguns

Many of these use 5V TTL CSYNC, incorporate a voltage divider of around 1:4  on C using the breakout PCB or the customizable daughterboard.

### Others

In fact it probably never hurts to add 220μF coupling capacitors on all inputs. This could be incorporated into future revisions of the daughterboard.

In general, if you have RGB modded your system, well designed mods will already use the correct voltages, AC coupling, and impedances, and I found that my RGB-modded PC Engine (doujindance) and NESRGB needed no further modification.

## Build considerations

### Daughterboard choice

Use the simple daughterboard if your inputs are standardized: if they're all AC-coupled (0 DC bias), with 75Ω impedance, and 0.7V peak-peak at 100 IRE.

You may use the customizable daughterboard to adjust voltages with voltage dividers built into the board. Or it may be more convenient to build these into your cables. It's your choice.

Of course you can also mix and match.

### Terimation

The TI 74HC4067 muxer has On-On resistance of around 230Ω. If you use this with a source designed for 75Ω termination, this means that there will be 75+230=305Ω resistance inline, and 75Ω termination. This forms a voltage divider that will make the image much more dim (normally the 75Ω inline and 75Ω terminating resistors form a 1:2 divider).

If you have control over the termination, for example by using any buffered video device between the switcher and your TV, you can design around this. For example, if using a RGB-Component transcoder, replace the 75Ω terminating resistors with ~300Ω resistors.

If you don't need to use a transcoder, you could instead use a simple THS7374 circuit with 300Ω termination.

**Note** a buffer could be integrated into a future revision of the hardware so that the switcher has typical 75Ω impedance.

### Cables

In the MD2 pinout, ground is carried over the shielding of a cable. Therefore, all mini-DIN 9 cables used with this must be shielded. Use a multimeter to check continuity between the metal outer ring of each end of the cable to verify you have shielded cables.

I can verify that (at the time I purchased them) these cables from "wg cable store" are shielded: [AliExpress](https://www.aliexpress.com/item/1005004607170871.html)
