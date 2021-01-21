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

CVAR(String, serial_device, "/dev/ttyUSB0", CVAR_CONFIG_ONLY);
CVAR(Int, serial_speed, 9600, CVAR_CONFIG_ONLY);
CVAR(Int, serial_width, 8, CVAR_CONFIG_ONLY);
CVAR(Int, serial_stopbit, 1, CVAR_CONFIG_ONLY);
CVAR(String, serial_parity, "n", CVAR_CONFIG_ONLY);

SerialBase * sb;

SerialBase::SerialBase()
{
};

SerialBase::~SerialBase()
{
}

void SerialBase::sendReaction(int c)
{
    // TODO
}
