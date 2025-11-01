# MyOS Development Roadmap

This document outlines future features and enhancements for MyOS, providing a learning path from basic OS concepts to advanced system programming.

## Current Status (v0.1.0)

- ✅ ARM64 bootloader
- ✅ UART serial console
- ✅ Basic memory allocator (bump allocator)
- ✅ In-memory file system (32 files, 4KB each)
- ✅ Interactive shell with basic commands
- ⚠️ Runtime issue: Shell prompt not appearing (being debugged)

---

## Phase 1: Foundation Fixes & Improvements

### Priority: High | Complexity: Low-Medium

**1.1 Fix Current Runtime Issues**
- Debug and resolve shell prompt hanging issue
- Ensure all commands work reliably
- Fix any memory allocation bugs

**1.2 Enhanced Shell Features**
- Command history (up/down arrow keys)
- Tab completion for filenames and commands
- Line editing with backspace, delete, home, end keys
- Support for quoted arguments: `edit "my file.txt" "content here"`

**1.3 Additional File System Commands**
- `cp <src> <dst>` - Copy files
- `mv <src> <dst>` - Move/rename files
- `touch <file>` - Create empty file
- `find <pattern>` - Search for files by name

**1.4 Improved Memory Management**
- Replace bump allocator with free-list allocator
- Implement proper `free()` functionality
- Add memory usage statistics command
- Detect and report memory leaks

**Dependencies:** None
**Learning Resources:**
- [OSDev: Memory Allocation](https://wiki.osdev.org/Memory_Allocation)
- [Free-list implementation guide](https://wiki.osdev.org/Heap)

---

## Phase 2: Advanced File System & Storage

### Priority: High | Complexity: Medium

**2.1 Disk I/O Support**
- Implement virtio-blk driver for QEMU
- Block device read/write operations
- Disk buffer cache for performance

**2.2 Persistent File System**
- Choose file system: FAT32 (compatible) or custom (educational)
- Implement directory support with `mkdir`, `cd`, `pwd`, `rmdir`
- File system mounting and unmounting
- Support for absolute and relative paths

**2.3 File System Features**
- File permissions and ownership
- Timestamps (created, modified, accessed)
- Symbolic links
- File seeking (read from specific offset)
- Larger file support (currently limited to 4KB)

**Dependencies:** Phase 1 completion
**Learning Resources:**
- [OSDev: Disk Access](https://wiki.osdev.org/Disk_Access)
- [FAT32 Specification](https://wiki.osdev.org/FAT)
- [virtio-blk driver guide](https://docs.oasis-open.org/virtio/virtio/v1.1/virtio-v1.1.html)

---

## Phase 3: Networking Stack

### Priority: High | Complexity: High

**3.1 Network Driver**
- Implement virtio-net driver for QEMU
- Ethernet frame handling
- MAC address configuration
- DMA buffer management

**3.2 TCP/IP Stack Implementation**

**Layer 2 - Data Link:**
- Ethernet frame parsing
- ARP (Address Resolution Protocol)
- ARP cache management

**Layer 3 - Network:**
- IPv4 packet handling
- ICMP (ping support)
- IP routing (basic single-route)
- Fragmentation/reassembly

**Layer 4 - Transport:**
- UDP protocol (simpler, start here)
- TCP protocol (connection management, flow control, retransmission)
- Socket API (create, bind, listen, accept, connect, send, recv)

**3.3 Network Utilities**
- `ping <host>` - ICMP echo request/reply
- `ifconfig` - Network interface configuration
- `netstat` - Display network connections
- `route` - Display/modify routing table

**Dependencies:** Phase 1, Phase 2 (for configuration files)
**Learning Resources:**
- [OSDev: Networking](https://wiki.osdev.org/Networking)
- [TCP/IP Illustrated](https://en.wikipedia.org/wiki/TCP/IP_Illustrated)
- [lwIP - Lightweight TCP/IP](https://savannah.nongnu.org/projects/lwip/)
- [virtio-net specification](https://docs.oasis-open.org/virtio/virtio/v1.1/virtio-v1.1.html)

---

## Phase 4: HTTP Client & File Transfer

### Priority: Medium | Complexity: Medium-High

**4.1 DNS Resolution**
- UDP-based DNS client
- DNS query/response parsing
- `/etc/resolv.conf` support for nameservers
- DNS caching

**4.2 HTTP/HTTPS Client (curl-like)**

**HTTP Support:**
- HTTP/1.1 protocol implementation
- Request methods: GET, POST, PUT, DELETE
- Headers parsing and generation
- Chunked transfer encoding
- Redirect following
- Cookie support

**HTTPS/TLS Support:**
- TLS 1.2/1.3 handshake
- Certificate validation (basic or using mbedTLS library)
- Encrypted communication

**4.3 File Transfer Tool**
- `curl <url>` - Download file from URL
- `curl -X POST -d "data" <url>` - Upload data
- `curl -o <file> <url>` - Save to specific file
- Progress indicators
- Resume interrupted downloads
- Authentication (basic, bearer token)

**4.4 Higher-level Utilities**
- `wget <url>` - Recursive file download
- FTP client (optional)

**Dependencies:** Phase 3 (networking stack)
**Learning Resources:**
- [HTTP/1.1 RFC](https://tools.ietf.org/html/rfc2616)
- [DNS RFC](https://tools.ietf.org/html/rfc1035)
- [mbedTLS library](https://github.com/Mbed-TLS/mbedtls)
- [TLS 1.3 RFC](https://tools.ietf.org/html/rfc8446)

---

## Phase 5: Process Management & Multitasking

### Priority: Medium | Complexity: High

**5.1 Process Management**
- Process Control Block (PCB) structure
- Process creation (`fork`, `exec`)
- Process termination
- Process states (running, ready, blocked)
- Process ID (PID) management

**5.2 Scheduler**
- Round-robin scheduling
- Priority-based scheduling
- Context switching (save/restore CPU registers)
- Interrupt handling for timer-based preemption

**5.3 Inter-Process Communication (IPC)**
- Pipes
- Message queues
- Shared memory
- Signals

**5.4 System Calls**
- System call interface (SVC instruction on ARM64)
- User mode vs kernel mode separation
- System call dispatcher

**Dependencies:** Phase 1, Phase 2
**Learning Resources:**
- [OSDev: Processes](https://wiki.osdev.org/Processes)
- [OSDev: Scheduling](https://wiki.osdev.org/Scheduling_Algorithms)
- [ARM64 Exception Levels](https://developer.arm.com/documentation/102412/latest/)

---

## Phase 6: Graphics & X Server

### Priority: Medium | Complexity: Very High

**6.1 Framebuffer Driver**
- virtio-gpu driver for QEMU
- Pixel buffer management
- Color depth configuration (RGB565, RGB888)
- Screen resolution detection/setting
- Double buffering for flicker-free updates

**6.2 Graphics Primitives**
- Pixel plotting
- Line drawing (Bresenham's algorithm)
- Rectangle drawing (filled/outlined)
- Circle drawing
- Text rendering (bitmap fonts)
- Image loading (BMP, PNG)

**6.3 Window Manager**
- Window data structures
- Window creation, destruction, movement
- Window stacking (Z-order)
- Input event distribution (mouse, keyboard)
- Window decorations (title bar, borders)

**6.4 X11 Server Implementation**

**Core X11 Protocol:**
- X11 connection handling
- Protocol parsing (X11 requests/replies)
- Resource management (windows, pixmaps, fonts)
- Graphics contexts
- Event delivery

**X Extensions:**
- XSHM (shared memory)
- XRANDR (resolution switching)
- Input extensions (XI2)

**6.5 Simple GUI Applications**
- Terminal emulator
- Text editor
- File manager
- Clock/calendar

**Dependencies:** Phase 5 (multitasking for window manager)
**Learning Resources:**
- [OSDev: Framebuffer](https://wiki.osdev.org/Framebuffer)
- [virtio-gpu specification](https://docs.oasis-open.org/virtio/virtio/v1.1/virtio-v1.1.html)
- [X11 Protocol](https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html)
- [Tiny X Server (TinyX)](https://www.freedesktop.org/wiki/Software/TinyX/)
- [Writing a Simple Framebuffer Driver](https://wiki.osdev.org/User:Omarrx024/Writing_a_Framebuffer_Driver)

---

## Phase 7: Advanced Features

### Priority: Low | Complexity: Varies

**7.1 USB Support**
- USB host controller driver (EHCI/XHCI)
- USB device enumeration
- USB mass storage (flash drives)
- USB input devices (keyboard, mouse)

**7.2 Audio Support**
- Audio driver (AC'97 or Intel HDA)
- PCM playback
- Audio file format support (WAV, MP3)
- Mixer controls

**7.3 Dynamic Linking & Libraries**
- ELF loader
- Shared library support (.so files)
- Dynamic linker
- Standard C library port (musl or newlib)

**7.4 Package Manager**
- Package format definition
- Package repository
- Dependency resolution
- Install/uninstall packages

**7.5 Self-Hosting Capability**
- Port GCC or Clang to MyOS
- Assembler and linker
- Make or build system
- Ability to compile MyOS from within MyOS

**Dependencies:** Most previous phases
**Learning Resources:**
- [OSDev: USB](https://wiki.osdev.org/USB)
- [OSDev: Sound](https://wiki.osdev.org/Sound)
- [ELF Specification](https://refspecs.linuxfoundation.org/elf/elf.pdf)

---

## Implementation Strategy

### General Approach

1. **Start Simple:** Implement basic version first, then add features
2. **Test Incrementally:** Test each component thoroughly before moving on
3. **Document Everything:** Keep code well-commented for learning
4. **Use QEMU Features:** Leverage virtio drivers for easier implementation
5. **Study Existing Code:** Look at Linux, BSD, or other educational OSes

### ARM64-Specific Considerations

- **Memory Management:** ARM64 has a sophisticated MMU - implement page tables for Phase 5
- **Cache Management:** ARM64 requires explicit cache management for DMA
- **GIC (Generic Interrupt Controller):** Needed for interrupts in networking and graphics
- **PSCI (Power State Coordination Interface):** For multi-core management
- **NEON/SIMD:** Can optimize graphics operations significantly

### Testing Strategy

- Use QEMU for all development and testing
- Create automated test suites for each component
- Test on real ARM64 hardware (Raspberry Pi 4) once stable
- Implement debugging tools (stack traces, memory dumps)

---

## Recommended Learning Path

### For Networking & curl:

1. Complete Phase 1 (foundation)
2. Implement Phase 3.1 (network driver)
3. Implement Phase 3.2 UDP first
4. Implement Phase 4.1 (DNS)
5. Implement Phase 3.2 TCP
6. Implement Phase 4.2-4.3 (HTTP client)

**Estimated Timeline:** 3-6 months of part-time work

### For X Server:

1. Complete Phase 1-2 (foundation + file system)
2. Implement Phase 5 (process management)
3. Implement Phase 6.1-6.2 (framebuffer + graphics)
4. Implement Phase 6.3 (window manager)
5. Optionally implement full X11

**Estimated Timeline:** 6-12 months of part-time work

---

## Alternative/Simplified Approaches

### Networking (Easier Path):

Instead of implementing full TCP/IP from scratch:
- Use [lwIP](https://savannah.nongnu.org/projects/lwip/) - lightweight TCP/IP stack
- Port it to your OS (well-documented, educational, and production-ready)

### Graphics (Easier Path):

Instead of full X11 server:
- Implement framebuffer-only graphics (no windowing)
- Use [Wayland](https://wayland.freedesktop.org/) protocol (simpler than X11)
- Or implement a simple custom GUI toolkit

---

## Resources & References

### Books
- "Operating Systems: Three Easy Pieces" - Remzi Arpaci-Dusseau
- "The Design and Implementation of the FreeBSD Operating System"
- "Linux Device Drivers" - Jonathan Corbet
- "TCP/IP Illustrated" - W. Richard Stevens

### Websites
- [OSDev Wiki](https://wiki.osdev.org/) - Comprehensive OS development resource
- [ARM Developer Documentation](https://developer.arm.com/documentation/)
- [QEMU Documentation](https://www.qemu.org/docs/master/)

### Example OS Projects
- [SerenityOS](https://github.com/SerenityOS/serenity) - Modern, feature-rich OS
- [ToaruOS](https://github.com/klange/toaruos) - Includes graphics and network
- [xv6](https://github.com/mit-pdos/xv6-public) - Educational Unix-like OS

### Forums & Communities
- [OSDev Forums](https://forum.osdev.org/)
- [r/osdev](https://www.reddit.com/r/osdev/)

---

## Contributing Ideas

As you implement features, consider:
- Documenting challenges and solutions
- Creating tutorial blog posts
- Contributing to OSDev wiki
- Helping other OS developers

---

**Last Updated:** 2025-11-01
**Current Version:** 0.1.0 (Initial Development)
