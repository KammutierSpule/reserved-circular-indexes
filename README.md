# Reserved Circular Indexes

**Circular index with reservation**

An C language algorithm library that manages a given size set of indexes, operating in a circular fashion. It provides a mechanism to advance the head or tail and optionally reserve the tail indexes for deferred processing. While an index is reserved, it is protected from being overwritten by head advances.

It focuses purely on index management, not the underlying data storage.

## Key Features:
  - Index-based management (not tied to data): Only handles circular index logic, allowing external ownership of actual data.
  - Advance Head (Push): Allocates the next available index from the head.
  - Reserve Tail (Pop): Reserves the oldest active index from the tail for reading or processing, and advances the tail.
  - Reservation Safety: Reserved indexes are excluded from overwrite by future head advances.
  - Manual Release: Reserved indexes can later be explicitly marked as free, making them available again.

This pattern is suitable for systems that need deferred consumption, lock-safe buffering, or preemptive inspection of items in FIFO-style flows. Example:

  - Use the same buffer, without copy, to transmit over some stream channel.
  - Reuse the same buffer for in-buffer modification without need to realloc a new buffer or have a temporary buffer. (eg: transform upper case to low case and transmit).
  - On slow systems (eg: microcontrollers) and large buffers, reserve the buffer index until you have process it, while able to use the circular buffer for other proposes (eg: system log, data log).

## ASCII art
```
Buffer Slots (Index):     0     1     2     3     4     5     6     7
                         ----- ----- ----- ----- ----- ----- ----- -----
States:                 |  R  |  T  |  U  |  U  |  U  |  HU |  H  |  F  |
                         ----- ----- ----- ----- ----- ----- ----- -----
                           ↑     ↑                       ↑     ↑      \_ Free slot
                    Reserved     Tail          Head in use     Head (next push)

Legend:
- Reserved (R):      index reserved for delayed processing; won't be overwritten by pushs of the head.
- Tail (T):          oldest valid item (to pop and reserve).
- Used (H):          used blocks (body of the snake).
- Head in use (HU):  current index of the front head.
- Head (H):          where the next push (write) will go (a free slot in this case)
- Free (F):          a free empty slot.
```

## Concepts
### Circular Buffer
The well known Circular Buffer concept with the front head will be increasing and a tail pointer. The head is virtually connected to the tail and will overwrite it if no free slots are available.

### FIFO
First In, First Out nature of a circular buffer.

## Use cases and targets
  - Logs and sensor data storage.
  - Low to high-end range microcontrollers with RAM limitations.
  - Reuse the same storage buffer as a temporary storage for packing transmition or transformation.

## Usage
To integrate the code on your project, you need to consider only the following two folders:
  -    [rescidx/include](/rescidx/include) header files folder which must be added to your include paths.
  -    [rescidx/src](/rescidx/src) source code to be compiled by your project.

You may want to perform a [git sparse checkout](https://git-scm.com/docs/git-sparse-checkout) to your project with only those folders.

No CMAKE file to build project as a library is currently supported.

All other files on this repository are intended for internal use and development of this library.

## Dependencies
None

## License
Distributed under the 3-Clause BSD License. See accompanying file LICENSE or a copy at https://opensource.org/licenses/BSD-3-Clause

SPDX-License-Identifier: BSD-3-Clause

## Contribution
Any contribution to the project be submitted under the same license of this project.

## Development Investment

Open source software doesn't come free. This project represents **14 hours** of development time.

**Time Breakdown:** Core development (6 hrs) • Testing and bug fixing (5 hrs) • Documentation (2 hrs) • Research (1 hrs)

**Appreciate this work?** ⭐ Star the repo • 🐛 Report issues • 💝 [Sponsor/donate]

**Need consulting?** I'm available for custom software development and consulting services. [Contact me](mailto:mrluzeiro@ua.pt).
