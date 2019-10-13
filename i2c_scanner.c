/*

The MIT License

Copyright (c) 2019 Hanyazou

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/


#include <stdio.h>
#include <libmpsse/mpsse.h>

int
main(int argc, char **argv)
{
    struct mpsse_context *mpsse = NULL;

    if ((mpsse = MPSSE(I2C, FOUR_HUNDRED_KHZ, MSB)) == NULL || !mpsse->open) {
        fprintf(stderr, "MPSSE open failed: %s\n", ErrorString(mpsse));
	exit(1);
    }

    int addr;
    int prev_status;
    int prev_addr;
    for (addr = 0; addr <= 0x80; ++addr) {
        char addr_wr = addr << 1;
	int found;

        Start(mpsse);
        Write(mpsse, &addr_wr, 1);
        if (addr < 0x80) {
            found = (GetAck(mpsse) == ACK) ? 1 : 0;
        } else {
            found = !prev_status;
	}
        if (addr == 0) {
          prev_status = found;
          prev_addr = addr;
        } else
        if (prev_status != found) {
            if (prev_addr == addr - 1) {
                printf("%sfound at %02X\n", prev_status ? "" : "not ", prev_addr);
            } else {
                printf("%sfound at %02X - %02X\n", prev_status ? "" : "not ", prev_addr, addr -1);
            }
            prev_status = found;
            prev_addr = addr;
        }

        Stop(mpsse);
    }
    Close(mpsse);

    exit(0);
}
