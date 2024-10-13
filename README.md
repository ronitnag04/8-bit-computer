# 8-bit Computer
Hi, my name is Ronit Nagarapu, and this is a repository containing my 8-bit breadboard computer. This was inspired by Ben Eater's [8-bit computer open-source project](https://eater.net/8bit/). Naga V0 is my version of the basic 8-bit computer, while Naga V1 is my design for an advanced 8-bit computer modeled after the functionality of the Intel 6502 processor.

## Naga V0 Completed Processor
Here is a demo of the 8-bit processor counting up and down by counts of 4, switching at the bounds of 0 and 255 when it detects the zero flag. 

[![Demo](https://img.youtube.com/vi/K0oQ7QJAWTM/0.jpg)](https://www.youtube.com/watch?v=K0oQ7QJAWTM)

## Naga V0 Modifications & Planned Improvements:

Here are some modifications I made to the basic 8-bit computer design

- I used a 74LS138 IC to multiplex the output control lines. Since they are exclusive (only one output should be asserted onto the databus at a time), these control lines can be multiplexed, which allows for more non-exclusive general control lines for additional features
- I added a Microcode Step Reset control line to improve instruction cycle times. This reset signal allows impler instructions to terminate quicker, allowing for faster programs than a universal standard microcode instruction cycle time that is bottlenecked by the most complex instruction.
- I added a negative flag to the ALU flags register, which can allow for more complex control and program logic.

Here are some potential improvements:
- I have one unused exclusive output control line
- I have one unused output line and one unused general control line
- I have an extra unused address line on the EEPROMs
I can use these to expand the memory register and upgrade the RAM to an 8-bit address and 256 x 8 chip.  I can use the address line on the EEPROMS to expand to 16 -> 32 opcodes or 8 -> 16 max microcode steps. 

## Naga V1 Processor design:
