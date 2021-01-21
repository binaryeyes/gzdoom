#ifndef G_SERIAL_H
#define G_SERIAL_H
// linux specific
#ifdef __linux__
#include <termios.h>
#endif

#ifdef _WIN32

#endif

class SerialBase {
    int _opened;
#ifdef __linux__
    int fd;
    struct termios prev_tty, tty_sets;
#endif
#ifdef _WIN32

#endif

    SerialBase();
protected:
    static SerialBase * _singleton;
  public:
    ~SerialBase();

    void sendReaction(int);
    static SerialBase * getInstance();
    int is_opened() const { return _opened; };
    int open();
    int close();
    int serviceDamage(int);
};

extern SerialBase * sb;

#endif
