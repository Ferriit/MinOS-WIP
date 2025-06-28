# MinOS

### MinOS is a Work-in-Progress OS that can capture keyboard input and display it on the screen using the Nordic keyboard layout. If you wish to change the layout you can go into the **keyb.c** file and change whatever keys you want to whatever layout you want.


## Features:

> Captures Keyboard input and displays it using VGA codes (including ctrl and win/command/meta but no capital letters)
> Displays a startup message from Minos Prime that cannot be removed because Minos Prime is superior to everyone else
> Clears the input when pressing enter


## How to build:

> Install the ELF compiler using the script *build-cross-i686-elf.sh* (note that you need to have make, tar and wget installed)
> Go into the *build/* directory and execute the script *compile.sh* while in the *build/* directory
> Use *qemu-system-i386 -cdrom ../dist/minOS.iso* to run the generated ISO in *dist/* (you don't have to create a disk image as the OS can't yet read/write to disks)


## Dependencies:
> Make
> Tar
> Wget
> Nasm
> Ld
> GCC
> Grub
> Xorriso


## Notes:
> Make is only necessary for installing the compiler
> This is a Work-in-Progress
> GRUB is most definitely overkill for and OS like this because the OS really only needs like 3MB I think while GRUB uses 27MB of the ISO. So it's probably going to get replaced with Limine
> Make sure both *build-croww-i686-elf.sh* and *compile.sh* have +x permissions. If you get a permission error just run `chmod +x [script that's making the error occur]`

