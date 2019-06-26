## Linux driver for Asus Wireless Keyboard + Mouse Combo

This is a Linux driver hack to make the keyboard work for a Asus wireless keyboard + mouse combo (specifically for Asus W2500) manufactured by Primax. The keyboard problems experienced (keyboard works in BIOS, only a few special keys work in Linux) were very similar to problems reported by many many users for Asus wireless keyboard + mouse combos also manufactured by Primax, but the Lenovo driver hack, as well as HP driver hack, that this driver hack is a fork of did not work for the Asus keyboard.

### Compatibility
So far, this driver has been tested successfully with the following models:

* Asus W2500 Wireless Keyboard (Primax)

However, I have seen several reports of keyboards of other brands with similar problems, so it is possible that this fix could work for them. Feel free to test and please tell me if it worked or not.

Linux: Mint 19.1 Cinnamon, Ubuntu 18.10  
Kernel: 4.15, 4.18

### Usage
Clone or download and install with:

```bash
./install.sh
```

and uninstall with:

```bash
./uninstall.sh
```

### Known issues
* Other keyboard connected at the same time freezes the system for a second on every keypress. Uninstalling the driver fixes the issue.

If you find other issues or know how to fix something, please tell me.
