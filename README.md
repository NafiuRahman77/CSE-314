# **CSE-314 Operating System Sessional**

This repository contains implementations of assignments from the Operating System course offered in the Department of Computer Science and Engineering at Bangladesh University of Engineering and Technology (BUET).

---

## **Assignments**

### **Shell Script**

**Offline 1**
- [Problem Statement](/Offline-1/CSE%20314%20Jan%202023%20-%20Shell%20Scripting%20Assignment.pdf)
- [Solution](/Offline-1)

**Online-1**
- [Problem Statement](/Online%-1/b1%20online.pdf)
- [Solution](/Online-1/1905077.sh)

### **Xv6 System Call**

**Offline 2**
- [Problem Statement](/Offline-2/Offline-2-spec.pdf)
- [Solution](/Offline-2/1905077.patch/)

  **How to Apply Patch:**
  1. Place the `1905077.patch` file in the root directory of the xv6 source code.
  2. Apply the patch using the following command:
     ```bash
     git apply 1905077.patch
     ```
  3. Rebuild the xv6 system:
     ```bash
     make clean && make
     ```
  4. Run xv6 in QEMU:
     ```bash
     make qemu
     ```

### **Xv6 Lottery Scheduling**

**Offline 3**
- [Problem Statement](/Offline-3/_CSE-314--assignment-03.pdf)
- [Solution](/Offline-3/1905077.patch/)

  **How to Apply Patch:**
  1. Place the `1905077.patch` file in the root directory of the xv6 source code.
  2. Apply the patch:
     ```bash
     git apply 1905077.patch
     ```
  3. Rebuild the xv6 system:
     ```bash
     make clean && make
     ```
  4. Run xv6 in QEMU:
     ```bash
     make qemu
     ```

### **Inter-Process Communication**

**Offline 4**
- [Problem Statement](/Offline-4/IPC%20Offline.pdf)
- [Solution](/Offline-4/)

  **How to Run:**
  - Compile `main.cpp` along with all other `.hpp` and `.cpp` files in the same directory.
  - Example:
    ```bash
    g++ main.cpp file1.cpp file2.cpp -o offline4
    ./offline4
    ```

**Online-3**
- [Solution](/Online-3/online.cpp)

  **How to Run:**
  - Compile `online.cpp` using g++:
    ```bash
    g++ online.cpp -o online3
    ./online3
    ```

### **Xv6 Threading and Synchronization**

**Offline 5**
- [Problem Statement](/Offline-5/threading_specification.md)
- [Solution](/Offlline-5/1905077.patch/)

  **How to Apply Patch:**
  1. Place the `1905077.patch` file in the root directory of the xv6 source code.
  2. Apply the patch:
     ```bash
     git apply 1905077.patch
     ```
  3. Rebuild the xv6 system:
     ```bash
     make clean && make
     ```
  4. Run xv6 in QEMU:
     ```bash
     make qemu
     ```

---
