# prgmTRANSFER v0.0.1b

### This software is still in beta, no liability for corrupted or lost files, etc!

An MTP-based variable transfer program for the TI-84+ CE and TI-83 Premium CE calculators.
Running this program on the calculator will allow you to transfer variable files between a
Windows 10/Ubuntu 20.04/Android with preinstalled software, or other OSes with various
PTP or MTP transfer software.

## Known Working Computer OSes
- Windows 10 using the default file explorer, check under Computer after connecting.
- Ubuntu 20.04 using the default Gnome Files (nautilus) or Dolphin.
- Android 11 using the builtin Files application, check notifications after connecting to open.

## Installation
1. Send [TRANSFER.8xp release](https://github.com/jacobly0/transfer/releases/latest) and [nightly clibs.8xg from usbdrvce branch](https://jacobly.com/a/toolchain/usbdrvce/clibs.zip) to your calculator using other transfer software.
1. Run `Asm(prgmTRANSFER)` and then plug-and-play with a usb cable to supported OSes, or using supported software.
1. The screen should display a debug log that can be ignored unless things go wrong, in which case the last few lines should be reported if there is an issue.
1. Press `clear` to exit.

## Implemented MTP Features
- [x] Sending variable files from computer to calculator in either RAM or Archive or file default.
- [x] Receiving variable files from the calculator RAM or Archive to the computer.
- [x] Deleting calculator variables from RAM or Archive.
- [x] Moving variables directly between RAM and Archive.
- [ ] Sending or receiving calculator operating systems.
- [ ] Sending, receiving, or deleting calculator applications.
- [ ] Receiving a rom dump.
- [ ] Copying variables in either RAM or Archive to another name in either RAM or Archive.
- [x] Getting free space in RAM or Archive.
- [x] Geting and seting the current time.
- [x] Geting the current battery level.
- [ ] Geting a device icon for displaying in MTP program UIs.
- [ ] Optimize for a smaller program size.
