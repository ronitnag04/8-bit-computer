# 8-bit Computer
Hi, my name is Ronit Nagarapu, and this is a repository containing my work on breadboard computers. I was inspired to learn more about the fundamental circuits of computers after watching Ben Eater's videos on his [8-bit computer open-source project](https://eater.net/8bit/). 
- **Naga V0** is my version of the basic 8-bit beardboard computer that I built in Summer 2023, with various optimizations for more functionality out of the limited hardware
- **Naga V1** is my design for a more advanced 8-bit computer, which I sketched after I finished building Naga V0.

## Naga V0 Completed Processor
Here is a demo of the 8-bit processor counting up and down by counts of 4, reversing at the bounds of 0 and 255 based on the control flags. 

https://github.com/user-attachments/assets/427f3ba2-040c-49de-a52f-b4d7c6f1c7b2

## Naga V0 Modifications & Planned Improvements:

Here are some modifications I made to the basic 8-bit computer design

- I used a 74LS138 IC to multiplex the output control lines. Since they are exclusive (only one output should be asserted onto the databus at a time), these control lines can be multiplexed, which allows for more non-exclusive general control lines for additional features
- I added a Microcode Step Reset control line to improve instruction cycle times. This reset signal allows simpler instructions to terminate quicker than a constant 6 cycles, allowing for faster programs than a universal standard microcode instruction cycle time that is bottlenecked by the most complex instruction.
- I added a negative flag to the ALU flags register, which can allow for more complex control and program logic.

Here are some potential improvements:
- I have one unused exclusive output control line
- I have one unused output line and one unused general control line
- I have an extra unused address line on the EEPROMs
I can use these to expand the memory register and upgrade the RAM to an 8-bit address and 256 x 8 chip.  I can use the address line on the EEPROMS to expand to 16 -> 32 opcodes or 8 -> 16 max microcode steps. 

## Naga V1 Processor design:
After building the basic breadboard computer and learning more about computer circuits & early computer architectures, I decided to sketch out my own design for a breadboard computer. My hope was that this computer would be able to perform complex programs on a standard ISA. I added I/O to allow the user to interact with the computer, which would allow them to play basic video games or interact with a terminal for essentially a full computing experience.

![Naga V1 Top Design](https://github.com/ronitnag04/8-bit-computer/blob/main/Naga%20V1/Naga%20V1-01.jpg)

Unfortunately, I have not gotten a chance to get all the hardware to build this. Hopefully, I'll make this design come to life one day!
