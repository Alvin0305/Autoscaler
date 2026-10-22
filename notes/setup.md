# Setup

## Environment Setup (Ubuntu)
```bash
sudo apt update

sudo apt install qemu-kvm qemu-system-x86 qemu-utils
sudo apt install libvirt-daemon-system libvirt-clients libvirt-dev
sudo apt install virt-manager 
sudo apt install bridge-utils
sudo apt install cpu-checker
sudo apt install libxml++2.6-dev
```
## Verification 

```bash
kvm-ok
virsh --version
qemu-system-x86_64 --version
libvirtd --version
```

### Sample Output
```bash
alvin@alvin-laptop:~/Codes/C++/AutoScaler$ kvm-ok
INFO: /dev/kvm exists
KVM acceleration can be used
alvin@alvin-laptop:~/Codes/C++/AutoScaler$ file /dev/kvm
/dev/kvm: character special (10/232)
alvin@alvin-laptop:~/Codes/C++/AutoScaler$ virsh --version
10.0.0
alvin@alvin-laptop:~/Codes/C++/AutoScaler$ qemu-system-x86_64 --version
QEMU emulator version 8.2.2 (Debian 1:8.2.2+ds-0ubuntu1.18)
Copyright (c) 2003-2023 Fabrice Bellard and the QEMU Project developers
alvin@alvin-laptop:~/Codes/C++/AutoScaler$ libvirtd --version
libvirtd (libvirt) 10.0.0
alvin@alvin-laptop:~/Codes/C++/AutoScaler$
```

## Enable libvirtd and give permission
```bash
sudo systemctl enable --now libvirtd
sudo usermod -aG libvirt $USER
sudo usermod -aG kvm $USER
```

> After giving the permission, we should be able to use `virsh` and `virt-manager` without `sudo`. If no, log out and log in again.
> Check this before continuing. run `virsh uri`. If it returns `qemu:///system`, then its fine. If it shows permission error while `sudo virsh uri` works fine, then a restart might fix. Else, ask gpt.
