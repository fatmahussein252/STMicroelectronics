This task aim is to use real-time signals to simulate terminate and abort signals as follows:

  - If the sent data is 0, then terminate.
  - If the sent data is 1, then abort (terminate with core dump).

To demonstrate this a simple sender and reciever applications provided. The reciever prints its pid and pause to wait for a signal, and the sender prints its pid and uid to compare them when the signal is caught in the reciever using the sigaction structure members.
## How to use
1. enable the core dump files generation for this session if disabled
```ulimit -c unlimited```
2. make the core dump files be generated in the cwd
```echo "core" | sudo tee /proc/sys/kernel/core_pattern```
3. compile tha sender and reciever apps
```
gcc -o rsc realsig_catch.c
gcc -o rss realsig_sender.c
```
4. Run the reciever app and copy its pid 
```./rsc```
5. Run the sender app with the copied pid and 0 or 1 for core dump generation
```
./rss <copied_pid> <0 or 1>
```
## output
![image](https://github.com/user-attachments/assets/2eaa1a23-a9be-4343-8339-e239ae0a875a)



