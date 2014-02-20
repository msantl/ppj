#ifndef AKCIJA
#define AKCIJA

#include <string>

typedef enum {POMAK, REDUCIRAJ, PRIHVATI, ODBACI} type;

class akcija{
  private:
    type tip;
    int state;
    int value;
    std::string LHS;
  public:

    akcija(void);

    int getType(void);
    void setType(type);

    int getState(void);
    int getValue(void);
    std::string getLHS(void);

    void setState(int);
    void setValue(int);
    void setLHS(std::string);

    void clear(void);

};

#endif
