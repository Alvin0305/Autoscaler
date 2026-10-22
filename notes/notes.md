# Setup
- Follow the setup instructions in `setup.md`.

## VM Details
- We can use the host for the client. 
- We need only 2 VMs. One for the server and the other as the replica.
- The 2nd VM will start only when the first VM CPU usage exceeds a certain threshold.
- Based on the question, we should set the threshold in a way that, after spawning the 2nd VM, the CPU usage should goes below the threshold. 
- There should not be a need for spawning more than 1 replica.
- We can use Ubuntu Server for this
- Download it from `https://ubuntu.com/download/server` or run `wget https://releases.ubuntu.com/26.04.1/ubuntu-26.04.1-live-server-amd64.iso`
- We can give 1vCPU, 1GB RAM and ~10-15GB disk for each server.
- Open virt-manager from terminal or GUI.
- Click on Create a new virtual machine.
- Select the downloaded ISO file and follow the prompts to create the VM.
- Once the VM is created, start it and follow the prompts to install Ubuntu Server.
- Give the following values:
> Your name: <your name>
> Your servers name: server1
> Pick a username: server1
> Choose a password: <password>
