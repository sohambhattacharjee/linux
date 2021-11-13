# CMPE-283 - Assignments 1, 2, 3
#### [Soham Bhattacharjee](mailto:soham.bhattacharjee@sjsu.edu) (Student ID: 015443818)
#### [Rajat Banerjee](mailto:rajat.banerjee@sjsu.edu) (Student ID: 015920931)
<details>
 <summary>Assignment 1</summary>
 
  ### Question - 1 : Breakup of tasks

  ## Soham
  - Building the linux kernel
  - RnD into Certificate generation issue : 
   [Stackexchange](https://unix.stackexchange.com/questions/293642/attempting-to-compile-kernel-yields-a-certification-error/649484#649484)
  - Installing the kernel
  - Modifying the custom functions and running the code
  - Generating the Kernel object

  ## Rajat
  - Virtualization Software and how to get the free version
  - Online repository to get free linux distrubution
  - Setting up the VM with Vmware Fusion
  - Setting up the missing modules needs for the build
  - Git setup & Github ssh keys setup
  - Code setup

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
  - `mkdir cmpe283`
  - `cd cmpe283`
  - `git clone git@github.com:sohambhattacharjee/linux.git`

  ### Missing modules
  We’ve already installed git in the previous step. In order to complete the rest of the assignment, we need to install a few more modules:
  - `sudo apt install make`
  - `sudo apt-get install gcc`
  - `sudo apt-get install flex`
  - `sudo apt-get install bison`
  - `sudo apt-get install libssl-dev`
  - `sudo apt install libelf-dev`
  - `sudo apt install dwarves`

  Copy over the `/boot/config.X.Y.Z` file as `.config` file in the `linux` directory.
  In the .config file, find and comment out the line
  `CONFIG_SYSTEM_TRUSTED_KEYS="debian/canonical-certs.pem"`
  Then run `make oldconfig`. Accept the defaults for all options.

  ### Build the linux kernel
  - `cd linux`
  - `make -j 4 modules`
  - `make -j 4`

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
  <img width="692" alt="Screen Shot 2021-10-29 at 1 35 41 AM" src="https://user-images.githubusercontent.com/24656792/139404426-643e3910-8262-407c-b6aa-8d9c97f4d85e.png">
  <img width="740" alt="Screen Shot 2021-10-29 at 1 36 48 AM" src="https://user-images.githubusercontent.com/24656792/139404431-ce3058e8-bbe4-48d1-8919-8b8b748b1ea9.png">


  ### Cleanup
  `sudo rmmod cmpe283-1` would remove the module from kernel. Once the kernel has been built, it doesn't need to be rebuilt again. instead, only building the module and installing it would suffice.
</details>

<details>
 <summary>Assignment 2</summary>

### Question - 1 : Breakup of tasks

## Soham
- Setting up nested virtual machines
- Building linux kernel
- Code changes on cpuid.c, fixing run time issues
- Coming up with unit tests (not checked in)

## Rajat
- Buildinng linux kernel, fix build time issues
- Researching about atomic variables and their proper usage
- Code changes on vmx.c
- Debugging issues during execution

### Question 2 : Handling leaf 0x4FFFFFFF and 0x4FFFFFFE


As part of this assingment, the linux kernel would be updated (CPUID) to handle the following leaf nodes (%eax):
- 0x4FFFFFFF: return the total number of interrupts handled by the hypervisor in %eax.
- 0x4FFFFFFE: return the total cpu cycles spent in handling interrupts, with the high bits of the 64 bit number in %ebx and lo bits in %ecx

The code changes have been made in arch/x86/kvm/cpuid.c and arch/x86/kvm/vmx/vmx.c 

 ### Build the linux kernel
  - `cd linux`
  - `make -j 4 modules`
  - `make -j 4`

  Since we commented out the trusted key, the make process will create a new key during build.

  To create a new installer, run
  `sudo make INSTALL_MOD_STRIP=1 modules_install`

  To install that kernel version, run
  `sudo make install`

  This updates the kernel version running on the VM. Reboot the VM for the update to take effect.


### Test the code changes
#### Prepare VM for nested virtualiztion
- `sudo apt install virtinst`
- `sudo apt install libvirt-clients`
- `sudo apt install virt-top`
- `sudo apt install qemu-kvm`
- `sudo apt install libvirt-daemon`
- `sudo apt install libvirt-daemon-systems`
- `sudo apt install bridge-utils`
- `sudo apt install virt-manager`

Launch virt-manager and step through the wizard to add a new Ubuntu os as nested VM.

Once the setup is done, install `sudo apt install cpuid` on the inner VM. 
once that has been installed, run `cpuid -l 0x4fffffff` to test question 1 and `cpuid -l 0xffffffe` to test question 2.
Expected output should be something like this:
![Assignment 2](https://user-images.githubusercontent.com/24656792/140996140-59acac24-aa92-441b-8158-dfff41d19ab6.PNG)
</details>

<details>
  <summary>Assignment 3</summary>

  ### Question - 1 : Breakup of tasks

  #### Soham
  - Building linux kernel
  - Code changes on cpuid.c, fixing run time issues
  - Coming up with unit tests (not checked in)
  - Printing all data from counters to dmesg
  - handling error conditions

  #### Rajat
  - Buildinng linux kernel, fix build time issues
  - Code changes on vmx.c, including getting int index from exit reason
  - Debugging issues during execution
  - clearinng registers for output

  ### Question 2 : Handling leaf 0x4FFFFFFD and 0x4FFFFFFC
  - For CPUID leaf node %eax=0x4FFFFFFD:
    - Return the number of exits for the exit number provided (on input) in %ecx
    - This value should be returned in %eax 
  - For CPUID leaf node %eax=0x4FFFFFFC:
    - Return the time spent processing the exit number provided (on input) in %ecx
    - Return the high 32 bits of the total time spent for that exit in %ebx
    - Return the low 32 bits of the total time spent for that exit in %ecx
  
  ##### This assignment builds on top of Assignment 2 above. Setting the nested VM is defined there.
  The code changes have been made in arch/x86/kvm/cpuid.c and arch/x86/kvm/vmx/vmx.c 

  #### Build the linux kernel
    - `cd linux`
    - `make -j 4 modules`
    - `make -j 4`

    Since we commented out the trusted key, the make process will create a new key during build.

    To create a new installer, run
    `sudo make INSTALL_MOD_STRIP=1 modules_install`

    To install that kernel version, run
    `sudo make install`

    This updates the kernel version running on the VM. Reboot the VM for the update to take effect.

  #### Testing the code changes
  From within the nested VM, run `cpuid -l 0x4FFFFFFD -s 0` to get the information about how many times exit reason `0` was handled.

  On the outer VM, run `dmesg` to get a dump of all the exit reasons and the number of times they were handled.
 ![Exit counter](https://user-images.githubusercontent.com/24656792/141657428-b771b871-3c78-4df0-bd88-536f9137208e.PNG)

  To test CPU cycles, on the nested VM, run `cpuid -l 0x4FFFFFFC -s 0` to get the information about how many cpu cycles were spent for handling exit reason `0`. Change the exit reason to get information about other exit reason codes.

  On the outer VM, run `dmesg` again to dump all the information about exit reasons and corresponding cpu cycles.
![CPU Cycles](https://user-images.githubusercontent.com/24656792/141657431-77044d45-8bc8-4d98-bf22-99be4cddf84f.PNG)

  ### Question 3 - Comment on the frequency of exits – does the number of exits increase at a stable rate? Or are there more exits performed during certain VM operations? 

  No, not all exits have the same growth rate. For instance, some exits reasons rarely occur. Like exit code 50 - 69 were all exited 0 times.
  Exit reason 1 (external interrupt) gows very fast if we do a `CURL` command from within the nexted VM. For example, it grew from 129620 to 130593 between a `curl www.google.com` command in nested VM.
  
  #### Approximately how many exits does a full VM boot entail?

  For a `sudo reboot` command run on the nessted VM, total exit count before - 25a8e3 (2468067), after - 368bae (3574702), which is 1.1 million exits.

  ### Question 4 - Of the exit types defined in the SDM, which are the most frequent? Least?
  Exit reasons 1 (External interrupt), 10 (CPUID), 30 (I/O Instructions), 32 (WMR), 48 (EPT Violation) happen very frequently.
  (Not counting the exits that never occurred) Exit reasons 29 (MOV DR), 46 (Access to GDTR or IDTR) happened only 2 times.


</details>
