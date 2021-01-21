#ifndef G_SERIAL_H
#define G_SERIAL_H
// linux specific
#include <termios.h>

class SerialBase {
    int _opened;
    int fd;
    SerialBase();
    struct termios prev_tty, tty_sets;
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
