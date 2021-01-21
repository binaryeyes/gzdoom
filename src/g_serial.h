#ifndef G_SERIAL_H
#define G_SERIAL_H

class SerialBase {
  public:
    SerialBase();
    ~SerialBase();

    void sendReaction(int);
};

extern SerialBase * sb;

#endif
