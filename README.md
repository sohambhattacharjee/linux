# CMPE-281 Assignment 1 
#### [Soham Bhattacharjee](mailto:soham.bhattacharjee@sjsu.edu)
##### These steps can be followed to complete the assignment on a mac, running intel CPU. This will be using a VM to run another VM (nested virtualization).
### Set up VM
Download [VMWare Fusion]( https://customerconnect.vmware.com/en/downloads/info/slug/desktop_end_user_computing/vmware_fusion/12_0) and install the software on your machine. You’d need to register at vmware to get a non-commercial use key or use the software in trial mode for 30 days.

Download Ubuntu 64 bit VMWare image from [OSBoxes.org]( https://www.osboxes.org/ubuntu/#ubuntu-21-04-vmware). Note that the image should be for VMWare.

In the “Processors and Memory” section of the virtual machine settings, increase the processor core (more the better) and increase the memory (more the better). Click on the “Advanced Options”, select “Enable Hypervisor Applications in this Virtual Machine” – this enables the nested virtualization option.

 Start the VM. You can find the password from the OSBoxes.org page, under the “Info” tab.

Once Ubuntu is running, open the terminal. To ensure that nested virtualization is active, run the command `cat /proc/cpuinfo`. Check the output of this file, if it contains `vmx flags:` then the setup has been successful and the VM has hardware assisted virtualization capabilities.

### Cloning Linux
Fork the Linux open source [codebase](https://github.com/torvalds/linux) on GitHub, into your own repo, (for example – [this fork](https://github.com/sohambhattacharjee/linux))

Run `ssh-keygen -t rsa` to generate a new key in the VM terminal. Copy this Key into GitHub settings -> SSH and GPG Keys, to allow cloning from GitHub into the VM.

Install git on the ubuntu machine by running `sudo apt install git`
Then clone the forked repo into a directory of the VM.
`mkdir cmpe283`
`cd cmpe283`
`git clone git@github.com:sohambhattacharjee/linux.git`

### Missing modules
We’ve already installed git in the previous step. In order to complete the rest of the assignment, we need to install a few more modules:
`sudo apt install make`
`sudo apt-get install gcc`
`sudo apt-get install flex`
`sudo apt-get install bison`
`sudo apt-get install libssl-dev`
`sudo apt install libelf-dev`
`sudo apt install dwarves`

Copy over the `/boot/config.X.Y.Z` file as `.config` file in the `linux` directory.
In the .config file, find and comment out the line
`CONFIG_SYSTEM_TRUSTED_KEYS="debian/canonical-certs.pem"`
Then run `make oldconfig`. Accept the defaults for all options.

### Build the linux kernel
`cd linux`
`make -j 4 modules`
`make -j 4`

Since we commented out the trusted key, the make process will create a new key during build.

To create a new installer, run
`sudo make INSTALL_MOD_STRIP=1 modules_install`

To install that kernel version, run
`sudo make install`

This updates the kernel version running on the VM. Reboot the VM for the update to take effect.

### Generating Kernel Object file
1.	Build the custom code
    `cd ..`
    `make`
2.	Step 1 generates a .ko file (kernel object file). Add this to the kernel by running
    `sudo insmod cmpe283-1.ko`
This prints all the output in the message buffer. To read the buffer, run `sudo dmesg`. The output from the assignment will be printed towards the end.

### Cleanup
`sudo rmmod cmpe283-1` would remove the module from kernel. Once the kernel has been built, it doesn't need to be rebuilt again. instead, only building the module and installing it would suffice.


