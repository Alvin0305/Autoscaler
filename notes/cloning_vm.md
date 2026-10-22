# Cloning a VM

- We dont want to create each VM from scratch and install Ubuntu on each one whenever we need to scale up
- So, we will clone a base VM.
- Each VM will have a configuration file and a disk image.
- The disk image will be present in the folder `/var/lib/libvirt/images/`
- The image file will be having an extension `qcow2`

> For example, we can check the disk image information using the commad:
> `sudo qemu-img info /var/lib/libvirt/images/autoscaler-server1.qcow2`

# Creating the Base VM
- So we will be creating a base VM which will contain configurations which are common.
- Create a VM with name `autoscaler-base`
- Start it, follow the installation process, and install Ubuntu on it.
- In the profile configuration page:
  - Your name: alvin
  - Server Name: autoscaler-base
  - Hostname: autoscaler
  - Password: password
- Check ip address and route using:
  - `ip addr`
  - `ip route`
- Make sure `192.168.122.1` is the default gateway.
- Once installation is complete, reboot and check connectivity. Run the following commands in the VM
  - `ping -c 3 192.168.122.1`
  - `ping -c 3 8.8.8.8`
- Check whether its possible to ping the VM from the host machine.
- Update the packages using:
  - `sudo apt update`
  - `sudo apt upgrade`
- Install the packages needed for communication between the VM and the host machine.
  - `sudo apt install qemu-guest-agent`
- Enable it using systemctl
  - `sudo systemctl enable qemu-guest-agent`
  - `sudo systemctl start qemu-guest-agent`
- Check the status
  - `sudo systemctl status qemu-guest-agent`
- It should be active and running.
- Install required development packages
  - `sudo apt install build-essential cmake git curl wget`
- Install monitoring and networking tools
  - `sudo apt install procps htop iproute2 dnsutils`
- Check whether SSH is installed  
  - `systemctl status ssh`
- Check whether all the depdencencies are installed using `--version` flag for each installed dependcney

- Test the QEMU guest agent in the VM from the host. Run the following in the host machine:
  - `virsh domifaddr autoscaler-base --source agent`
- Try sshing to the VM from the host machine:
  - `ssh autoscaler@192.168.122.111`

- Create a socket program in the VM and run it
- Add it to systemd so that it will start on booting up after the network is setup
- Use the following toml file as `/etc/systemd/system/autoscaler-server.service` file
```toml
[Unit]
Description=Autoscaler HTTP Server
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
User=autoscaler
ExecStart=/opt/autoscaler-server/autoscaler-server
Restart=on-failure
RestartSec=2

[Install]
WantedBy=multi-user.target
```

# Cloning Process
```bash
sudo virt-clone --original autoscaler-base --name autoscaler-server1 --auto-clone
```