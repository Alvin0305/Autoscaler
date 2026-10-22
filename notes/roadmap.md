16. What should the project architecture look like?

I'd structure it approximately like this:
```text
cloud-autoscaler/
│
├── CMakeLists.txt
├── README.md
│
├── include/
│   ├── libvirt/
│   ├── autoscaler/
│   ├── monitor/
│   ├── server/
│   ├── client/
│   └── common/
│
├── src/
│   ├── autoscaler/
│   │   ├── Autoscaler.cpp
│   │   ├── VMManager.cpp
│   │   └── Policy.cpp
│   │
│   ├── monitor/
│   │   ├── CPUMonitor.cpp
│   │   └── Metrics.cpp
│   │
│   ├── server/
│   │   └── Server.cpp
│   │
│   ├── client/
│   │   ├── Client.cpp
│   │   └── LoadGenerator.cpp
│   │
│   └── main.cpp
│
├── config/
│   └── config.json
│
├── vm/
│   └── server-template.xml
│
└── scripts/
    ├── setup-vm.sh
    └── setup-server.sh
```
We don't need to build all of this immediately.

17. Break the project into phases

This is the important part.

Don't jump directly to:

"Let's write the autoscaler."

We'll build it in stages.

Phase 0 — Virtualization environment

Goal:
```text
Kubuntu
   │
   └── libvirt
          │
          ├── server1
          └── server2
```
Learn:

KVM
QEMU
libvirt
virt-manager
virsh
VM XML
libvirt networking

At the end you should be able to:

virsh list --all

and see:

server1
server2
Phase 1 — Understand VM XML

This is specifically required by the assignment.

Run:

virsh dumpxml server1

You'll see things like:

<domain type='kvm'>
    <name>server1</name>

    <memory unit='KiB'>1048576</memory>

    <vcpu>1</vcpu>

    <disk>
        ...
    </disk>

    <interface>
        ...
    </interface>
</domain>

You need to understand:

<name>
<memory>
<vcpu>
<disk>
<interface>
<source>
<target>
<devices>

The XML becomes important later because the autoscaler needs to create a new VM.

The assignment specifically suggests using XML configurations to create new VMs.

Phase 2 — Build the server

Now ignore autoscaling.

Build a simple server.

I'd recommend a CPU-intensive computation server, because the assignment explicitly says CPU utilization is what we're monitoring and therefore a CPU-intensive workload is preferable.

For example:
```text
Client
   │
   │ TCP
   ▼
Server
   │
   ├── receive request
   │
   ├── compute
   │
   └── send result
```
Request:

COMPUTE 50000000

Server performs some CPU-heavy operation.

Could be:

prime calculation
matrix multiplication
hash computation
Fibonacci
Monte Carlo
etc.

I'd probably use a configurable CPU workload rather than something artificial like Fibonacci.

19. Make the server multithreaded

The assignment explicitly expects the server to handle requests independently and be horizontally scalable.

So:
```text
Server
│
├── accept()
│
├── client 1 → thread
├── client 2 → thread
├── client 3 → thread
└── client N → thread
```
For example:

while (true) {
    int client = accept(...);

    std::thread(handleClient, client).detach();
}

We'll make this much cleaner when we implement it.

20. Phase 3 — Build the client/load generator

Now create:

Client

which can generate:

Low load
10 requests/sec
High load
50 requests/sec

or whatever values actually saturate your VM.

The assignment specifically requires a variable traffic generator capable of low and high load modes.

The client should also support multiple concurrent requests.

Something like:

              ┌──► Server 1
Client ───────┤
              └──► Server 2

Initially only:

Client → Server 1

After scaling:

Client → Server 1
       → Server 2
21. Phase 4 — Build CPU monitoring

Now we start using libvirt.

Your program connects to:

qemu:///system

Conceptually:

virConnectPtr connection =
    virConnectOpen("qemu:///system");

Then:

connection
    │
    ▼
virDomainPtr
    │
    ▼
server1

You'll obtain VM CPU statistics through libvirt.

This is one of the central parts of the assignment. The provided instructions specifically tell you to obtain a virConnectPtr, then a virDomainPtr, and use the domain/host APIs for resource monitoring.

22. Important: CPU utilization isn't just a number

You'll need to understand this carefully.

Suppose you obtain:

CPU time at t1 = 20 seconds
CPU time at t2 = 25 seconds

over:

5 seconds

The VM consumed:

5 CPU-seconds

during 5 wall-clock seconds.

For a one-vCPU VM:

CPU utilization ≈ 100%

This is why you don't simply ask libvirt:

"What's CPU percentage?"

You generally calculate utilization from CPU time differences.

And the assignment even warns about discrepancies between CPU usage reported inside the VM and usage measured by the monitoring program.

That will be a good viva question.

23. Phase 5 — Autoscaling policy

Now introduce a policy.

For example:

CPU > 80%
for 5 consecutive measurements
        │
        ▼
     OVERLOAD

rather than:

CPU > 80%
once

because one temporary spike shouldn't create a VM.

The assignment explicitly recommends avoiding false positives from transient CPU spikes.

A good initial configuration might be:

sampling interval = 1 second
threshold = 80%
required consecutive samples = 5

We'll tune these experimentally.

24. Phase 6 — Spawn Server 2

This is the fun part.

Autoscaler detects:

server1 CPU
████████████████████ 95%

Then:

Autoscaler
     │
     │ libvirt
     ▼
create/define VM
     │
     ▼
server2

The assignment explicitly permits approaches involving the create domain and define XML APIs for spawning VMs.

25. Automatically start the server application

There's another important requirement that's easy to miss.

When server2 boots:

VM boot
  ↓
Linux startup
  ↓
server application starts

The assignment says the server application should start as soon as the VM boots, and suggests mechanisms such as rc.local, .profile, or init scripts.

I'd actually use a systemd service rather than the older mechanisms.

That gives:

server.service
     │
     ├── starts automatically
     ├── restarts on failure
     └── runs in background

Much cleaner.

26. Phase 7 — Notify the client

Once server2 is ready:

Autoscaler
    │
    │ "Server 2 available"
    ▼
Client

The assignment intentionally leaves the notification mechanism up to you.

We have several choices:

Option A — TCP control socket
Autoscaler → Client
             "ADD 192.168.122.102"
Option B — shared configuration

Client periodically asks:

GET SERVERS
Option C — Unix socket

Since autoscaler/client can run on the host:

/tmp/autoscaler.sock

For your project, I'd use a small TCP control channel because it gives you another useful networking component.

27. Phase 8 — Client starts using Server 2

Initially:

servers = {
    server1
}

After notification:

servers = {
    server1,
    server2
}

Then use:

round-robin

for request distribution.

For example:

Request 1 → server1
Request 2 → server2
Request 3 → server1
Request 4 → server2
...

This is simple and perfectly adequate for the assignment.

28. Phase 9 — Demo/metrics

This is where you make the project look polished.

Record:

timestamp
server1 CPU
server2 CPU
requests/sec
average latency

Then produce something like:

CPU
100% |              ████
     |              ████
 80% |         █████████
     |         █████████
 60% |         █████
     |              ████
 40% |              ████
     |              ████
 20% |
     +---------------------------->
                 time

                  ↑
             Server 2 spawned

The assignment explicitly suggests plotting CPU utilization and/or throughput/latency before and after the scaling event.

29. The complete architecture

Eventually:

                           KUBUNTU
┌───────────────────────────────────────────────────────┐
│                                                       │
│                    ┌─────────────┐                    │
│                    │ Autoscaler  │                    │
│                    └──────┬──────┘                    │
│                           │                            │
│                      libvirt API                       │
│                           │                            │
│                           ▼                            │
│                  ┌─────────────────┐                   │
│                  │  KVM / QEMU     │                   │
│                  └───────┬─────────┘                   │
│                          │                             │
│             ┌────────────┴────────────┐               │
│             │                         │               │
│             ▼                         ▼               │
│      ┌──────────────┐          ┌──────────────┐      │
│      │   Server 1   │          │   Server 2   │      │
│      │     VM       │          │     VM       │      │
│      │              │          │              │      │
│      │ 1 CPU        │          │ 1 CPU        │      │
│      │ 1 GB RAM     │          │ 1 GB RAM     │      │
│      └──────┬───────┘          └──────┬───────┘      │
│             │                         │              │
│             └────────────┬────────────┘              │
│                          │                            │
│                    libvirt network                    │
│                          │                            │
│                          ▲                            │
│                          │                            │
│                     ┌────┴─────┐                      │
│                     │  Client  │                      │
│                     │  / Load  │                      │
│                     │ Generator │                     │
│                     └───────────┘                     │
│                                                       │
└───────────────────────────────────────────────────────┘
30. What you should learn before coding

I'd divide the prerequisites into four levels.

Level 1 — Linux

You should be comfortable with:

systemd
processes
signals
permissions
network interfaces
IP addresses
TCP sockets
SSH
shell

You already have quite a bit of this from your networking work.

Level 2 — Virtualization

Learn:

KVM
QEMU
libvirt
virt-manager
virsh
VM lifecycle
VM XML
virtual NIC
NAT
virbr0
Level 3 — libvirt API

This is the most important prerequisite.

Understand:

virConnectPtr
virDomainPtr
virConnectOpen()
virConnectClose()

virDomainLookupByName()

virDomainCreate()
virDomainCreateXML()

virDomainGetInfo()
virDomainGetCPUStats()

virDomainShutdown()
virDomainDestroy()

virDomainFree()

Not necessarily every API in libvirt — just the domain lifecycle and monitoring pieces relevant to this project.

Level 4 — C++ systems programming

You'll need:

TCP sockets
std::thread
mutex
condition_variable
chrono
RAII
smart pointers
error handling
CMake

And importantly, C APIs from C++.

libvirt itself is a C API, so you'll encounter things like:

virDomainPtr

and manually managed resources.

We can wrap these using RAII so the final code stays clean.

31. What I would NOT install

Don't overcomplicate this.

You don't need:

Docker
Kubernetes
OpenStack
Minikube
AWS
Azure
Terraform
Ansible

for the assignment.

This is specifically about:

libvirt
+
KVM
+
QEMU
+
VMs
+
autoscaling

Adding Kubernetes/OpenStack would actually distract from what you're being evaluated on.

32. One important hardware consideration

You have 16 GB RAM, so I'd keep the environment conservative.

Two VMs:

Server 1 → 1 GB
Server 2 → 1 GB

plus:

Kubuntu
libvirt
QEMU
your C++ programs

is completely reasonable.

The assignment itself only requires two VMs at minimum.

Don't start with:

4 VMs × 4 GB

just because you can. 😄

33. The development order I'd use

This is the exact order I'd recommend for us:

                    PROJECT ROADMAP

PHASE 0
│
├── Install KVM/QEMU/libvirt
├── Install virt-manager
├── Verify /dev/kvm
└── Verify libvirt
        │
        ▼
PHASE 1
│
├── Create server1
├── Create server2
├── Configure networking
├── Static IPs
└── Understand VM XML
        │
        ▼
PHASE 2
│
├── C++ TCP server
├── CPU-intensive workload
└── Multithreading
        │
        ▼
PHASE 3
│
├── C++ client
├── Concurrent requests
├── Low-load mode
└── High-load mode
        │
        ▼
PHASE 4
│
├── libvirt connection
├── Find VM
├── Read CPU statistics
└── Calculate CPU %
        │
        ▼
PHASE 5
│
├── Monitoring loop
├── Threshold
├── Consecutive samples
└── Overload detection
        │
        ▼
PHASE 6
│
├── Clone/create server2
├── Start VM
└── Wait for server
        │
        ▼
PHASE 7
│
├── Notify client
├── Add server2
└── Round-robin requests
        │
        ▼
PHASE 8
│
├── Metrics
├── CPU graph
├── latency
└── throughput
        │
        ▼
PHASE 9
│
├── Cleanup
├── Error handling
├── Documentation
└── Viva preparation
34. And there's a subtle challenge we should plan for

The hardest part isn't actually:

"How do I start another VM?"

It's this:

How do I reliably create a new VM that already contains the server application and automatically starts it?

We need to solve the VM image/template problem.

I'd approach it like:

server-template VM
        │
        ├── Ubuntu Server
        ├── server binary
        ├── config
        └── systemd service
                │
                ▼
          template disk
                │
                ▼
        server2 disk / VM

Then:

Autoscaler
    │
    ├── create VM from template XML
    ├── attach template/copy disk
    ├── assign network
    └── start VM

That makes the autoscaling process deterministic.

35. One more thing: don't make the application too complicated

The assignment explicitly says the actual client-server application is your choice.

So don't build:

REST API
+ database
+ authentication
+ Redis
+ frontend
+ load balancer
+ ...

That would be cool, but it would bury the actual assignment.

I'd build:

TCP client
      │
      ▼
CPU-intensive TCP server
      │
      ▼
result

Then put the complexity where it matters:

libvirt
monitoring
VM lifecycle
autoscaling
load generation
metrics

That's where the marks are.

36. Your immediate next step

Don't create the project code yet.

First let's get the virtualization layer completely working.

On your Kubuntu host, the first stage should be:

1. Install KVM
2. Install QEMU
3. Install libvirt
4. Install virt-manager
5. Configure your user
6. Verify /dev/kvm
7. Verify libvirt
8. Create Ubuntu Server VM
9. Create second Ubuntu Server VM
10. Configure their networking
11. Understand their XML

Once that's working, we'll move to the C++ side.

And I'd strongly recommend that we do this incrementally: for each phase, first I'll explain why we're doing it and what the underlying virtualization/libvirt concept means, then give you the exact Kubuntu commands/configuration, and only after that move to the next phase. That fits this assignment particularly well because the libvirt API and VM lifecycle are the actual learning objectives, not merely getting a demo to work.
