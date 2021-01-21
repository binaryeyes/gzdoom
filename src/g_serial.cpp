#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <memory>

#include "i_time.h"
#include "templates.h"
#include "version.h"
#include "doomdef.h" 
#include "doomstat.h"
#include "d_protocol.h"
#include "d_netinf.h"
#include "intermission/intermission.h"
#include "m_argv.h"
#include "m_misc.h"
#include "menu.h"
#include "m_crc32.h"
#include "p_saveg.h"
#include "p_tick.h"
#include "d_main.h"
#include "wi_stuff.h"
#include "hu_stuff.h"
#include "st_stuff.h"
#include "am_map.h"
#include "c_console.h"
#include "c_bind.h"
#include "c_dispatch.h"
#include "filesystem.h"
#include "p_local.h" 
#include "gstrings.h"
#include "r_sky.h"
#include "g_serial.h"
#include "sbar.h"
#include "m_png.h"
#include "a_keys.h"
#include "cmdlib.h"
#include "d_net.h"
#include "d_event.h"
#include "p_acs.h"
#include "p_effect.h"
#include "m_joy.h"
#include "r_utility.h"
#include "a_morph.h"
#include "p_spec.h"
#include "serializer_doom.h"
#include "vm.h"
#include "dobjgc.h"
#include "gi.h"
#include "a_dynlight.h"
#include "i_system.h"
#include "p_conversation.h"
#include "v_palette.h"
#include "s_music.h"
#include "p_setup.h"

#include "v_video.h"
#include "g_hub.h"
#include "g_levellocals.h"
#include "events.h"
#include "c_buttons.h"
#include "d_buttons.h"
#include "hwrenderer/scene/hw_drawinfo.h"
#include "doommenu.h"

#ifdef __linux__
#include <termios.h>
#endif
#ifdef _WIN32
#include <windows.h>
HANDLE hComm;
DCB prev_tty, tty_sets;
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


CVAR(String, serial_device, "/dev/ttyUSB0", CVAR_CONFIG_ONLY);
CVAR(Int, serial_speed, 9600, CVAR_CONFIG_ONLY);
CVAR(Int, serial_width, 8, CVAR_CONFIG_ONLY);
CVAR(Int, serial_stopbit, 1, CVAR_CONFIG_ONLY);
CVAR(String, serial_parity, "n", CVAR_CONFIG_ONLY);
CVAR(String, serial_message1, "~", CVAR_CONFIG_ONLY);
CVAR(String, serial_message2, "!", CVAR_CONFIG_ONLY);

SerialBase * sb;

SerialBase* SerialBase::_singleton = NULL;

SerialBase::SerialBase() : _opened(0)
#ifdef __linux__
, fd(-1)
#endif
{
};

SerialBase::~SerialBase()
{
}

void SerialBase::sendReaction(int c)
{
    // TODO
}

SerialBase * SerialBase::getInstance()
{
    // TODO cover with mutex
    if (_singleton == NULL)
        _singleton = new SerialBase();

    return _singleton;
}

#ifdef __linux__
int SerialBase::open() {
    // TODO implement and override
// linux specific
    fd = ::open(serial_device, O_RDWR | O_NDELAY);

    if (fd >=0)
        _opened = 1;
    else {
        _opened = 0;
        return (fd >= 0);
    }

    memset(&prev_tty, 0, sizeof(struct termios));
    memset(&tty_sets, 0, sizeof(struct termios));
    tcgetattr(fd, &prev_tty);
    tcgetattr(fd, &tty_sets);
    switch(serial_parity[0]) {
        case 'n': default:
            tty_sets.c_cflag &= ~PARENB;
            break;
        case 'e':
            tty_sets.c_cflag |= PARENB;
            break;
        case 'o':
            tty_sets.c_cflag |= PARENB;
            tty_sets.c_cflag |= PARODD;
            break;
    }

    switch(serial_stopbit) {
        case 2:
            tty_sets.c_cflag |= CSTOPB;
            break;
        case 1: default:
            tty_sets.c_cflag &= ~CSTOPB;
        break;
    }

    switch(serial_width) {
        case 8:
            tty_sets.c_cflag |= CS8;
            break;
        case 7:
            tty_sets.c_cflag |= CS7;
            break;
        case 6:
            tty_sets.c_cflag |= CS6;
            break;
        default:
            tty_sets.c_cflag |= CS8;
    }
    tty_sets.c_cflag &= ~CRTSCTS;
    tty_sets.c_cflag |= CREAD | CLOCAL;
    tty_sets.c_lflag &= ~ICANON;
    tty_sets.c_lflag &= ~(ECHO | ECHOE| ECHONL | ISIG );
    tty_sets.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty_sets.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    tty_sets.c_oflag &= ~( OPOST | ONLCR);
    tty_sets.c_cc[VTIME] = 10;
    tty_sets.c_cc[VMIN] = 0;
    cfsetospeed(&tty_sets, serial_speed);
    if (tcsetattr(fd, TCSANOW, &tty_sets) != 0) {
        //printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        Printf ("SerialSetup : Setting up serial connection attributes failed\n");
        ::close(fd);
        fd = -1;
        _opened = 0;
    }

    return (fd >= 0);
}
#endif
#ifdef _WIN32
int SerialBase::open() {
    char serialFullName[200];
    sprintf(serialFullName, "\\\\.\\%s", serial_device.GetHumanString());
    hComm = CreateFileA(serialFullName,                //port name
        GENERIC_READ | GENERIC_WRITE, //Read/Write
        0,                            // No Sharing
        NULL,                         // No Security
        OPEN_EXISTING,// Open existing port only
        0,            // Non Overlapped I/O
        NULL);        // Null for Comm Devices

    if (hComm == INVALID_HANDLE_VALUE) {
        Printf("Error in opening serial port");
        return 0;
    }
    else {
        Printf("opening serial port successful");
        _opened = 1;
    }
    GetCommState(hComm, &prev_tty);
    GetCommState(hComm, &tty_sets);
    tty_sets.DCBlength = sizeof(DCB);
    tty_sets.BaudRate = serial_speed;
    tty_sets.fBinary = TRUE;
    switch (serial_parity[0]) {
        case 'n': default:
            tty_sets.fParity = FALSE;
            tty_sets.Parity = NOPARITY;
            break;
        case 'e':
            tty_sets.fParity = TRUE;
            tty_sets.Parity = EVENPARITY;
            break;
        case 'o':
            tty_sets.fParity = TRUE;
            tty_sets.Parity = ODDPARITY;
            break;
    }
    switch (serial_stopbit) {
        case 2:
            tty_sets.StopBits = TWOSTOPBITS;
            break;
        case 15:
            tty_sets.StopBits = ONE5STOPBITS;
        case 1: default:
            tty_sets.StopBits = ONESTOPBIT;
            break;
    }
    tty_sets.ByteSize = serial_width;
    tty_sets.fOutxCtsFlow = FALSE;
    tty_sets.fOutxDsrFlow = FALSE;
    tty_sets.fDtrControl = DTR_CONTROL_DISABLE;
    tty_sets.fRtsControl = RTS_CONTROL_DISABLE;
    tty_sets.fNull = FALSE;
    tty_sets.fInX = FALSE;
    tty_sets.fOutX = FALSE;
    tty_sets.wReserved = 0;
    if (!SetCommState(hComm, &tty_sets))
    {
        Printf("SerialSetup : Setting up serial connection attributes failed\n");
        CloseHandle(hComm);//Closing the Serial Port
        _opened = 0;
    }
    return 1;
}
#endif

int SerialBase::close()
{
    if (! _opened)
        return 0;

    // TODO implement
#ifdef __linux__
    ::close(fd);

#endif
#ifdef _WIN32
    CloseHandle(hComm);
#endif
    _opened = 0;
    return -1;
}

int SerialBase::serviceDamage(int val)
{
    //for now we'll use 2 type of messages
    if (!_opened)
        return -1;

    int rv = -1;
#ifdef __linux__
    if ( val > 6) {
        rv = write(fd, serial_message1, 1);
    } else {
        rv = write(fd, serial_message2, 1);
    }
#endif
#ifdef _WIN32
    DWORD written;
    BOOL status;
    if (val > 6)
    {
        status = WriteFile(hComm, serial_message1, 1, &written, NULL);
    }
    else {
        status = WriteFile(hComm, serial_message2, 1, &written, NULL);
    }
    rv = status;
#endif
    return rv;
}

